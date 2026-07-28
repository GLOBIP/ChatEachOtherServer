#include "ncurses.h"
#include "server.h"

void windowShowProgra::destroy_win(WINDOW *local_win) {

  wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(local_win);
  delwin(local_win);
}
void windowShowProgra::catchKeyboard(WindowParts under_winParts,
                                     std::string &sendMessage, short &message,
                                     WINDOW *under_win, int &textHeight,
                                     int bottomHeightChat, int copytextHeight) {
  int letter;

  letter = getch();

  if (letter == 10 || letter == KEY_ENTER) {
    message = 2;
    return;
  } else if (letter == 127 || letter == KEY_BACKSPACE) {
    if (!sendMessage.empty())
      sendMessage.pop_back();
  } else if (letter == KEY_UP) {
    textHeight++;
    message = 4;
  } else if (letter == KEY_DOWN) {
    textHeight--;
    message = 4;
  } else if (letter == KEY_RIGHT || message == 3) {
    textHeight = bottomHeightChat;
    message = 4;
  } else if (letter == KEY_LEFT) {
    textHeight = copytextHeight;
    message = 4;
  }

  else if (letter == '|')
    message = 0;
  else if (letter >= 32 && letter <= 127) {
    sendMessage.push_back(letter);
  }
  clrtoeol();
  wmove(under_win, under_winParts.under_window_height / 2, 1);
  mvwprintw(under_win, under_winParts.under_window_height / 2, 1, "%s",
            sendMessage.c_str());
  wrefresh(under_win);
}
WINDOW *windowShowProgra::create_newwin(int height, int width, int starty,
                                        int startx) {
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0); /* 0, 0 gives default characters
                         * for the vertical and horizontal
                         * lines			*/
  wrefresh(local_win);  /* Show that box 		*/

  return local_win;
}
