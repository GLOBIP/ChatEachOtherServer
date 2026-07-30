#include "server.h"
#include <cstring>
#include <functional>
#include <iostream>
#include <ncurses.h>
#include <string>

void makeWindow(windowShowProgra &myWindowProgram, Server &NetworkStuff,
                int &socket) {
  struct FirstIpWindow {
    int height = 10;
    int width = 30;
    int starty = (LINES - height) / 2; /* Calculating for a center placement */
    int startx = (COLS - width) / 2;   /* of the window		*/
    const char *message1 = "Welcome to Server";
    const char *message2 = "Give Port";
  };
  WINDOW *my_win;
  FirstIpWindow VarsWindow;
  refresh();
  init_pair(1, COLOR_BLACK, COLOR_CYAN);
  my_win = myWindowProgram.create_newwin(VarsWindow.height, VarsWindow.width,
                                         VarsWindow.starty, VarsWindow.startx);
  wbkgd(my_win, COLOR_PAIR(1));
  refresh();

  mvwprintw(my_win, 1, 7, VarsWindow.message1);
  mvwprintw(my_win, 2, 11, VarsWindow.message2);
  wrefresh(my_win);
  std::string port = NetworkStuff.catchInput(my_win);
  socket = NetworkStuff.initilizeNetwork(std::stoi(port));
  myWindowProgram.destroy_win(my_win);
}

// MAIN WINDOW
void putText(windowShowProgra &myWindowProgram, WindowParts *windowVariables,
             int &height, std::string message, WINDOW *&new_win,
             windowFileRelated &WindowRelated, int &bottomHeightChat,
             windowShowProgra &CreateDestroyWin) {
  init_pair(3, COLOR_BLACK, COLOR_BLUE);
  new_win = myWindowProgram.create_newwin(
      windowVariables->upper_window_height, windowVariables->width,
      windowVariables->starty, windowVariables->startx);
  wbkgd(new_win, COLOR_PAIR(3));
  WindowRelated.putReadIntoScreen(message, height, windowVariables, new_win,
                                  bottomHeightChat, CreateDestroyWin);
  wrefresh(new_win);
}
void writeRecievedData(std::string message1, files &myfiles,
                       windowShowProgra &myWindowProgram, WINDOW *&new_win) {
  myfiles.sendFileServer(message1);
  myWindowProgram.destroy_win(new_win);
}
void makeMainWindow(Server &NetworkStuff, windowFileRelated &WindowRelated,
                    int client, files &myfiles,
                    windowShowProgra &myWindowProgram) {
  WINDOW *new_win;
  WINDOW *under_win;
  WindowParts windowVariables;
  WindowParts *cptr = &windowVariables;
  int writeHeigth = LINES - 6;
  short writeMessage{3};
  std::string sendMessage = "";
  curs_set(0);
  int textHeight = windowVariables.height - 15;
  int copytextHeight = textHeight;
  int bottomHeightChat{};
  putText(myWindowProgram, cptr, textHeight, sendMessage, new_win,
          WindowRelated, bottomHeightChat, myWindowProgram);

  refresh();
  under_win = myWindowProgram.create_newwin(
      windowVariables.under_window_height, windowVariables.width,
      windowVariables.height, windowVariables.startx);
  init_pair(4, COLOR_BLACK, COLOR_CYAN);
  wbkgd(under_win, COLOR_PAIR(4));
  nodelay(new_win, true);
  nodelay(under_win, true);
  nodelay(stdscr, true);

  keypad(under_win, TRUE);
  wrefresh(new_win);
  while (writeMessage) {
    std::string message1 = NetworkStuff.readValue(client);
    if (!message1.empty()) {
      writeRecievedData(message1, myfiles, myWindowProgram, new_win);
      putText(myWindowProgram, cptr, textHeight, sendMessage, new_win,
              WindowRelated, bottomHeightChat, myWindowProgram);
    }
    myWindowProgram.catchKeyboard(windowVariables, sendMessage, writeMessage,
                                  under_win, textHeight, bottomHeightChat,
                                  copytextHeight);
    if (writeMessage == 2) {
      NetworkStuff.sendValue(
          client, sendMessage.c_str());    // send text to be written by client
      myfiles.sendFileClient(sendMessage); // writes to this chat file

      myWindowProgram.destroy_win(new_win); // show chat protocol
      putText(myWindowProgram, cptr, textHeight, sendMessage, new_win,
              WindowRelated, bottomHeightChat, myWindowProgram);
      writeMessage = 3;

      sendMessage = "";
      // end of protocol
    } else if (writeMessage == 4) {
      myWindowProgram.destroy_win(new_win); // show chat protocol
      putText(myWindowProgram, cptr, textHeight, sendMessage, new_win,
              WindowRelated, bottomHeightChat, myWindowProgram);
      writeMessage = 1;
    }
    wclrtoeol(under_win);
    wrefresh(under_win);
  }
  getch();

  myWindowProgram.destroy_win(under_win);
  myWindowProgram.destroy_win(new_win);
}

void GUI::guiFunc(Server &NetworkStuff) {

  initscr();
  start_color();
  init_pair(2, COLOR_BLACK, COLOR_BLUE);
  bkgd(COLOR_PAIR(2));
  keypad(stdscr, TRUE);
  noecho();
  cbreak();
  int socket{};
  makeWindow(myWindowProgram, NetworkStuff, socket);
  makeMainWindow(NetworkStuff, myWindow, socket, myfiles, myWindowProgram);

  endwin();
  NetworkStuff.closeNetwork(socket);
}
