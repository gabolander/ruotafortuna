#include <ncurses.h>

int main()
{
  initscr();
  mvprintw(0, 0,"Nord-ovest");
  mvprintw(LINES-1, 0,"Sud-ovest");
  mvprintw(0, COLS-8,"Nord-est");
  mvprintw(LINES-1, COLS-7,"Sud-est");

  refresh();
  getch();
  endwin();
  return 0;
}
