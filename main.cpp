#include "server.h"
#include <iostream>
#include <unistd.h>

int main() {
  GUI MyServerProgram;
  Server Internet;
  int socket = Internet.initilizeNetwork();
  MyServerProgram.guiFunc(Internet, socket);
  Internet.closeNetwork(socket);
  return 0;
}
