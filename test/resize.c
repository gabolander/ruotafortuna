/*
 * This program is copyrighted to Gabriele Zappi (C)
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <ncurses.h>
#include <ctype.h>

#ifdef __sun__
#include <sys/termio.h>
#include <sys/filio.h>
#endif

#ifdef __CYGWIN__
#include <sys/termios.h>
#include <sys/socket.h>
#endif
/* #include "strfunz.c" */

#if 0
#if defined(SIGWINCH) && defined(TIOCGWINSZ) && defined(NCURSES_VERSION)
#define CAN_RESIZE 1
#else
#define CAN_RESIZE 0
#endif
#endif
#define CAN_RESIZE 1

#if CAN_RESIZE
static int adjust(int sig);
static int interrupted;
#endif

int maschera_base(void);
int _premi_un_tasto(char * );

void (*kt_resize_event)(void);

void kreinit(int sn) {
    struct winsize size;

    if(ioctl(fileno(stdout), TIOCGWINSZ, &size) == 0) {
	resizeterm(size.ws_row, size.ws_col);
	wrefresh(curscr);

	if(kt_resize_event)
	    kt_resize_event();
    }

    signal(sn, &kreinit);
}


int _premi_un_tasto(char * messaggio)
{
  char c;

  fprintf(stdout,(messaggio[0]==0) ? "Premere un tasto per Continuare.\n\r" : messaggio );
  fflush(stdout);
  c=getch();

  return c;
}


int ges_fine()
{
  _premi_un_tasto("Premere un tasto per TERMINARE.\n\r");

  endwin();
  exit(0);
}

int maschera_base()
{
	clear();
	/* traccio due righe */
  color_set( 1 ,NULL);
  move(1,0);  hline( ACS_HLINE, COLS );
  move(LINES-2,0); hline( ACS_HLINE, COLS );
	mvprintw(0,(COLS-11)/2,"Prima linea");
	mvprintw((LINES-1),(COLS-12)/2,"Ultima linea");

	refresh();
}


int main(int argc, const char *const *argv) {
	register int i,j;

#if CAN_RESIZE
  // (void) signal(SIGWINCH, adjust); /* arrange interrupts to resize */
    signal(SIGWINCH, &kreinit);
    kt_resize_event = 0;
#endif
  atexit( (void *) ges_fine ); // al return o exit del main()
  signal(SIGINT, (void *) ges_fine); // in caso di interruzione
  signal(SIGTERM, (void *) ges_fine); // in caso di interruzione
  signal(SIGKILL, (void *) ges_fine); // in caso di interruzione
  signal(SIGQUIT, (void *) ges_fine); // in caso di interruzione

  initscr();
  start_color();
  crmode();
  noecho();
  curs_set(0);
  keypad(stdscr,TRUE);
  refresh();

  for(i=0;i<COLORS;i++)
    for(j=0;j<COLORS;j++)
      init_pair(i*COLORS+j,j, i);


	maschera_base();
	move(LINES-6,0);
 
  _premi_un_tasto("");

	return(0);
	exit(0);
}
