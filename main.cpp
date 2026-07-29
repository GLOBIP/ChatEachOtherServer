#include "server.h"
#include <iostream>
#include <unistd.h>

int main() {
  GUI MyServerProgram;
  Server Internet;
  MyServerProgram.guiFunc(Internet);
  return 0;
}
