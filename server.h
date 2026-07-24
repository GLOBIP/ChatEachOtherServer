#pragma once
#include <string>
class Server {

public:
  void closeNetwork(int clSock);
  std::string readValue(int socket);
  void sendValue(int socket, const char *message);
  int clientSocket;
  int initilizeNetwork();
};

class files {
public:
  void sendFileServer(std::string message);
  void sendFileClient(std::string message);
  void readFile();
};

class windowFileRelated {
public:
  void putReadIntoScreen(std::string message, int left, int right, int &starty);
  void drawOnScreen(int startx, int starty, const char *msg);
};
class GUI {
  files myfiles;
  windowFileRelated myWindow;

public:
  void guiFunc(Server &NetworkStuff, int client);
};
