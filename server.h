#pragma once
#include "ncurses.h"
#include <string>
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
  void drawOnScreen(int startx, int starty, const char *msg);
  void destroy_win(WINDOW *local_win);
  void catchKeyboard(int startx, int starty, std::string &sendMessage,
                     short &message);
};
class files {
public:
  void sendFileServer(std::string message);
  void sendFileClient(std::string message);
  void readFile();
};

class windowFileRelated {
public:
  void putReadIntoScreen(std::string message, int &starty,
                         WindowParts *MyParts);
  void drawOnScreen(int side, WindowParts *MyParts, int &starty,
                    const char *msg);
};
struct WindowParts {

  int height = LINES - 5;
  int width = COLS - 10;
  int starty = 5;
  int startx = 10;
  int left = startx + 2;
  int right = width - 4;
};
class GUI {

  windowShowProgra myWindowProgram;
  files myfiles;
  windowFileRelated myWindow;

public:
  void guiFunc(Server &NetworkStuff, int client);
};
