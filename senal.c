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
#include "strfunz.c"

#ifndef strlower
#define strlower strlow
#endif

#define MAX_BUFFER_SIZE (1024*100)
#define MAX(a,b)	(a>b)?(a):(b)
#define MIN(a,b)	(a<b)?(a):(b)
#define DEFAULT_MEM_SIZE 50000000L
#define MAX_ELEM 6
#define SECONDI(a)  (a/CLOCKS_PER_SEC)

#define TRUE 1
#define true 1
#define YES 1
#define SI 1
#define FALSE 0
#define false 0
#define NO 0
#define MAXVETT 30000

/* typedefs */
typedef unsigned char u8;

/* prototipi di funzione */
int tira_numero_random( int );
int delete_element( int *, int, int );
int ordina_vettore_intero(int []);

int main(int argc, char *argv[])
{
	register int count;
	register int i, old_rep=-1;
	int irnd;
	int inum[30000], onum[] = { 0, 0, 0, 0, 0, 0 };
	int itank[30000], isenal[10];
	unsigned int rand_seed;
	clock_t clk;
	int elems, max_times;
	int rep_this=0;
	int repetitions=0;
	int tot=0;
	struct tms *tempo;
	u8 b_params=false;

	elems=90;

#if 0
	// if argc == 1, no parameters
	printf("argc = %d\n",argc);
#endif
	if( argc > 1 )
		b_params=true;

	if( elems == 0 ) 
		elems = MAXVETT;

	/* clk = clock(); */
	tempo = malloc( sizeof(struct tms) );

	if( !b_params )
	{
		/* visualizza elementi */
		printf( "\n Dati estrazione ..... : \n\n" );
		for( i=0; i < elems; i++ )
		{
			printf( "%3d) %2d ", i+1, inum[i] );
		}
		putchar('\n'); putchar('\n');
		max_times = 1;
	}
	else
		max_times = atoi( argv[1] );
	
	if( b_params )
		max_times++;

	rep_this=0;
	old_rep=-1;
	for( count = 0; count<max_times; count++) /* main loop */
	{
		/* assegno vettore bidone */
		for( i=0; i < elems; i++ )
			itank[i]=i+1;

		clk = times( tempo );

		rand_seed = (unsigned int)((long)clk % 32768);
		srand( rand_seed );

		for( i=0; i < elems; i++ )
		{
			irnd = tira_numero_random( elems - i ); /* su elems, elems - 1, ... */
			inum[i] = itank[irnd-1];
			delete_element( itank, irnd - 1, elems - i );
		}

		/* trasferisco i primi sei numeri per ordinarli */
		for( i=0; i < 6; i++ )
			isenal[i] = inum[i];
		ordina_vettore_intero( isenal );

		/* check: if first six numbers are equal to last time's, then relaunch */
		for( i=0; i < 6 && onum[i]==isenal[i] ; i++ ) ;
		if(i == 6) /* all new numbers are equal to old ones */
		{
			count--;
			if( old_rep != count )
			{
				old_rep = count;
				rep_this=0;
			}
			repetitions++;
			rep_this++;
			continue;
		}
		else
		{
			if( rep_this )
			{
				printf( " ### sequenza ripetuta %d volte!! \n", rep_this );
				tot+=rep_this;
			}
			else
				putchar( '\n' );
		}

		if( b_params && count == (max_times - 1) )
			break; // Controlla ripetizione anche su ultimo lancio
		
		if( !b_params )
			printf( "\nI primi 6 (sei) numeri sono i seguenti : \n\n" );
		else
			putchar( '\n' );
		for( i=0; i < 5; i++ )
		{
			printf( " %2d -", isenal[i] );
		}
		printf( " %2d ", isenal[5] );

		for( i=0; i < 6; i++ )
			onum[i] = isenal[i];
	}
	putchar( '\n' );

	if( repetitions )
			printf( "\n# Si sono verificate %d ripetizioni. \n", repetitions );
		/*
			printf( "\nSi sono verificate %d ripetizioni. (totale %d)\n", repetitions, tot );
			*/

	free( tempo );
	return(0);
	exit(0);
}

int tira_numero_random( int finoa )
{
	int j;

	j=1+(int) ((float)finoa*rand()/(RAND_MAX+1.0));

	return( j );
}

/* Togliamo un elemento da un vettore di interi statico */
/* pos = nesimo elemento da cancellare **
** elems = numero elementi totali del vettore */
int delete_element( ivett, pos, elems )
int *ivett, pos, elems; 
{
	register int y;

	if( pos == (elems-1) )
	{
		ivett[pos]=0;
		return(0);
	}

	for( y=pos; y<(elems-1); y++ )
		ivett[y]= ivett[y+1];
	ivett[y]=0; /* pulisco ultimo elemento */

	return(0);
}

/* Algoritmo di ordinamento crescente di un vettore "per inserimento" */
int ordina_vettore_intero(vettnum)
int vettnum[];
{
	int 		iIndiceA;
	int 		iIndiceB;
	int	numappo;

	for(iIndiceA = 1; iIndiceA < MAX_ELEM ; iIndiceA++)
	{
		numappo = vettnum[iIndiceA];

		for(iIndiceB = iIndiceA - 1
			; (iIndiceB >= 0) && (numappo < vettnum[iIndiceB])
			; iIndiceB--)
		{	
			vettnum[iIndiceB+1] = vettnum[iIndiceB];
		}	
		vettnum[iIndiceB + 1] = numappo;
	}
	
	/*
	** giro di controllo che determina quanti elementi esistono nel vettore
	** con valore > 0 
	iIndiceA = 0;
	while( vettnum[iIndiceA] > 0.0 ) iIndiceA++;
	return( iIndiceA );
	*/

	return( 0 );
}
