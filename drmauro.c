#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "drmauro.h"

#include "myfunctions.h"

void carica_campo(struct gioco *gioco, char *percorso) {
	FILE *fd;
	char c = ' ';
	enum colore colour;
	int i = 0, j = 0;


	srand(time(NULL));
	inizializza_matrice(gioco, VUOTO, NO_COLOUR, -1);

	fd = fopen(percorso, "r");

	if(fd == NULL)/* controllo se ho aperto correttamente il file */
	{
		printf("errore apertura file\n");
		return;
	}

	while(i < RIGHE && !feof(fd))
	{
		j = 0;
		c = ' ';
		while(j < COLONNE && c != '\n')
		{
			fscanf(fd, "%c", &c); /* leggo 1 carattere alla volta nel file */
			printf("carattere letto da file: %c\n",c);
			switch(c)
			{
				case 'R':
					colour = ROSSO;
					break;
				case 'G':
					colour = GIALLO;
					break;
				case 'B':
					colour = BLU;
					break;
				default:
					colour = NO_COLOUR;
			}

			if(c != '\n')
			{
				if(colour != NO_COLOUR)
				{
					setta_cella(gioco, i, j, MOSTRO, colour, _GLOBAL_ID);
					_GLOBAL_ID ++;
				}
				else
				{
					setta_cella(gioco, i, j, VUOTO, colour, _GLOBAL_ID);
					_GLOBAL_ID ++;
				}
			}

			j ++;
		}
		while(c != '\n' && !feof(fd))
		{
			fscanf(fd, "%c", &c);
		}
		i ++;
	}

	fclose(fd); /* chiudo il file */

	genera_pastiglia(gioco);

}

void riempi_campo(struct gioco *gioco, int difficolta) {

	srand(time(NULL));

	inizializza_matrice(gioco, VUOTO, NO_COLOUR, -1);

	genera_pastiglia(gioco);

	setta_campo(gioco, difficolta); /* inserisco i mostri */
}

void step(struct gioco *gioco, enum mossa comando) {
	int id_attivo = gioco->active_id, orie, virus_deleted = 0, cadu = 1, mosse_consecutive_max = 3;
	double moltiplicatore = 1;
	int i_tmp = 0, j_tmp = 0, totale_tmp = 0;

	inizializza_vuoti(gioco); /* setto a -1 gli id delle celle vuote della matrice */

	orie = orientamento(gioco, id_attivo);
	setta_perno(gioco, id_attivo);


	if(!caduta(gioco) && !consecutive(gioco, &i_tmp, &j_tmp, &totale_tmp, ROSSO) && !consecutive(gioco, &i_tmp, &j_tmp, &totale_tmp, GIALLO) && !consecutive(gioco, &i_tmp, &j_tmp, &totale_tmp, BLU))
	{

		if(comando == GIU)
		{
			tasto_giu(gioco);
			comando = NONE;
		}

		cadu = 1;
		if(comando == DESTRA || comando == SINISTRA) /* || comando == GIU)*/
		{
			if(orie == 1 && comando != GIU)
			{
				if(movimento_valido(gioco, id_attivo, comando))
				{
					muovi_pastiglia(gioco, comando, id_attivo);
					muovi_pastiglia(gioco, comando, id_attivo + 1);
				}
				else
					comando = NONE;
			}
			else
			{
				if(movimento_valido(gioco, id_attivo, comando) && movimento_valido(gioco, id_attivo + 1, comando))
				{
					muovi_pastiglia(gioco, comando, id_attivo);
					muovi_pastiglia(gioco, comando, id_attivo + 1);
				}
				else
				{
					if(comando == DESTRA || comando == SINISTRA) /* aggiunto il 13-06-2018, se la pastiglia è verticale e non posso muovermi a destra o a sinistra è come se avessi eseguito la mossa NONE, quindi devo andare perforza giu di 1 */
						comando = NONE;
				}
			}
		}
		else
		{
			if(comando == ROT_DX || comando == ROT_SX)
				rotazione_past(gioco, id_attivo, comando);
			else /* aggiunto 13-06-2018 per poter fare più mosse consecutive valide prima di far scendere la pastiglia */
			{
				if(comando == NONE)
					_MOSSA_CONS = 0;
			}
		}

		_MOSSA_CONS ++;

		if(movimento_valido(gioco, id_attivo, GIU) && movimento_valido(gioco, id_attivo + 1, GIU)) /* mando sempre la pastiglia in giu di 1 posizione */
		{
			if(_MOSSA_CONS >= mosse_consecutive_max || comando == NONE || comando == GIU || comando == ROT_DX || comando == ROT_SX) /* controllo aggiunto 13-06-2018 per poter fare più mosse consecutive prima di far scendere la pastiglia */
			{
				muovi_pastiglia(gioco, GIU, id_attivo + 1);
				muovi_pastiglia(gioco, GIU, id_attivo);
				_MOSSA_CONS = 0;
			}
		}
		else
		{
			if((gioco->campo[0][(COLONNE-1)/2 + 0]).id == -1 && (gioco->campo[0][(COLONNE-1)/2 + 1]).id == -1)/* aggiunto 08-06-2018 per risolvere problema che quando ho solo un pezzo di pastiglia nella posizione iniziale non posso generare una nuova pastiglia e quindi ho perso */
				genera_pastiglia(gioco);
			else
			{
					_LOST = 1; /* variabile creata 08-06-2018 */
			}
		}
	}
	virus_deleted = elimina_consecutive(gioco); /* elimino le eventuali celle consecutive e conto quanti virus tra esse, sono stati eliminati */
	if(virus_deleted != 0)
	{
		if(cadu) /* se la caduta delle pastiglie non è all'inizio */
			moltiplicatore = 1;
		else /* se la caduta delle pastigie non è ancora finita */
			moltiplicatore *= 2;

		(gioco->punti) += moltiplicatore * sommatoria(virus_deleted);
		cadu = 0;
	}
}

enum stato vittoria(struct gioco *gioco) {
	int i = 0, j = 0, add = 0, id_attivo = gioco->active_id, pos_iniziale = 1;

	if(_LOST && !caduta(gioco)) /* aggiunto 08-06-2018 */
		return SCONFITTA;

	if(conta_tipo(gioco, MOSTRO) == 0)
		return VITTORIA;


	while(add < 2 && pos_iniziale)/* controllo se la pastiglia è nella posizione iniziale */
	{
		posiziona(gioco, &i, &j, id_attivo + add);

		if(!(i == 0 && j == (COLONNE - 1) / 2 + add))/* se la pastiglia non è nella posizione iniziale */
		{
			pos_iniziale = 0;
		}
		add ++;
	}

	if(!pos_iniziale)
		return IN_CORSO;
	else
	{
		if((!movimento_valido(gioco, id_attivo, GIU) || !movimento_valido(gioco, id_attivo + 1, GIU))) /* se la pastiglia è alla posizione iniziale ed entrambe non possono muoversi in basso -> ritorno il valore SCONFITTA, altrimenti ritorno il valore IN_CORSO */
		{
			if(((gioco->campo[2][(COLONNE - 1)/2 + 0]).id != -1 && (gioco->campo[2][(COLONNE - 1)/2 + 1]).id != -1)) /* aggiunto 15-06-2018 per risolvere bug che quando viene generata una nuova pastiglia e nella posizione +1 riga, c'è una pastiglia che deve cadere, anche se il movimento riga +1 non è valido non ho perso perchè la pastiglia deve ancora cadere */
			{
				return SCONFITTA;
			}
			else
				return IN_CORSO;
		}
		else
			return IN_CORSO;
	}
}
