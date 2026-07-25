
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
void windowFileRelated::drawOnScreen(int side, WindowParts *MyParts,
                                     int &starty, const char *msg) {

  std::string tekst = msg;
  int primarySide = side;
  std::string tekst2 = "";
  std::vector<std::string> tesktWriteVec = {};
  std::vector<int> sideVec = {};
  std::vector<int> startyVec = {};
  int right = MyParts->startx + MyParts->width;
  int middlescr = MyParts->startx + MyParts->width / 2;
  for (;;) {
    while (starty + 2 > MyParts->height) { // some magic way MyParts.height is
                                           // starting point of text tab lmao
      starty -= 2;
    }
    if (side + tekst.size() >= right) {
      side--;
      continue;
    } else if (side < middlescr && primarySide > middlescr) {
      starty++;
      side = middlescr + 2;
      int middle = 80;                  // TO MUSI BYC SKALOWALNE
                                        // ŹLE ROZDZIELAMY Z MIDDLE
      tekst2 = tekst.substr(0, middle); // TO TEZ SLABE
      tekst.erase(0, middle);           // TO JEST SLABE
                                        /*
                                  tesktWriteVec.push_back(tekst2);
                                  sideVec.push_back(side);
                                  startyVec.push_back(starty);*/

    } else if (side + tekst.size() > middlescr && primarySide < middlescr) {
      starty++;
      side = primarySide;
      int middle = 80;                  // TO MUSI BYC SKALOWALNE
                                        // ŹLE ROZDZIELAMY Z MIDDLE
      tekst2 = tekst.substr(0, middle); // TO TEZ SLABE
      tekst.erase(0, middle);           // TO JEST SLABE
                                        /*
                                  tesktWriteVec.push_back(tekst2);
                                  sideVec.push_back(side);
                                  startyVec.push_back(starty);*/

    } else {
      tesktWriteVec.push_back(tekst);
      sideVec.push_back(side);
      startyVec.push_back(starty);
      break;
    }

    // push back always because if it:
    // 1. Is too right it slides left and continuse - begin on start
    // 2. if all ok it breaks

    tesktWriteVec.push_back(tekst2);
    sideVec.push_back(side);
    startyVec.push_back(starty);
  }
  for (int i{}; i < tesktWriteVec.size(); i++) {
    move(startyVec.at(i), sideVec.at(i));
    refresh();
    printw(tesktWriteVec.at(i).c_str());
    refresh();
  }
}
