#include "ncurses.h"
#include "server.h"

void windowShowProgra::drawOnScreen(int startx, int starty, const char *msg) {
  move(starty, startx);
  refresh();
  printw(msg);
  refresh();
}
void windowShowProgra::destroy_win(WINDOW *local_win) {

  wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(local_win);
  delwin(local_win);
}
void windowShowProgra::catchKeyboard(WindowParts under_winParts,
                                     std::string &sendMessage, short &message,
                                     WINDOW *under_win) {
  int letter;

  letter = getch();
  if (letter == 10 || letter == KEY_ENTER) {
    message = 2;
    return;
  } else if (letter == 127 || letter == KEY_BACKSPACE) {
    if (!sendMessage.empty())
      sendMessage.pop_back();
  } else if (letter != ERR)
    sendMessage.push_back(letter);
  clrtoeol();
  wmove(under_win, under_winParts.under_window_height / 2, 1);
  mvwprintw(under_win, under_winParts.under_window_height / 2, 1,
            sendMessage.c_str());
  wrefresh(under_win);
}
