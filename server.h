#pragma once
class Server {

public:
  int clientSocket;
  int initilizeNetwork();
  void closeNetwork();
};

class GUI {
public:
  void guiFunc();
};
