#include "server.h"
#include <cstring>
#include <functional>
#include <iostream>
#include <ncurses.h>
#include <string>

WINDOW *create_newwin(int height, int width, int starty, int startx) {
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0); /* 0, 0 gives default characters
                         * for the vertical and horizontal
                         * lines			*/
  wrefresh(local_win);  /* Show that box 		*/

  return local_win;
}
void destroy_win(WINDOW *local_win) {

  wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(local_win);
  delwin(local_win);
}
void makeWindow() {
  WINDOW *my_win;
  int startx, starty, width, height;
  int ch;
  height = 10;
  width = 30;
  starty = (LINES - height) / 2; /* Calculating for a center placement */
  startx = (COLS - width) / 2;   /* of the window		*/

  refresh();
  my_win = create_newwin(height, width, starty, startx);
  refresh();
  const char *message1 = "Welcome to Server";
  mvprintw(starty + 1, startx + strlen(message1) / 2, message1);
  const char *message2 = "Press a key to continue";
  mvprintw(starty + 2, startx + strlen(message2) / 6, message2);
  getch();
  destroy_win(my_win);
}

// MAIN WINDOW
void catchKeyboard(int startx, int starty, std::string &sendMessage,
                   bool &message) {
  int letter;

  letter = getch();
  if (letter == 10 || letter == KEY_ENTER)
    message = false;
  else if (letter == 127 || letter == KEY_BACKSPACE) {
    if (!sendMessage.empty())
      sendMessage.pop_back();
  } else if (letter != ERR)
    sendMessage.push_back(letter);
  clrtoeol();
  mvprintw(starty, startx + 2, sendMessage.c_str());
  refresh();
}
void makeMainWindow() {
  WINDOW *new_win;
  WINDOW *under_win;
  int startx, starty, width, height;
  int ch;
  height = LINES - 5;
  width = COLS - 10;
  starty = 5; /* Calculating for a center placement */
  std::string sendMessage = ",";
  startx = 10; /* of the window		*/
  new_win = create_newwin(height, width, starty, startx);
  refresh();
  under_win = create_newwin(5, width, LINES - 5, startx);
  refresh();
  bool writeMessage{true};
  while (writeMessage) {

    catchKeyboard(startx, LINES - 3, sendMessage, writeMessage);
    refresh();
  }

  destroy_win(under_win);
  destroy_win(new_win);
}

void GUI::guiFunc() {
  initscr();
  noecho();
  makeWindow();
  makeMainWindow();

  endwin();
}
