
#include "ncurses.h"
#include "server.h"
#include <cstring>
#include <fstream>
#include <unistd.h>
void checkRead(char &Who, std::string zdanie) {
  if (zdanie == "Client")
    Who = 'C';
  else if (zdanie == "Server")
    Who = 'S';
}

void windowFileRelated::putReadIntoScreen(std::string message, int &starty,
                                          WindowParts *MyParts) {

  std::ifstream plikOtworz("comm.txt", std::ios::in);
  int y{};
  std::string zdanie = "";
  char WhosMessage{};
  bool isSecondTime{false};
  while (std::getline(plikOtworz, zdanie)) {
    checkRead(WhosMessage, zdanie);
    bool clientServerOr = (zdanie == "Client" || zdanie == "Server");
    if (clientServerOr) {
      isSecondTime = true;
      continue;
    }
    if (isSecondTime) {
      isSecondTime = false;
      continue;
    }

    if (WhosMessage == 'C')
      drawOnScreen(MyParts->left, MyParts, starty, zdanie.c_str());
    else if (WhosMessage == 'S')

      drawOnScreen(MyParts->right, MyParts, starty, zdanie.c_str());
    starty -= 1;
  }
  refresh();
}
void windowFileRelated::drawOnScreen(int side, WindowParts *MyParts, int starty,
                                     const char *msg) {

  int right = MyParts->startx + MyParts->width;
  /*
if (side + strlen(msg) >= right) {
  int times = 1;
  for (int i{}; i <= times; i++)
    side -= 4;
}*/
  for (;;) {
    if (side + strlen(msg) >= right) {
      side--;
    } else {
      break;
    }
  }

  move(starty, side);
  refresh();
  printw(msg);
  refresh();
}
