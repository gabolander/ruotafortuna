#include <ncurses.h>

int main()
{
  initscr();
  start_color();
  init_pair(1,COLOR_RED,COLOR_BLACK);
  init_pair(2,COLOR_WHITE,COLOR_BLUE);

  printw("Normale\n");
  attron(COLOR_PAIR(1));
  printw("Rosso su nero\n");
  attron(COLOR_PAIR(2));
  printw("Bianco su blu");

  refresh();
  getch();

  printw("Cambio il colore a tutto!");
  bkgd(COLOR_PAIR(2));

  refresh();
  getch();
  endwin();
  return 0;
}
