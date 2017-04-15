/*
 * This program is copyrighted to Gabriele Zappi (C)
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
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

int ges_fine()
{

  endwin();
  exit(0);
}

int main(int argc, char *argv[])
{
	WINDOW *win;
	S_GAMEPROFILE * gp;
	int i,j;
	int ret;

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

#ifdef DEBUG
	fprintf(stdout, "argv[0]= %s\n\r", argv[0] );
	fprintf(stdout, "Prgname = %s\n\r", Prgname );
	fflush(stdout);
#endif


  win=initscr();
	start_color();
  crmode();
  noecho();
  erase();
  refresh();

	/*
	 * Alloco memoria per la principale struttura di gioco.
	 */
	if ( (gp = malloc(sizeof(S_GAMEPROFILE *))) == NULL ){
		printf( "Errore: non posso allocare tanta memoria per la struttura di gioco\n\r" );
		exit(-1);
	}
	

	for(i=0;i<COLORS;i++)
		for(j=0;j<COLORS;j++)
			init_pair(i*COLORS+j,j, i);


	do {
		ret = gioca( gp );
	} while( ret!=9 );


	return(0);
	exit(0);
}



int gioca( S_GAMEPROFILE * gp ) 
{
	static u8 c=0;
	static long volta_entrato=0L;
	register int i,j;
	int ncolore;
	++volta_entrato;

	erase();
	refresh();

	move(0,0);
	// COLOR_PAIR(c1%(COLORS*COLORS));

	for(i=0;i<COLORS;i++)
		for(j=0;j<COLORS;j++) {
			ncolore=i*COLORS+j;
			move(j,i*8);
			color_set( ncolore ,NULL);
			printw("Col = %2d", ncolore );
		}

	color_set( 0 ,NULL);
	move(23,0);
	printw("Premere 'q' o <invio> per terminare... ");


	fflush(stdout);

	c=getch();

	if( c=='q' || c==10 || c==13 ) {
		return 9;
	}

	return EXIT_SUCCESS;
}

