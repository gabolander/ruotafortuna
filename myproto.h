/* prototipi di funzione */
#include "strfunz.h"
int tira_numero_random( int );
int ordina_vettore_intero(int []);
int strpulall(char *);
int pul(char *);
int puldim( char *, int );

int init_game_struct( S_GAMEPROFILE * );
int imposta_match( S_GAMEPROFILE * );
int gioca( S_GAMEPROFILE * );
int maschera_base( S_GAMEPROFILE * );
int headline( S_GAMEPROFILE * );
int statusline( S_GAMEPROFILE * );
int dai_soluzione( S_GAMEPROFILE * );
int _premi_un_tasto( char * );
int _debug_struct( S_GAMEPROFILE * );
int inputcontr( int, int, char *, int, 
								const char *, const unsigned char,
								const char *, const char );
void aiuto(void);
void msgalert(char *);
void msgerror(char *);
int disegna_box( int , int , int , int , int , bool , int, const char * );
int ruota( S_GAMEPROFILE *, bool );
int stato_giocatori( S_GAMEPROFILE * );
int mostra_consonanti( S_GAMEPROFILE * );
int mostra_vocali( S_GAMEPROFILE * );
int quadro_parola( S_GAMEPROFILE * );
int scegli_consonante( S_GAMEPROFILE * );
int compra_vocale( S_GAMEPROFILE * );
int scegli_vocale( S_GAMEPROFILE * );
int analisi_gioco( S_GAMEPROFILE * );
int vittoria( S_GAMEPROFILE * );
