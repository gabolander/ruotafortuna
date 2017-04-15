/*
 * This program is copyrighted to Gabriele Zappi (C)
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <curses.h>
/* #include "strfunz.c" */
#include "ruota.h"

#ifndef strlower
#define strlower strlow
#endif

/* defines e macrodefines */
// in "ruota.h"

/* typedefs */
// in "ruota.h"
														 

/* prototipi di funzione */
#include "myproto.h"


/* Variabili globali */
char * Prgname;   // Puntatore a carattere da assegnare.
WINDOW * Win;

int ges_fine()
{
	_premi_un_tasto("Premere un tasto per terminare.\n\r");

  endwin();
  exit(0);
}

/*
 *     funzione main()
 */
int main(int argc, char *argv[])
{
	S_GAMEPROFILE * gp;
	int i,j;
	int ret;

#if 0
	printf( "Dimensione della struttura * gp = %d\n\r", sizeof( *gp ) );
	exit(0);
#endif

	// imposta la funzione da eseguire prima dell'uscita comandata o provocata
	atexit( (void *) ges_fine ); // al return o exit del main()
  signal(SIGINT, (void *) ges_fine); // in caso di interruzione
  signal(SIGTERM, (void *) ges_fine); // in caso di interruzione
  signal(SIGKILL, (void *) ges_fine); // in caso di interruzione
  signal(SIGQUIT, (void *) ges_fine); // in caso di interruzione

	/* 
	 * Siccome Prgname e' un puntatore, devo allocare memoria per 
	 * decidere (dinamicamente) la sua dimensione. Deve tenere il nome
	 * del programma chiamato senza path, quindi sicuramente non puo' 
	 * essere piu' lungo di argv[0]. Sono quindi sicuro che e' una
	 * dimensione sufficiente.
	 */
	Prgname = malloc( strlen( argv[0] ) );
	if( Prgname == NULL ) {
		printf( "Errore di allocazione memoria per Prgname\n\r" );
		exit(-1);
	}

	/* Prendo il nome escludendo l'eventuale pathname */
	strcpy( Prgname, (strrchr(argv[0],'/'))? strrchr(argv[0],'/') + 1 : argv[0] );

#ifdef DEBUG_VERBOSE
	fprintf(stdout, "argv[0]= %s\n\r", argv[0] );
	fprintf(stdout, "Prgname = %s\n\r", Prgname );
	fflush(stdout);
#endif


  Win=initscr();
	start_color();
  crmode();
  noecho();
	curs_set(0);
	keypad(stdscr,TRUE);
  erase();
  refresh();

	/*
	 * Alloco memoria per la principale struttura di gioco.
	 */

	if ( (gp = malloc(sizeof(* gp))) == NULL ){
		printf( "Errore: non posso allocare tanta memoria per la struttura di gioco\n\r" );
		exit(-1);
	}
	
	for(i=0;i<COLORS;i++)
		for(j=0;j<COLORS;j++)
			init_pair(i*COLORS+j,j, i);

	init_game_struct( gp );
	/* _debug_struct( gp ); */
	
	ret=imposta_match( gp );
	if( ret==9 ) {
		exit(0);
	}


	// _debug_struct( gp );
	

	do {
		ret = gioca( gp );
	} while( ret!=9 && !gp->winner );

	if( gp->winner ) {
		color_set( 61 ,NULL);
		vittoria( gp );
	}

	clear();
	refresh();

	_debug_struct( gp );


	return(0);
	exit(0);
}


/*
 *     funzione init_game_struct( S_GAMEPROFILE * gp ) 
 */
int init_game_struct( S_GAMEPROFILE * gp ) 
{
	register int i;

	/* Per prudenza, riempio tutta la struttura a zero */
	memset( ( S_GAMEPROFILE *) gp, 0, sizeof( * gp ) );

	for(i=0;i<MAX_NUM_PLAYER;i++) {
		sprintf(gp->az_giocatore[i],"Giocatore %d",(i+1));
		gp->al_credito[i]=0L;
	}
	gp->z_parola[0]='\0';
	for(i=0;i<MAX_LEN_PAROLA;i++) 
			gp->ac_lettere_prese[i]=0;

	memset((char *)gp->ac_consonanti_prese,'\0',NUM_CONSONANTI);
	memset((char *)gp->ac_vocali_prese,'\0',NUM_VOCALI);
	gp->turno = 0;
	gp->winner = 0;
	gp->l_passaggio = 0L;
	gp->i_punti_turno = 0;
	gp->bit_azioni_permesse = 0;

	return EXIT_SUCCESS;
}

/*
 *     funzione _debug_struct( S_GAMEPROFILE * gp ) 
 */
int _debug_struct( S_GAMEPROFILE * gp ) 
{
#ifdef DEBUG
	register int i;

	printf("Dimensione struttura = %d bytes\r\n\r\n", sizeof( * gp ));

	for(i=0;i<MAX_NUM_PLAYER;i++) {
		printf("Giocatore %d : [%s]\n\r",i+1,gp->az_giocatore[i]);
		printf("Credito Giocatore %d : %ld\n\r",i+1, gp->al_credito[i]);
	}
	printf("Parola : [%s]\n\r", gp->z_parola);

	printf("lettere prese: ");
	for(i=0;i<MAX_LEN_PAROLA;i++) {
			printf("%d=%c ",i ,(gp->ac_lettere_prese)?gp->ac_lettere_prese[i]:'_');
	}
	printf("\n\r");

	printf("consonanti prese: ");
	for(i=0;i<NUM_CONSONANTI;i++) {
			printf("%d=%c ",i ,(gp->ac_consonanti_prese)?gp->ac_consonanti_prese[i]:'_');
	}
	printf("\n\r");

	printf("vocali prese: ");
	for(i=0;i<NUM_VOCALI;i++) {
			printf("%d=%c ",i ,(gp->ac_vocali_prese)?gp->ac_vocali_prese[i]:'_');
	}
	printf("\n\r");

	printf("E' il turno %d \r\n", gp->turno );
	printf("Winner is %d \r\n", gp->winner );
	printf("Passaggio %ld \r\n", gp->l_passaggio );

	_premi_un_tasto("");

	fflush(stdout);
#endif

	return EXIT_SUCCESS;
}

/*
 *     funzione imposta_match( S_GAMEPROFILE * gp ) 
 */
int imposta_match( S_GAMEPROFILE * gp ) 
{
	//int i,j;
	clock_t clk;
	unsigned int rand_seed;
	struct tms *tempo;

	FILE *fin;
	char zriga[MAXRIGA];
	int nrighe=0;
	int nriga=0;

	if( access( FILE_DIZIONARIO, 0 ) != 0 )
	{
		printf( "file %s inaccessibile\n\r", FILE_DIZIONARIO );
		exit(-1);
	}

	if ((fin=fopen(FILE_DIZIONARIO,"r")) == NULL)  /* apertura file in lettura */
	{
		/* se la fopen ritorna null, significa che l'apertura del file
		sorgente e fallita,  quindi ... */
		printf("%s: non posso aprire il file in input \"%s\" \n\r",Prgname,FILE_DIZIONARIO);
		fclose (fin);
		exit (-1);    /* ...ed esce */
	}


	nrighe=0;
	while ( !feof(fin) )
	{
		memset(zriga, 0, sizeof( zriga ) );
		fgets(zriga, 590, fin ); 
		if( feof(fin) )
		{
#ifdef DEBUG_VERBOSE
		 	fprintf(stdout, "Fine file.\n Conenuto ultima riga: \"%s\"\n\r", zriga );
#endif
			break;
		}
		// puldim( zriga, 590 ); 
		strpulall( zriga );
#ifdef DEBUG_VERBOSE
		 	fprintf(stdout, "Il valore della riga e' \"%s\"\n\r", zriga );
#endif
		if( strvuota(zriga) )
			continue;

		++nrighe;
	}
#ifdef DEBUG_VERBOSE
		 	fprintf(stdout, "Il numero di righe piene e' %d\n\r", nrighe );
#endif

	fflush(stdout);

	fclose(fin);

	// Inizializzo il generatore di numeri casuali
	/* clk = clock(); */
	tempo = malloc( sizeof(struct tms) );
	clk = times( tempo );
	rand_seed = (unsigned int)((long)clk % 32768);
	srand( rand_seed );

	// Prendo una riga in automatico
	nriga=tira_numero_random( nrighe );
#ifdef DEBUG_VERBOSE
		 	fprintf(stdout, "prendo la riga %d\n\r", nriga );
#endif

	if ((fin=fopen(FILE_DIZIONARIO,"r")) == NULL)  /* apertura file in lettura */
	{
		printf("%s: Non posso aprire il file in input \"%s\" \n\r",Prgname,FILE_DIZIONARIO);
		fclose (fin);
		exit (-1);    /* ...ed esce */
	}
	nrighe=0;
	while ( !feof(fin) )
	{
		memset(zriga, 0, sizeof( zriga ) );
		fgets(zriga, 590, fin ); 
		if( feof(fin) )
		{
			break;
		}
		// puldim( zriga, 590 ); 
		strpulall( zriga );
		if( strvuota(zriga) )
			continue;

		if((++nrighe)>=nriga) break;
	}
#ifdef DEBUG_VERBOSE
	fprintf(stdout, "Il valore della parola scelta e' \"%s\"\n\r", zriga );
	fflush(stdout);
#endif
	fclose(fin);

	strlower( zriga );

	strncpy0(gp->z_parola,zriga,MIN(MAX_LEN_PAROLA,strlen(zriga)));

	return EXIT_SUCCESS;
}

/*
 *     funzione gioca( S_GAMEPROFILE * gp ) 
 */
int gioca( S_GAMEPROFILE * gp ) 
{
	static int c=0;
	static long volta_entrato=0L;
	int ret,pos;
	char z_tmp[513];
	// int i;

	// init variabili locali
	++volta_entrato; pos=0;

	if( volta_entrato == 1 ) {
		move(0,0);
		clear();
	}
	refresh();

	maschera_base( gp );

	headline( gp );

#ifdef DEBUG
	sprintf(z_tmp,"giro:%ld pass:%ld",volta_entrato,gp->l_passaggio);
	pos=80-(strlen(z_tmp)+3); 
	if(pos<0) pos=0;
	move(1,pos);
	printw("[%s]",z_tmp);
#endif

	statusline( gp );

#ifdef DEBUG_VERBOSE
	move(LINEA_STATUSLINE,0);
	printw("Tasto premuto : ");
	if( c )
		printw("%c dec(%d) hex(%x) ", c,c,c );
#endif

	// mvprintw(15,35,"Qualcosa");
	// disegna_box( 5, 1, 3, 30, 6, TRUE, 62, "Titolo" );
	
	// disegno box della ruota
	// disegna_box( 5, 40, 3, 39, 6, TRUE, 0, "Ruota" );
	
	stato_giocatori( gp );

	ruota( gp, FALSE );


	quadro_parola( gp );


	mostra_consonanti( gp );
	mostra_vocali( gp );

	fflush(stdout);

	analisi_gioco( gp );  /* Qui guardo a che punto sono (finito consonati, ecc.) */
	if( gp->winner )
		return EXIT_SUCCESS;

	c=getch();

	c=tolower(c);
	move(22,0);
	clrtoeol();
	fflush(stdout);
	
	switch(c)
	{
		case 'q':
			return 9;
			break;
		case 'g':
			if( gp->bit_azioni_permesse && !( gp->bit_azioni_permesse & AZ_CONSONANTE ) ) 
			{
				msgalert( "Consonanti finite! "); beep();
			}
			else
			{
				ret = ruota( gp, TRUE );
				scegli_consonante( gp );
			}
			break;
		case 's':
			ret = dai_soluzione( gp );
			break;
		case 'v':
			if( gp->bit_azioni_permesse && !( gp->bit_azioni_permesse & AZ_VOCALE ) ) 
			{
				msgalert( "Vocali finite! "); beep();
			}
			else
			{
				ret = compra_vocale( gp );
				break;
			}
		case 'h':
		case '?':
			aiuto();
			refresh();
			break;
		default:
			move(22,10);
			color_set( 15 ,NULL);
			printw("Scelta '%c' (dec %d) non valida!",c,c);
			beep();
			fflush(stdout);
			break;
	}

	analisi_gioco( gp );  /* Qui guardo a che punto sono (finito consonati, ecc.) */

	return EXIT_SUCCESS;
}

/*
 *     funzione maschera_base( S_GAMEPROFILE * gp )
 */
int maschera_base( S_GAMEPROFILE * gp )
{
	register int i;
	int pos;
	/* Traccio le linee orizzontali */
	color_set( 1 ,NULL);
	move(1,0);  hline( ACS_HLINE, 80 );
	move(21,0); hline( ACS_HLINE, 80 );
	move(23,0); hline( ACS_HLINE, 80 );

	/* Cancello il corpo della videata */
	color_set( 0 ,NULL);
	for(i=2;i<21;i++) {
		mvhline( i,0, ' ', 80 );
	}

#ifdef DEBUG
	color_set( 1 ,NULL);
	pos=80-(strlen(gp->z_parola)+3); 
	if(pos<0) pos=0;
	move(23,pos);
	printw("[%s]",gp->z_parola);
#endif

	return 0;
}

/*
 *     funzione headline( S_GAMEPROFILE * gp )
 */
int headline( S_GAMEPROFILE * gp )
{
	/* HEAD-LINE: Version info - name, etc... */
	move(0,0);
	color_set( 2 ,NULL);
	printw("%20.20s - Versione: ",PROGRAMNAME);
	color_set( 0 ,NULL);
	printw("%6.6s",VERSIONE);

	return 0;
}

/*
 *     funzione statusline( S_GAMEPROFILE * gp )
 */
int statusline( S_GAMEPROFILE * gp )
{
	move(LINEA_STATUSLINE,0);
	color_set( 31 ,NULL);
	hline(' ',80);
#ifndef DEBUG_VERBOSE
	move(LINEA_STATUSLINE,0);
	printw(" Premere: (H)elp, (G)ira la ruota, (S)oluzione, (V)ocale ");
#endif
	move(LINEA_STATUSLINE,61);
	printw(" - 'q/Q' = uscita ");
	return 0;
}


/*
 *     funzione dai_soluzione( S_GAMEPROFILE * gp )
 */
int dai_soluzione( S_GAMEPROFILE * gp )
{
	char z_risposta[MAX_LEN_PAROLA+1];
	int ret;

	z_risposta[0]=0;

	/*
  winp=initscr();
	start_color();
  crmode();
  noecho();
  erase();
  refresh();
	box(winp,ACS_VLINE,ACS_HLINE);
	*/

	mvprintw(18,0,"Inserire risposta: ");
	ret=inputcontr( 18,19, z_risposta, strlen(gp->z_parola), ALFABETO, 'L', "", '_' );

#ifdef DEBUG_VERBOSE
	mvprintw(20,0,"La risposta inserita e' [%s]",z_risposta,"");

	_premi_un_tasto("");
#endif

	if( strcmp( gp->z_parola, z_risposta ) ) {
		gp->turno++;
		if( gp->turno > 1 ) gp->turno=0;

		gp->winner=2; /* mi segno che ha vinto l'altro giocatore */
	}
	else
		gp->winner=1;

	return 0;
}

/*
 *     funzione aiuto()
 */
void aiuto()
{
	WINDOW *wh;
	int c;
	FILE *fh;
	const char *file_help="aiuto.txt";
	char zriga[191];
	int riga=0;
	bool nofile;

	wh = newwin(18,70,4,5);
	box(wh,0,0);

	if( access( file_help, 0 ) != 0 )
	{
		nofile=TRUE;
	}
	if ((fh=fopen(file_help,"r")) == NULL)  /* apertura file in lettura */
	{
		/* se la fopen ritorna null, significa che l'apertura del file
		sorgente e fallita,  quindi ... */
		nofile=TRUE;
	}

	if( nofile ) {
		mvwprintw(wh,1,1,"Scrivo qualcosa");
	} else {
		riga=0;
		while(fgets(zriga,180,fh)) {
			riga++;
			pul(zriga); strpuldx(zriga);
			mvwprintw(wh,riga,1,zriga);
		}
		fclose(fh);
	}


	mvwprintw(wh,17,1,"[premi un tasto per chiudere questa finestra]");


	wrefresh(wh);
	c=getch();
	wborder(wh, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(wh);
	delwin(wh);
}

#define COLORE_NUMERO_SPENTO 39
#define COLORE_NUMERO_ACCESO 56
/*
 *     funzione ruota( S_GAMEPROFILE * gp, bool la_giro )
 */
int ruota( S_GAMEPROFILE * gp, bool la_giro )
{
	// 100, 200, 500 e 1000
	// costo vocale=1000
	register int i;
	static int pos=0;
	int num,pos_act;
#ifdef DEBUG_VERBOSE
	char buffer[70];
#endif

	const char valori[4][7] = {"  100 ", "  200 ", "  500 ", " 1000 " };

	disegna_box( 8, 26, 3, 26, 6, TRUE, 0, "Ruota" );
	for(i=0;i<4;i++) {
		color_set( (pos==i)?COLORE_NUMERO_ACCESO:COLORE_NUMERO_SPENTO ,NULL);
		mvprintw(9,27+(6*i),valori[i]);
	}

	gp->i_punti_turno=0L;

	if (la_giro)
	{
		num=tira_numero_random( 4 )-1;
		pos_act=pos;
#ifdef DEBUG_VERBOSE
			sprintf(buffer, "Valore di num = %d", num );
			mvprintw(22,1,buffer);
#endif

		// giri veloci
		for(i=0;i<30;i++) {
			color_set( COLORE_NUMERO_SPENTO ,NULL);
			mvprintw(9,27+(6*pos_act),valori[pos_act]);
			if((++pos_act)>=4) pos_act=0;
			color_set( COLORE_NUMERO_ACCESO ,NULL);
			mvprintw(9,27+(6*pos_act),valori[pos_act]);

			// fflush(stdout);
			refresh();
			usleep(25000);
		}
		// giri meno veloci
		for(i=0;i<10;i++) {
			color_set( COLORE_NUMERO_SPENTO ,NULL);
			mvprintw(9,27+(6*pos_act),valori[pos_act]);
			if((++pos_act)>=4) pos_act=0;
			color_set( COLORE_NUMERO_ACCESO ,NULL);
			mvprintw(9,27+(6*pos_act),valori[pos_act]);
			// fflush(stdout);
			refresh();
			beep();
			usleep(50000);
		}
		// giri meno veloci ancora
		for(i=0;i<10;i++) {
			color_set( COLORE_NUMERO_SPENTO ,NULL);
			mvprintw(9,27+(6*pos_act),valori[pos_act]);
			if((++pos_act)>=4) pos_act=0;
			color_set( COLORE_NUMERO_ACCESO ,NULL);
			mvprintw(9,27+(6*pos_act),valori[pos_act]);
			// fflush(stdout);
			refresh();
			beep();
			usleep(80000);
		}
		// giri lenti e raggiungimento posizione
		for(i=0;i<6;i++) {
			color_set( COLORE_NUMERO_SPENTO ,NULL);
			mvprintw(9,27+(6*pos_act),valori[pos_act]);
			if((++pos_act)>=4) pos_act=0;
			color_set( COLORE_NUMERO_ACCESO ,NULL);
			mvprintw(9,27+(6*pos_act),valori[pos_act]);
			// fflush(stdout);
			refresh();
			beep();
			usleep(150000);
		}
		while(pos_act!=num) {
			color_set( COLORE_NUMERO_SPENTO ,NULL);
			mvprintw(9,27+(6*pos_act),valori[pos_act]);
			if((++pos_act)>=4) pos_act=0;
			color_set( COLORE_NUMERO_ACCESO ,NULL);
			mvprintw(9,27+(6*pos_act),valori[pos_act]);
			// fflush(stdout);
			refresh();
			usleep(150000);
		}

		pos=num;

		gp->i_punti_turno=(long) atoi(valori[pos]);

	}

	return pos;
}

#define COLORE_GIOCATORE_SPENTO 6
#define COLORE_GIOCATORE_ACCESO 48
/*
 *     funzione stato_giocatori
 */
int stato_giocatori( S_GAMEPROFILE * gp )
{
	char buffer[100];

	color_set( (gp->turno==0) ? COLORE_GIOCATORE_ACCESO:COLORE_GIOCATORE_SPENTO ,NULL);
	sprintf( buffer, "Giocatore 1 : %-12.12s", gp->az_giocatore[0] );
	mvprintw( 2, 1, buffer );
	sprintf( buffer, "Cash .......: %6ld      ", gp->al_credito[0] );
	mvprintw( 3, 1, buffer );

	color_set( (gp->turno==1) ? COLORE_GIOCATORE_ACCESO:COLORE_GIOCATORE_SPENTO ,NULL);
	sprintf( buffer, "Giocatore 2 : %-12.12s", gp->az_giocatore[1] );
	mvprintw( 2, 53, buffer );
	sprintf( buffer, "Cash .......: %6ld      ", gp->al_credito[1] );
	mvprintw( 3, 53, buffer );

	return 0;
}


#define COLORE_CONSONANTE_SPENTA 0
#define COLORE_CONSONANTE_ACCESA 31
/*
 *     funzione mostra_consonanti
 */
int mostra_consonanti( S_GAMEPROFILE * gp )
{
	register int i;
	const char kz_consonanti[]=CONSONANTI;

	color_set( 3 ,NULL);
	mvprintw( 20, 5, "Cons:" );

	for(i=0;i<NUM_CONSONANTI;i++) {
		color_set( (kz_consonanti[i]==gp->ac_consonanti_prese[i]) ? COLORE_CONSONANTE_ACCESA:COLORE_CONSONANTE_SPENTA ,NULL);
		mvprintw( 20, 11+i, "%c",toupper(kz_consonanti[i]) );
	}
	return 0;
}

#define COLORE_VOCALE_SPENTA 0
#define COLORE_VOCALE_ACCESA 31
/*
 *     funzione mostra_vocali
 */
int mostra_vocali( S_GAMEPROFILE * gp )
{
	register int i;
	const char kz_vocali[]=VOCALI;

	color_set( 3 ,NULL);
	mvprintw( 20, 56, "Vocali:" );

	for(i=0;i<NUM_VOCALI;i++) {
		color_set( (kz_vocali[i]==gp->ac_vocali_prese[i]) ? COLORE_VOCALE_ACCESA:COLORE_VOCALE_SPENTA ,NULL);
		mvprintw( 20, 64+i, "%c",toupper(kz_vocali[i]) );
	}
	return 0;
}

/*
 *     funzione quadro_parola
 */
int quadro_parola( S_GAMEPROFILE * gp )
{
	int pos_ini, len_parola;
	register int i;
	char c;

	len_parola=strlen(gp->z_parola);
	pos_ini=(80-(len_parola+2))/2;


	disegna_box( 12, pos_ini, 3, len_parola+2, 6, TRUE, 0, "Parola" );
	// gp->z_parola[0]='\0';
	for(i=0;i<len_parola;i++) {
			c=toupper(gp->ac_lettere_prese[i]);
			if(c==0) c='-';
		  mvprintw( 13, pos_ini+1+i, "%c",c );
	}

	return 0;
}

/*
 *     funzione scegli_consonante
 */
int scegli_consonante( S_GAMEPROFILE * gp )
{
	register int i;
	char c;
	char z_buf[3];
	char z_bid[100];
	int quante_prese=0;

	strcpy(z_buf,"");
	strcpy(z_bid,  CONSONANTI );

	do {
		color_set( 3 ,NULL);
		mvprintw(22,1, "                                                                 ");
		mvprintw(22,1, "Scegli consonante : " );
		inputcontr( 22,21, z_buf, 1, CONSONANTI, 'L', "", '_' );
		c=z_buf[0];

		/* se e' gia stata estratta, torno un errore e faccio ripetere */
		for(i=0;i<NUM_CONSONANTI;i++){
			if(gp->ac_consonanti_prese[i]==c) break;
		}

		if(i<NUM_CONSONANTI){
			color_set( 1 ,NULL);
			mvprintw(22,1, "Consonante '%c' gia' scelta -- Riprovare. ", toupper(c) );
			refresh();
			usleep(500000);
		}
		
	} while( i<NUM_CONSONANTI );

	/* "marchio" la consonante come gia' scelta */
	for(i=0;i<NUM_CONSONANTI;i++){
		if(gp->ac_consonanti_prese[i]==c) break;
	}
	gp->ac_consonanti_prese[strchr(z_bid,c)-z_bid]=c;

	/* Controllo quante ne ho prese nella parola, ed eventualmente le segno */
	for(i=0;i<MAX_LEN_PAROLA;i++){
		if(gp->z_parola[i]==c) {
			quante_prese++;
			gp->ac_lettere_prese[i]=c;
		}
	}

	/* Se non ho preso la consonante, il turno passa all'altro giocatore */
	if( !quante_prese ) {
		gp->turno++;
		if( gp->turno > 1 ) gp->turno=0;
	}
	else 
	{
		/* Aggiorna i crediti */
		gp->al_credito[gp->turno]+=gp->i_punti_turno * quante_prese;
		/* Rivisualizza lo stato dei giocatori */
		stato_giocatori( gp );
	}

	return 0;
}

/*
 *     funzione compra_vocale
 */
int compra_vocale( S_GAMEPROFILE * gp )
{
	int quante_prese=0;

	if( gp->al_credito[gp->turno] < (long) COSTO_VOCALE )
	{
			color_set( 1 ,NULL);
			mvprintw(22,1, "Credito insufficiente per comprare una vocale. ");
			refresh();
			usleep(500000);
	}
	else
		quante_prese=scegli_vocale( gp );

	return quante_prese;
}

/*
 *     funzione scegli_vocale
 */
int scegli_vocale( S_GAMEPROFILE * gp )
{
	register int i;
	char c;
	char z_buf[3];
	char z_bid[100];
	int quante_prese=0;

	strcpy(z_buf,"");
	strcpy(z_bid,  VOCALI );

	do {
		color_set( 3 ,NULL);
		mvprintw(22,1, "                                                                 ");
		mvprintw(22,1, "Scegli vocale : " );
		inputcontr( 22,21, z_buf, 1, VOCALI, 'L', "", '_' );
		c=z_buf[0];

		/* se e' gia stata estratta, torno un errore e faccio ripetere */
		for(i=0;i<NUM_VOCALI;i++){
			if(gp->ac_vocali_prese[i]==c) break;
		}

		if(i<NUM_VOCALI){
			color_set( 1 ,NULL);
			mvprintw(22,1, "Vocale  '%c' gia' scelta -- Riprovare. ", toupper(c) );
			refresh();
			usleep(500000);
		}
		
	} while( i<NUM_VOCALI );

	/* "marchio" la consonante come gia' scelta */
	for(i=0;i<NUM_VOCALI;i++){
		if(gp->ac_vocali_prese[i]==c) break;
	}
	gp->ac_vocali_prese[strchr(z_bid,c)-z_bid]=c;

	/* Controllo quante ne ho prese nella parola, ed eventualmente le segno */
	for(i=0;i<MAX_LEN_PAROLA;i++){
		if(gp->z_parola[i]==c) {
			quante_prese++;
			gp->ac_lettere_prese[i]=c;
		}
	}

	/* Aggiorna i crediti */
	gp->al_credito[gp->turno]-=(long) COSTO_VOCALE;
	/* Rivisualizza lo stato dei giocatori */
	stato_giocatori( gp );

	/* Se non ho preso la consonante, il turno passa all'altro giocatore */
	if( !quante_prese ) {
		gp->turno++;
		if( gp->turno > 1 ) gp->turno=0;
	}

	return quante_prese;
}


/*
 *     funzione analisi_gioco
 */
int analisi_gioco( S_GAMEPROFILE * gp )
{
	register int i;
	char c;
	int i_maxcons=0,i_maxvocs=0;
	int i_numcons=0,i_numvocs=0;

	/* conto le consonanti e le vocali nella parola */
	
	for(i=0;i<MAX_LEN_PAROLA;i++){
		c=tolower(gp->z_parola[i]);
		if(c<'a'|| c>'z') continue;
		if(c=='a'||c=='e'||c=='i'||c=='o'||c=='u')
			i_maxvocs++;
		else
			i_maxcons++;
	}

	/* conto le consonanti e le vocali prese */
	for(i=0;i<MAX_LEN_PAROLA;i++){
		c=tolower(gp->ac_lettere_prese[i]);
		if(c<'a'|| c>'z') continue;
		if(c=='a'||c=='e'||c=='i'||c=='o'||c=='u')
			i_numvocs++;
		else
			i_numcons++;
	}

	if( i_maxvocs == i_numvocs && i_maxcons == i_numcons )
		/* Finite le lettere quindi indovinata la parola */
		gp->winner=1;
	else {
		if ( i_numcons == i_maxcons ) {
		/* Finite le consonanti */
			gp->bit_azioni_permesse=AZ_INDOVINA;
			if( gp->al_credito[gp->turno] >= (long) COSTO_VOCALE )
			{
				msgalert( "Consonanti finite! Compra vocale (V) o (S)oluzione!");
				gp->bit_azioni_permesse|=AZ_VOCALE;
			} 
			else
				msgalert( "Consonanti finite! Credito insuff: solo (S)oluzione!");
		} else 
			if ( i_numvocs == i_maxvocs ) {
			/* Finite le vocali */
			msgalert( "Vocali finite! Gira ruota (G) o (S)oluzione!");
			gp->bit_azioni_permesse=AZ_INDOVINA|AZ_CONSONANTE;
		}
	}

	return 0;
}

/*
 *     funzione vittoria
 */
int vittoria( S_GAMEPROFILE * gp )
{
	WINDOW *wh;
	int c;

	color_set( 61 ,NULL);
	wh = newwin(12,60,6,10);
	box(wh,0,0);

	if( gp->winner == 1 ) {
		mvwprintw(wh,1,1,"Esatto !!!!!");
	} else {
		mvwprintw(wh,1,1,"Ahi ahi ahi! Hai sbagliato !!!!!");
	}

	mvwprintw(wh,3,1,"La parola esatta e': \"%s\"", gp->z_parola);

	color_set( 6 ,NULL);
	mvwprintw(wh,5,1,"Vince il giocatore %d (%s) con %ld punti!", gp->turno+1, gp->az_giocatore[gp->turno],gp->al_credito[gp->turno]);

	color_set( 61 ,NULL);

	mvwprintw(wh,11,1,"[premi un tasto per chiudere questa finestra]");


	wrefresh(wh);
	c=getch();
	wborder(wh, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(wh);
	delwin(wh);

	return 0;
}
