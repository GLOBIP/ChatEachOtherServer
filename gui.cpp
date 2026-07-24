#include "server.h"
#include <cstring>
#include <functional>
#include <iostream>
#include <ncurses.h>
#include <string>

struct BasicWindowVars {
  int height = 10;
  int width = 30;
  int starty = (LINES - height) / 2; /* Calculating for a center placement */
  int startx = (COLS - width) / 2;   /* of the window		*/
  const char *message1 = "Welcome to Server";
  const char *message2 = "Press a key to continue";
};
WINDOW *create_newwin(int height, int width, int starty, int startx) {
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0); /* 0, 0 gives default characters
                         * for the vertical and horizontal
                         * lines			*/
  wrefresh(local_win);  /* Show that box 		*/

  return local_win;
}

void makeWindow(windowShowProgra &myWindowProgram) {

  WINDOW *my_win;

  BasicWindowVars VarsWindow;

  refresh();
  my_win = create_newwin(VarsWindow.height, VarsWindow.width, VarsWindow.starty,
                         VarsWindow.startx);
  refresh();
  mvprintw(VarsWindow.starty + 1,
           VarsWindow.startx + strlen(VarsWindow.message1) / 2,
           VarsWindow.message1);
  mvprintw(VarsWindow.starty + 2,
           VarsWindow.startx + strlen(VarsWindow.message2) / 6,
           VarsWindow.message2);
  getch();
  myWindowProgram.destroy_win(my_win);
}

// MAIN WINDOW

void makeMainWindow(Server &NetworkStuff, windowFileRelated &WindowRelated,
                    int client, files &myfiles,
                    windowShowProgra &myWindowProgram) {
  WINDOW *new_win;
  WINDOW *under_win;
  WindowParts windowVariables;
  WindowParts *cptr = &windowVariables;
  int writeHeigth = LINES - 6;
  short writeMessage{1};
  std::string sendMessage = "";
  new_win = create_newwin(windowVariables.height, windowVariables.width,
                          windowVariables.starty, windowVariables.startx);
  refresh();
  under_win = create_newwin(5, windowVariables.width, windowVariables.height,
                            windowVariables.startx);
  refresh();
  while (writeMessage) {

    myWindowProgram.catchKeyboard(windowVariables.startx, LINES - 3,
                                  sendMessage, writeMessage);
    if (writeMessage == 2) {
      NetworkStuff.sendValue(client, sendMessage.c_str());
      myfiles.sendFileClient(sendMessage);
      WindowRelated.putReadIntoScreen(sendMessage, writeHeigth, cptr);
      sendMessage = "";
      writeMessage = 1;
    }
    refresh();
  }
  getch();

  myWindowProgram.destroy_win(under_win);
  myWindowProgram.destroy_win(new_win);
}

void GUI::guiFunc(Server &NetworkStuff, int client) {

  initscr();
  noecho();
  makeWindow(myWindowProgram);
  makeMainWindow(NetworkStuff, myWindow, client, myfiles, myWindowProgram);

  endwin();
}
