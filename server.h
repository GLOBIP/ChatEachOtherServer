#pragma once
#include "ncurses.h"
#include <string>
#include <vector>
struct WindowParts;
class Server {

public:
  void closeNetwork(int clSock);
  std::string readValue(int socket);
  void sendValue(int socket, const char *message);
  int clientSocket;
  int initilizeNetwork();
};

class windowShowProgra {
public:
  std::vector<WINDOW *> local_winVec = {};
  void destroy_win(WINDOW *local_win);
  void catchKeyboard(WindowParts under_winParts, std::string &sendMessage,
                     short &message, WINDOW *under_win, int &textHeight,
                     int bottomHeightChat, int copytextHeight);
  WINDOW *create_newwin(int height, int width, int starty, int startx);
};
class files {
public:
  void sendFileServer(std::string message);
  void sendFileClient(std::string message);
  void readFile();
};

class windowFileRelated {
public:
  void putReadIntoScreen(std::string message, int starty, WindowParts *MyParts,
                         WINDOW *new_win, int &bottomheightChat,
                         windowShowProgra &CreateDestroyWin);
  void drawOnScreen(int side, WindowParts *MyParts, int &starty,
                    const char *msg, WINDOW *new_win,
                    windowShowProgra &CreateDestroyWin);
};
struct WindowParts {

  int height = LINES - 5;
  int width = COLS - 10;
  int starty = 5;
  int startx = 10;
  int left = startx + 2;
  int right = width - 4;
  int under_window_height = 5;
  int upper_window_height = height - 5;
};
class GUI {

  windowShowProgra myWindowProgram;
  files myfiles;
  windowFileRelated myWindow;

public:
  void guiFunc(Server &NetworkStuff, int client);
};
