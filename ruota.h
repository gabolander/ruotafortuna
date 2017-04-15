/* defines e macrodefines */
#define PROGRAMNAME "Ruota della Fortuna"
#define VERSIONE "0.0.60"
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
#define FILE_DIZIONARIO "dizionario.lst"
#define MAXRIGA 1024

#ifndef SI
#define SI 1
#endif
#ifndef NO
#define NO 0
#endif

#ifndef ERRORE
#define ERRORE  (-1)
#endif
#ifndef NO
#define NO        0
#endif
#ifndef SI
#define SI        1
#endif
#ifndef IGNORA
#define IGNORA    2
#endif
#ifndef DEFAULT
#define DEFAULT   3
#endif
#ifndef NOBUFF
#define NOBUFF    4
#endif
#ifndef _OK
#define _OK       0
#endif

#ifndef NULL
#define NULL 0x00
#endif

#ifndef strlower
#define strlower strlow
#endif

#define strvuota(str)             (str==NULL ? SI : strnvuota(str,strlen(str)))


#define ALFABETO "abcdefghijklmnopqrstuvwxyz"
#define CONSONANTI "bcdfghjklmnpqrstvwxyz"
#define VOCALI "aeiou"
#define STD_INPUT_CURSOR '_'

#define MAX_LEN_PAROLA 60
#define MAX_LEN_PLAYER 50
#define MAX_NUM_PLAYER 2
#define NUM_CONSONANTI 21
#define NUM_VOCALI 5 

#define LINEA_COMUNICA   22
#define LINEA_STATUSLINE 24

#define COSTO_VOCALE 1000														 

/* define azioni */
#define AZ_INDOVINA 1
#define AZ_CONSONANTE 2
#define AZ_VOCALE 4

/* typedefs */
typedef unsigned char u8;
typedef unsigned char BOOL;
typedef struct tag_gameprofile {
		char az_giocatore[MAX_NUM_PLAYER][MAX_LEN_PLAYER+1];
		long al_credito[MAX_NUM_PLAYER];
		char z_parola[MAX_LEN_PAROLA+1];
		char ac_lettere_prese[MAX_LEN_PAROLA];
		char ac_consonanti_prese[NUM_CONSONANTI];
		char ac_vocali_prese[NUM_VOCALI];
		int  i_punti_turno;
		u8 turno;
		BOOL winner;
		long l_passaggio;
		unsigned char m_filler[76];
		int bit_azioni_permesse;
} S_GAMEPROFILE;


