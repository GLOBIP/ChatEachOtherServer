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

class GUI {
public:
  void guiFunc(Server &NetworkStuff, int client);
};
