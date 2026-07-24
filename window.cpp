
#include "ncurses.h"
#include "server.h"
#include <fstream>
void checkRead(char &Who, std::string zdanie) {
  if (zdanie == "Client")
    Who = 'C';
  else if (zdanie == "Server")
    Who = 'S';
}
void windowFileRelated::putReadIntoScreen(std::string message, int left,
                                          int right, int &starty) {

  std::ifstream plikOtworz("comm.txt", std::ios::in);
  int y{};
  std::string zdanie = "";
  char WhosMessage{};
  while (std::getline(plikOtworz, zdanie)) {
    checkRead(WhosMessage, zdanie);

    if (WhosMessage == 'C')
      drawOnScreen(left, starty, zdanie.c_str());
    else if (WhosMessage == 'S')

      drawOnScreen(right, starty, zdanie.c_str());
    starty -= 1;
  }
  refresh();
}
void windowFileRelated::drawOnScreen(int startx, int starty, const char *msg) {
  move(starty, startx);
  refresh();
  printw(msg);
  refresh();
}
