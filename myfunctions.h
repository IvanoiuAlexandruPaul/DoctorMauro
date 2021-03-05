#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H

#include <math.h>

int _GLOBAL_ID = 1; 	/*variabile globale che viene asegnata ad ogni cella aumentata ogni volta per identificare le celle*/
int
 = 0; 				/*se ho un pezzo di pastiglia nella posizione iniziale, non posso generare un nuova pastiglia e quindi ho perso */
int _MOSSA_CONS = 0; 	/*variabile per far scendere la pastiglia dopo tot movimenti validi */

/*funzioni create*/
enum contenuto genera_colore(); /* funzione che genera 1 dei 3 colori random */
void setta_cella(struct gioco *gioco, int r, int c, enum contenuto t, enum colore cl, int identificatore); /* funzione che setta la cella dell'identificatore, con i parametri passati */
void inizializza_matrice(struct gioco *gioco, enum contenuto t, enum colore c, int identificatore); /* funzione che inizializza tutta la matrice con i parametri passati */
int setta_campo(struct gioco *gioco, int diffic); /* funzione che mi inserisce correttamente i mostri nella matrice */
int correttezza_virus(struct gioco *gioco); /* funzione che verifica che tutti i mostri siano posizionati in maniera corretta -> richiamata nella foo setta_campo */
enum contenuto genera_contenuto(int tipologie); /* funzione che mi genera un tipo di cella random, fino a tipologie tipi */
void stampa_matrice(struct gioco *gioco);
int conta_celle(struct gioco *gioco, int r_start, int c_start); /* funzione che conta le celle della matrice a partire da r_start e c_start */
void genera_pastiglia(struct gioco *gioco); /* funzione che genera una nuova pastiglia, all'inizio della matrice */
void posiziona(struct gioco *gioco, int *r, int *c, int identificatore); /* funzione che setta gli indici r e c, mettendoli alla posizione in cui si trova la cella identificatore */
int muovi_pastiglia(struct gioco *gioco, enum mossa comd, int identificatore); /* funzione che muove la pastiglia in base alla mossa comd */
int movimento_valido(struct gioco *gioco, int identificatore, enum mossa comd); /* funzione che verifica se il movimento comd che si vuole fare, è valido */
int conta_tipo(struct gioco *gioco, enum contenuto type); /* funzione che conta quante celle di tipi type ci sono nella matrice */
int orientamento(struct gioco *gioco, int identificatore); /* funzione che guarda se la pastiglia identificatore è messa in orrizontale o verticale, valore di ritorno -> 1: orrizontale, 2: verticale */
void setta_perno(struct gioco *gioco, int identificatore); /* funzione che mette l'id identificatore nella cella perno */
void rotazione_past(struct gioco *gioco, int identificatore, enum mossa comd); /* funzione che verifica e ruota la pastiglia in base alla mossa comd, se è possibile ruotarla */
int consecutive(struct gioco *gioco, int *r, int *c, int *n, enum colore cl); /* funzione che trova almeno o più 4 celle consecutive, setta r e c alla posizione di inizio sequenza e setta n, in base a quante celle consecutive ci sono */
int elimina_consecutive(struct gioco *gioco); /* funzione che elimina le celle consecutive -> richiama foo consecutive */
int caduta(struct gioco *gioco); /* funzione che verifica se delle celle possono cadere, se si le sposta in giu di 1 posizione */
int compagno_near(struct gioco *gioco, int identificatore); /* funzione che guarda se ha il suo compagno vicino, valori restituiti -> 0: non c'è il compagno vicino, 1: il compagno è sopra, 2: il compagno è a destra, 3: il compagno è giu, 4: il compagno è a sinistra */
double sommatoria(int n);
int tasto_giu(struct gioco *gioco);
void inizializza_vuoti(struct gioco *gioco); /* funzione che mette a -1  tutti gli id delle celle vuote della matrice */


enum contenuto genera_colore(){ /*restituisce 1 dei 3 colori generati casualmente*/
	int c;
	c = rand() % 3;
	switch(c)
	{
		case 0:
			return ROSSO;
			break;
		case 1:
			return GIALLO;
			break;
		default:
			return BLU;
	}
}


void setta_cella(struct gioco *gioco, int r, int c, enum contenuto t, enum colore cl, int identificatore){ /*setta la cella della matrice che si vuole modificare con i parametri che gli vengono passati*/
	(gioco->campo[r][c]).tipo = t;
	(gioco->campo[r][c]).colore = cl;
	(gioco->campo[r][c]).id = identificatore;

return;
}

void inizializza_matrice(struct gioco *gioco, enum contenuto t, enum colore c, int identificatore){
	int i = 0, j = 0;
	while(i < RIGHE)
	{
		j = 0;
		while(j < COLONNE)
		{
			setta_cella(gioco, i, j, t, c, identificatore);
			j++;
		}
		i++;
	}
return;
}

enum contenuto genera_contenuto(int tipologie){ /* tipologie=0 : VUOTO,  tipologie=1: MOSTRO, tipologie=altro: PASTIGLIA*/
	int tipo;

	tipo = rand() % tipologie;
	switch(tipo)
	{
		case 0:
			return VUOTO;
			break;
		case 1:
			tipo = rand() % tipologie;
			if(tipo == 1)
			{
				return MOSTRO;
			}
			else
				return VUOTO;
			break;
		default:
			return PASTIGLIA;
	}
}

int correttezza_virus(struct gioco *gioco){
	int i = 0, j = 0, tripla = 0,cont = 1;
	enum colore c_prec, c_attu;
	enum contenuto tipo_attu;

	c_prec = (gioco->campo[i][j]).colore;
	c_attu = c_prec;

	while(i < RIGHE && !tripla) /*controllo orrizontale*/
	{
		j = 0;
		c_prec = (gioco->campo[i][j]).colore;
		c_attu = c_prec;
		cont = 1;
		while((j + 1) < COLONNE && !tripla)
		{
			c_attu = (gioco->campo[i][j + 1]).colore;
			tipo_attu = (gioco->campo[i][j + 1]).tipo;
			if(c_attu == c_prec && tipo_attu == MOSTRO)
				cont ++;
			else
				cont = 1;

			if(cont == 3)
				tripla = 1;

			c_prec = c_attu;
			j++;
		}
		i++;
	}

	if(tripla)
		return !tripla;

	i = 0; j = 0; tripla = 0; cont = 1;

	c_prec = (gioco->campo[i][j]).colore;
	c_attu = c_prec;
	cont = 1;
	while(j < COLONNE && !tripla)/*controllo verticale*/
	{
		i = 0;
		c_prec = (gioco->campo[i][j]).colore;
		c_attu = c_prec;
		while((i + 1) < RIGHE && !tripla)
		{
			c_attu = (gioco->campo[i + 1][j]).colore;
			tipo_attu = (gioco->campo[i + 1][j]).tipo;
			if(c_attu == c_prec && tipo_attu == MOSTRO)
				cont ++;
			else
				cont = 1;

			if(cont == 3)
				tripla = 1;

			c_prec = c_attu;
			i++;
		}
		j++;
	}

return !tripla;
}

void stampa_matrice(struct gioco *gioco){
	int i = 0, j = 0;

	printf("\n\nstampa matrice:\n");
	while(i < RIGHE)
	{
		j = 0;
		while(j < COLONNE)
		{
			if((gioco->campo[i][j]).colore == NO_COLOUR)
				printf("%d ",0);
			else
				printf("%d ",1);
			j++;
		}
		printf("\n");
		i++;
	}
}

int setta_campo(struct gioco *gioco, int diffic){ /*la funzione restituisce il numero di mostri aggiunti nella matrice*/
	enum colore colour;
	enum contenuto type;
	int i = 5, j = 0, tmp, n_virus;
	int free_pos;

	if(diffic < 0) /*controllo aggiunto 13-06-2018 per difficolta inserite dall'utente non comprese tra 0 e 15 */
		diffic = 0;
	else
		if(diffic > 15)
			diffic = 15;

	n_virus = 4 * (diffic + 1);
	tmp = _GLOBAL_ID;  /*variabile che mi server per fare la differenza tra il primo id della cella e l'ultimo*/

	while(i < RIGHE)
	{
		j=0;
		while(j < COLONNE)
		{
			free_pos = conta_celle(gioco, i, j) - n_virus; /* numero di posizioni in cui potrei generare ancora celle vuote */
			do{
				colour = genera_colore();
				if(n_virus > 0)
				{
					if(free_pos > 0)
						type = genera_contenuto(2); /*genera un contenuto tra i primi 2 tipi: 0 o 1 (VUOTO o MOSTRO)*/
					else
						type = MOSTRO;
				}
				else
					type = VUOTO;

				if(type != VUOTO)
					setta_cella(gioco, i, j, type, colour, _GLOBAL_ID);
				else
					setta_cella(gioco, i, j, type, NO_COLOUR, _GLOBAL_ID);
			}while(!correttezza_virus(gioco));/*rigenero lo stesso virus finchè la matrice non è corretta*/

			_GLOBAL_ID++;
			if(n_virus > 0 && (gioco->campo[i][j]).tipo == MOSTRO)
				n_virus --;

			j++;
		}
		i++;
	}
return _GLOBAL_ID - tmp;
}

int conta_celle(struct gioco *gioco, int r_start, int c_start){
	int i = r_start, j = c_start, count = 0;

	while(i < RIGHE)
	{
		if(i == r_start)
			j = r_start;
		else
			j = 0;
		while(j < COLONNE)
		{
			count ++;
			j++;
		}
		i++;
	}

return count;
}

void genera_pastiglia(struct gioco *gioco){
	enum colore colour;
	int i = 0;

	while(i < 2)
	{
		colour = genera_colore();
		setta_cella(gioco, 0, (COLONNE - 1) / 2 + i, PASTIGLIA, colour, _GLOBAL_ID);
		if(i == 0)
			gioco->active_id = _GLOBAL_ID;

		_GLOBAL_ID ++;
		i ++;

		if(i == 2)
			_GLOBAL_ID ++;/* salto 1 id per ogni pastiglia diversa */
	}

return;
}

void posiziona(struct gioco *gioco, int *r, int *c, int identificatore){
	int trovato = 0;

	(*r) = 0; (*c) = 0;
	while((*r) < RIGHE && !trovato)
	{
		(*c) = 0;
		while((*c) < COLONNE && !trovato)
		{
			if((gioco->campo[(*r)][(*c)]).id == identificatore)
				trovato = 1;
			(*c) ++;
		}
		(*r) ++;
	}

	(*r) --;
	(*c) --;

return;
}

int muovi_pastiglia(struct gioco *gioco, enum mossa comd, int identificatore){ /* ritorna 1 se il movimento è possibile, 0 altrimenti */
	int spostamento_col = 0, spostamento_rig = 0, i = 0, j = 0;

	switch(comd)
	{
		case DESTRA:
			spostamento_col = 1;
			break;
		case SINISTRA:
			spostamento_col = -1;
			break;
		case GIU:
			spostamento_rig = 1;
			break;
		case SU:
			spostamento_rig = -1;
			break;
		case DIAG_DX:							/*per la pastiglia in posizione vertical --> poi la pastiglia verrà messa in posizione orizzontale*/
			spostamento_rig = 1;
			spostamento_col = 1;
			break;
		case DIAG_SX:							/*per la pastiglia in posizione orizzontale --> poi la pastiglia verrà messa in posizione verticale*/
			spostamento_rig = -1;
			spostamento_col = -1;
			break;
		default:
			break;
	}

	posiziona(gioco, &i, &j, identificatore);

	setta_cella(gioco, i + spostamento_rig, j + spostamento_col, (gioco->campo[i][j]).tipo, (gioco->campo[i][j]).colore, (gioco->campo[i][j]).id);/* sposto la pastiglia al movimento successivo */
	setta_cella(gioco, i, j, VUOTO, NO_COLOUR, -1);/* rimuovo la pastiglia dalla sua cella precedente */

return 0;
}

int movimento_valido(struct gioco *gioco, int identificatore, enum mossa comd){
	int i = 0, j = 0, spostamento_col = 0, spostamento_rig = 0;

	switch(comd)
	{
		case DESTRA:
			spostamento_col = 1;
			break;
		case SINISTRA:
			spostamento_col = -1;
			break;
		case GIU:
			spostamento_rig = 1;
			break;
		case SU:
			spostamento_rig = -1;
			break;
		case DIAG_DX:
			spostamento_rig = 1;
			spostamento_col = 1;
			break;
		case DIAG_SX:
			spostamento_rig = -1;
			spostamento_col = -1;
			break;
		default:
			break;
	}

	posiziona(gioco, &i, &j, identificatore);
	if(j + spostamento_col >= 0 && j + spostamento_col < COLONNE && i + spostamento_rig >= 0 && i + spostamento_rig < RIGHE)/* controllo se muovendomi resto all'interno del campo */
	{
		if((gioco->campo[i + spostamento_rig][j + spostamento_col]).tipo == VUOTO)/* se al movimento successivo non ce niente posso spostarmi */
			return 1;
		else
		{
			if((gioco->campo[i + spostamento_rig][j + spostamento_col]).tipo == PASTIGLIA)/* se al movimento successivo ho una pastiglia */
			{
				if((gioco->campo[i][j]).id - (gioco->campo[i + spostamento_rig][j + spostamento_col]).id == -1 || (gioco->campo[i][j]).id - (gioco->campo[i + spostamento_rig][j + spostamento_col]).id == 1)/* controllo se è il mio compagno */
				{
					int tmp;
					tmp = (gioco->campo[i][j]).id;/* scambio gli id delle 2 pastiglie in modo da verificare se il movimento della prima pastiglia che deve muoversi è valido */
					(gioco->campo[i][j]).id = (gioco->campo[i + spostamento_rig][j + spostamento_col]).id;
					(gioco->campo[i + spostamento_rig][j + spostamento_col]).id = tmp;

					if(movimento_valido(gioco, identificatore, comd))
						return 1;
					else
						return 0;
				}
				else
					return 0;
			}
			else
				return 0;
		}
	}
	else
		return 0;
}

int conta_tipo(struct gioco *gioco, enum contenuto type){
	int i = 0, j = 0, count = 0;

	while(i < RIGHE)
	{
		j = 0;
		while(j < COLONNE)
		{
			if((gioco->campo[i][j]).tipo == type)
				count ++;

			j ++;
		}
		i ++;
	}

return count;
}

int orientamento(struct gioco *gioco, int identificatore){
	int i = 0, j = 0, spostamento = 1, trovato = 0;

	posiziona(gioco, &i, &j, identificatore);

		while(spostamento != -3 && !trovato)
		{
			if(j + spostamento >= 0 && j + spostamento < COLONNE)
			{
				if((gioco->campo[i][j]).id - (gioco->campo[i][j + spostamento]).id == -1 || (gioco->campo[i][j]).id - (gioco->campo[i][j + spostamento]).id == 1) /* se è il mio compagno */
					trovato = 1;
			}
			spostamento -= 2;
		}

	if(trovato)
		return 1;/* 1: la pastiglia è orrizontale */
	else
		return 2;/* la pastiglia è verticale */
}

void setta_perno(struct gioco *gioco, int identificatore){ /* funzione che mette identificatore nella cella perno */
	int i = 0, j = 0, spostamento_col = 0, spostamento_rig = 0, orie;

	posiziona(gioco, &i, &j, identificatore);

	orie = orientamento(gioco, identificatore);

	switch(orie)
	{
		case 1: /* la pastiglia è orrizontale */
			spostamento_col = 1;
			break;
		case 2: /*la pastiglia è verticale*/
			spostamento_rig = -1;
			break;
	}

		if(i + spostamento_rig >= 0 && i + spostamento_rig < RIGHE && j + spostamento_col >= 0 && j + spostamento_col < COLONNE) /* controllo se col spostamento esco dal campo */
		{
			if(!((gioco->campo[i][j]).id - (gioco->campo[i + spostamento_rig][j + spostamento_col]).id == -1 || (gioco->campo[i][j]).id - (gioco->campo[i + spostamento_rig][j + spostamento_col]).id == 1)) /* se a destra(pastiglia orrizintale) o sopra(pastiglia verticale) non ho il mio compagno, scambio il mio id con la cella a sinistra(pastiglia orizzontale) o in basso(pastiglia verticale), in modo che il mio id vada sulla cella perno */
			{
				int tmp;
				tmp = (gioco->campo[i][j]).id;
				(gioco->campo[i][j]).id = (gioco->campo[i - spostamento_rig][j - spostamento_col]).id;
				(gioco->campo[i - spostamento_rig][j - spostamento_col]).id = tmp;
			}
		}
		else /* se uscirei dal campo, è come se alla mia destra(pastiglia orizzontale) o sopra(pastiglia verticale) non avessi il mio compagno */
		{
			int tmp;
			tmp = (gioco->campo[i][j]).id;
			(gioco->campo[i][j]).id = (gioco->campo[i - spostamento_rig][j - spostamento_col]).id;
			(gioco->campo[i - spostamento_rig][j - spostamento_col]).id = tmp;
		}

}

void rotazione_past(struct gioco *gioco, int identificatore, enum mossa comd){
	int orie, i = 0, j = 0;
	/*char colour; tmp */

	setta_perno(gioco, identificatore);/* metto l'id dell'identificatore sulla cella perno */
	posiziona(gioco, &i, &j, identificatore + 1);
	orie = orientamento(gioco, identificatore);

	switch(comd)
	{
		case ROT_DX:
			if(orie == 1) /* pastiglia orrizontale */
			{
				if(movimento_valido(gioco, identificatore, SU))
				{
					muovi_pastiglia(gioco, SU, identificatore);
					muovi_pastiglia(gioco, SINISTRA, identificatore + 1);
				}
			}
			else /*pastiglia verticale*/
			{
				if(movimento_valido(gioco, identificatore + 1, DIAG_DX))
					muovi_pastiglia(gioco, DIAG_DX, identificatore + 1);
			}
			break;
		case ROT_SX:
			if(orie == 1) /* pastiglia orrizontale */
			{
				if(movimento_valido(gioco, identificatore + 1, DIAG_SX))
					muovi_pastiglia(gioco, DIAG_SX, identificatore + 1);
			}
			else /*pastiglia verticale*/
			{
				if(movimento_valido(gioco, identificatore, DESTRA))
				{
					muovi_pastiglia(gioco, DESTRA, identificatore);
					muovi_pastiglia(gioco, GIU, identificatore + 1);
				}
			}
			break;
		default:
			break;
	}
}

int consecutive(struct gioco *gioco, int *r, int *c, int *n, enum colore cl){ /* funzione che dice se ci sono n celle consecutive di colore cl */
	int trovato = 0;

	/* verifica orizzontale */
	(*r) = 0;
	(*c) = 0;
	(*n) = 0;

	while((*r) < RIGHE && !trovato)
	{
		(*n) = 0;
		(*c) = 0;
		while((*c) < COLONNE && !trovato)
		{
			if((gioco->campo[(*r)][(*c)]).colore == cl && (gioco->campo[(*r)][(*c)]).id != gioco->active_id && (gioco->campo[(*r)][(*c)]).id != (gioco->active_id) + 1)
			{
				(*n) ++;
			}
			else
				(*n) = 0;

			if((*n) == 4)
			{
				(*c) ++;
				while((gioco->campo[(*r)][(*c)]).colore == cl && (gioco->campo[(*r)][(*c)]).id != gioco->active_id && (gioco->campo[(*r)][(*c)]).id != (gioco->active_id) + 1 && (*c) < COLONNE)
				{
					if((gioco->campo[(*r)][(*c)]).colore == cl && (gioco->campo[(*r)][(*c)]).id != gioco->active_id && (gioco->campo[(*r)][(*c)]).id != (gioco->active_id) + 1)
						(*n) ++;

					(*c) ++;
				}
				trovato = 1;
			}
			(*c) ++;
		}
		(*r) ++;
	}

	(*r) --;
	(*c) -= 2;

	if((*n) >= 4)
		(*c) -= (*n) - 1;

	if(trovato)
		return 1;/* la sequenza è orizzontale */

	/* verifica verticale */

	(*r) = 0; (*c) = 0; (*n) = 0;

	while((*c) < COLONNE && !trovato)
	{
		(*n) = 0;
		(*r) = 0;
		while((*r) < RIGHE && !trovato)
		{
			if((gioco->campo[(*r)][(*c)]).colore == cl && (gioco->campo[(*r)][(*c)]).id != gioco->active_id && (gioco->campo[(*r)][(*c)]).id != (gioco->active_id) + 1)
			{
				(*n) ++;
			}
			else
				(*n) = 0;

			if((*n) == 4)
			{
				(*r) ++;
				while((gioco->campo[(*r)][(*c)]).colore == cl && (gioco->campo[(*r)][(*c)]).id != gioco->active_id && (gioco->campo[(*r)][(*c)]).id != (gioco->active_id) + 1 && (*r) < RIGHE)
				{
					if((gioco->campo[(*r)][(*c)]).colore == cl && (gioco->campo[(*r)][(*c)]).id != gioco->active_id && (gioco->campo[(*r)][(*c)]).id != (gioco->active_id) + 1)
						(*n) ++;

					(*r) ++;
				}
				trovato = 1;
			}

			(*r) ++;
		}
		(*c) ++;
	}

	(*c) --;
	(*r) -= 2;

	if((*n) >= 4)
		(*r) -= (*n) - 1;

	if(trovato)
		return 2;/* la sequenza è verticale */
	else
		return 0; /* non esiste una sequenza di colori >= 4 */
}

int elimina_consecutive(struct gioco *gioco){ /* ritorna il numero di virus eliminati */
	int i = 0, j = 0, tot = 0, direzione = 1, virus_iniziali = 0, virus_rimasti = 0, virus_eliminati = 0;

	if((direzione = consecutive(gioco, &i, &j, &tot, ROSSO)) || (direzione = consecutive(gioco, &i, &j, &tot, GIALLO)) || (direzione = consecutive(gioco, &i, &j, &tot, BLU))) /* se ho una sequenza eliminabile */
	{
		virus_iniziali = conta_tipo(gioco, MOSTRO); /* conto i virus presenti nella matrice prima di procedere all'eliminazione della sequenza */
		while(tot > 0)
		{
			setta_cella(gioco, i, j, VUOTO, NO_COLOUR, -1);
			if(direzione == 1)
				j ++;
			else
				i ++;

			tot --;
		}
		virus_rimasti = conta_tipo(gioco, MOSTRO);
		virus_eliminati = virus_iniziali - virus_rimasti;

		i = 0;
		j = 0;
	}

return virus_eliminati;
}

int caduta(struct gioco *gioco){
	int i = RIGHE - 1, j = COLONNE - 1, trovato = 0;

	while(i >= 0)
	{
		j = COLONNE - 1;
		while(j >= 0)
		{
			if(i + 1 < RIGHE)
			{
				if((gioco->campo[i][j]).tipo == PASTIGLIA && (gioco->campo[(i)][(j)]).id != gioco->active_id && (gioco->campo[(i)][(j)]).id != (gioco->active_id) + 1 && !compagno_near(gioco, (gioco->campo[(i)][(j)]).id)) /* se la cella è una pastiglia e non è la pastiglia attiva e vicino non ha il compagno */
				{
					if((gioco->campo[i + 1][j]).tipo == VUOTO) /* se sotto la pastiglia non c'è niente sposta la pastiglia in basso */
					{
							setta_cella(gioco, i + 1, j, (gioco->campo[i][j]).tipo, (gioco->campo[i][j]).colore, (gioco->campo[i][j]).id);/* sposta la pastiglia in basso */
							setta_cella(gioco, i, j, VUOTO, NO_COLOUR, -1);/* elimina la pastiglia dalla sua vecchia cella */
							trovato = 1;
					}
				}
				else /* se la pastiglia non è singola guardo che sotto di lei non ci sia un buco, altrimenti la sposto sotto */
				{
					if((gioco->campo[i][j]).tipo == PASTIGLIA && (gioco->campo[(i)][(j)]).id != gioco->active_id && (gioco->campo[(i)][(j)]).id != (gioco->active_id) + 1 && compagno_near(gioco, (gioco->campo[(i)][(j)]).id)) /* se la cella è una pastiglia e non è la cella attiva e vicino a lei ha il compagno */
					{
						int direzione, orie, spostamento_rig = 0, spostamento_col = 0;

						direzione = compagno_near(gioco, (gioco->campo[(i)][(j)]).id);
						orie = orientamento(gioco, (gioco->campo[(i)][(j)]).id);

						switch(direzione)
						{
							case 1:
								spostamento_rig = -1;
								break;
							case 2:
								spostamento_col = 1;
								break;
							case 3:
								spostamento_rig = 1;
								break;
							case 4:
								spostamento_col = -1;
								break;
							default:
								break;
						}

						if(orie == 2 && ((gioco->campo[i + 1][j]).tipo == VUOTO || (gioco->campo[i + spostamento_rig + 1][j + spostamento_col]).tipo == VUOTO)) /* se la pastiglia è verticale basta che sotto ad una delle 2 ci sia il buco per spostarla */
						{
							if(movimento_valido(gioco, (gioco->campo[i][j]).id, GIU))
							{
								muovi_pastiglia(gioco, GIU, (gioco->campo[i][j]).id);
								muovi_pastiglia(gioco, GIU, ((gioco->campo[i][j]).id) + 1);
								trovato = 1;
							}
						}
						else
						{
							if(orie == 1 && (gioco->campo[i + 1][j]).tipo == VUOTO && (gioco->campo[i + spostamento_rig + 1][j + spostamento_col]).tipo == VUOTO) /* se la pastiglia è orrizontale, sotto entrambe deve esserci il buco per poterle muovere in basso */
							{
								if(movimento_valido(gioco, (gioco->campo[i][j]).id, GIU))
								{
									muovi_pastiglia(gioco, GIU, (gioco->campo[i][j]).id);
									muovi_pastiglia(gioco, GIU, ((gioco->campo[i][j]).id) + 1);
									trovato = 1;
								}
							}
						}
					}
				}
			}
			j --;
		}
		i --;
	}

return trovato;
}

int compagno_near(struct gioco *gioco, int identificatore){
	int i = 0, j = 0, spostamento_rig = 0, spostamento_col = 0, id_past, trovato = 0, add = 0;

	posiziona(gioco, &i, &j, identificatore);

	id_past = (gioco->campo[i][j]).id;

	while(add < 4 && !trovato)
	{
		switch(add)
		{
			case 0:
				spostamento_rig = 1;
				spostamento_col = 0;
				break;
			case 1:
				spostamento_rig = -1;
				spostamento_col = 0;
				break;
			case 2:
				spostamento_col = 1;
				spostamento_rig = 0;
				break;
			case 3:
				spostamento_col = -1;
				spostamento_rig = 0;
				break;
			default:
				break;
		}

		if(i + spostamento_rig >= 0 && i + spostamento_rig < RIGHE && j + spostamento_col >= 0 && j + spostamento_col < COLONNE) /* controllo che guardando la cella successiva non esco dalla matrice */
			if((gioco->campo[i + spostamento_rig][j + spostamento_col]).id - id_past == -1 || (gioco->campo[i + spostamento_rig][j + spostamento_col]).id - id_past == 1) /* guarda se vicino ho il mio compagno */
			{
				switch(spostamento_rig)
				{
					case -1: /* compagno su */
						trovato = 1;
						break;
					case 1: /* compagno giu */
						trovato = 3;
						break;
					default:
						switch(spostamento_col)
						{
							case 1: /* compagno a destra */
								trovato = 2;
								break;
							case -1: /* compagno a sinistra */
								trovato = 4;
								break;
							default:
								break;
						}
				}
			}
		add ++;
	}

return trovato;
}

double sommatoria(int n){
	double i = 1, somma_parz = 0;

	while(i <= (double)n)
	{
		somma_parz += 100 * (pow(2,i));
		i ++;
	}

return somma_parz;
}

int tasto_giu(struct gioco *gioco){
	int i = 0, j = 0, trovato = 1, orie;

	setta_perno(gioco, (gioco->active_id));
	orie = orientamento(gioco, (gioco->active_id));
	posiziona(gioco, &i, &j, (gioco->active_id));

	while(trovato)
	{
		trovato = 0;
		posiziona(gioco, &i, &j, (gioco->active_id));
		if(i + 1 < RIGHE)
		{
			if(orie == 1 && ((gioco->campo[i + 1][j]).tipo) == VUOTO && ((gioco->campo[i + 1][j + 1]).tipo) == VUOTO) /* se la pastiglia è orrizontale, guardo che sotto entrambe ci sia il buco per poterle muovere(sono sicuro che l'active_id è a sinistra dato che l'ho settato come perno) */
			{
				muovi_pastiglia(gioco, GIU, (gioco->active_id));
				muovi_pastiglia(gioco, GIU, ((gioco->active_id) + 1));
				trovato = 1;
			}
			else
			{
				if(orie == 2 && ((gioco->campo[i + 1][j]).tipo) == VUOTO) /* se la pastiglia è verticale, basta che sotto all'active_id(settato come perno, ovvero è sotto) ci sia il buco per poterla muovere */
				{
					muovi_pastiglia(gioco, GIU, (gioco->active_id));
					muovi_pastiglia(gioco, GIU, ((gioco->active_id) + 1));
					trovato = 1;
				}
				else
				{

					trovato = 0;
				}
			}
		}
		else
			trovato = 0;
	}


return trovato;
}

void inizializza_vuoti(struct gioco *gioco){
	int i = 0, j = 0;

	while(i < RIGHE)
	{
		j = 0;
		while(j < COLONNE)
		{
			if((gioco->campo[i][j]).tipo == VUOTO)
				setta_cella(gioco, i, j, VUOTO, NO_COLOUR, -1);
			j ++;
		}
		i ++;
	}

}

#endif
