#include <ncurses.h>

int main()
{
int c,i,ciclo=TRUE;
WINDOW *dialogo;

  initscr();
  curs_set(0);
  noecho();

  timeout(500);

  while(ciclo) {
    for (i=1; i<10 ; i++)
    {
      mvprintw(i,i,"Salve! Questo e' un programma inutile! ");
      refresh();
      c=getch();
      if (c!=ERR)
      {
	break;
      }
      deleteln(); 
    }   

    if (c!=ERR)
    {
      dialogo= newwin (7,32,7,17);
      box(dialogo,'*','*');
      timeout(0);
      mvwprintw(dialogo,2,3,"Vuoi davvero interrompere?");
      mvwprintw(dialogo,4,12,"(s)icuro?");
      c=wgetch(dialogo);
      if (c=='s')
      {
	ciclo=FALSE;
      }
      delwin(dialogo);
      timeout(500);
    };

    erase(); 
    refresh();
  };

  endwin();
  return 0;
}
