
#include "ncurses.h"
#include "server.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>
void checkRead(char &Who, std::string zdanie) {
  if (zdanie == "Client")
    Who = 'C';
  else if (zdanie == "Server")
    Who = 'S';
}

void windowFileRelated::putReadIntoScreen(std::string message, int height,
                                          WindowParts *MyParts,
                                          WINDOW *new_win) {

  std::ifstream plikOtworz("comm.txt", std::ios::in);
  int y{};
  std::string zdanie = "";
  char WhosMessage{};
  bool isSecondTime{false};
  while (std::getline(plikOtworz, zdanie)) {
    checkRead(WhosMessage, zdanie);
    bool clientServerOr = (zdanie == "Client" || zdanie == "Server");
    /*
if (clientServerOr) {
  isSecondTime = true;
  continue;
}
if (isSecondTime) {
  isSecondTime = false;
  continue;
}*/

    if (WhosMessage == 'C')
      drawOnScreen(MyParts->left, MyParts, height, zdanie.c_str(), new_win);
    else if (WhosMessage == 'S')

      drawOnScreen(MyParts->right, MyParts, height, zdanie.c_str(), new_win);
    height += 1;
  }
  wrefresh(new_win);
}
void windowFileRelated::drawOnScreen(int side, WindowParts *MyParts,
                                     int &height, const char *msg,
                                     WINDOW *new_win) {

  std::string tekst = msg;
  int primarySide = side;
  std::string tekst2 = "";
  std::vector<std::string> tesktWriteVec = {};
  std::vector<int> sideVec = {};
  std::vector<int> heightVec = {};
  int right = MyParts->width;
  int middlescr = MyParts->width / 2;
  for (;;) {
    /*
while (height + 2 > MyParts->upper_window_height) { // THIS IS NOT GOOD
  // starting point of text tab lmao
  height -= 2;
}*/

    if (side + tekst.size() >= right) {
      side--;
      continue;
    } else if (side < middlescr && primarySide > middlescr) {
      side = middlescr + 2;
      int middle = 80;                  // TO MUSI BYC SKALOWALNE
                                        // ŹLE ROZDZIELAMY Z MIDDLE
      tekst2 = tekst.substr(0, middle); // TO TEZ SLABE
      tekst.erase(0, middle);           // TO JEST SLABE
                                        /*
                                  tesktWriteVec.push_back(tekst2);
                                  sideVec.push_back(side);
                                  heightVec.push_back(starty);*/

    } else if (side + tekst.size() > middlescr && primarySide < middlescr) {
      side = primarySide;
      int middle = 80;                  // TO MUSI BYC SKALOWALNE
                                        // ŹLE ROZDZIELAMY Z MIDDLE
      tekst2 = tekst.substr(0, middle); // TO TEZ SLABE
      tekst.erase(0, middle);           // TO JEST SLABE
                                        /*
                                  tesktWriteVec.push_back(tekst2);
                                  sideVec.push_back(side);
                                  heightVec.push_back(starty);*/

    } else {

      tesktWriteVec.push_back(tekst);
      sideVec.push_back(side);
      heightVec.push_back(height);
      break;
    }

    tesktWriteVec.push_back(tekst2);
    sideVec.push_back(side);
    heightVec.push_back(height);
    height++;
  }
  for (int i{}; i < tesktWriteVec.size(); i++) {
    if (heightVec.at(i) < 1) {
      continue;
      // text is too height , save it in another file?
    } else if (heightVec.at(i) + 2 > MyParts->upper_window_height) {
      continue;
    }
    wmove(new_win, heightVec.at(i), sideVec.at(i));
    wrefresh(new_win);
    wprintw(new_win, tesktWriteVec.at(i).c_str());
    //    wprintw(tesktWriteVec.at(i).c_str());
    wrefresh(new_win);
  }
}
