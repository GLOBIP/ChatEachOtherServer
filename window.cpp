
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
                                          WindowParts *MyParts, WINDOW *new_win,
                                          int &bottomheightChat,
                                          windowShowProgra &CreateDestroyWin) {

  std::ifstream plikOtworz("comm.txt", std::ios::in);
  int y{};
  int originHeight = height;
  std::string zdanie = "";
  char WhosMessage{};
  bool isSecondTime{false};
  int countLine{0};
  int padding{5};
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
      drawOnScreen(MyParts->left, MyParts, height, zdanie.c_str(), new_win,
                   CreateDestroyWin, "You");
    else if (WhosMessage == 'S')

      drawOnScreen(MyParts->right, MyParts, height, zdanie.c_str(), new_win,
                   CreateDestroyWin, "other");
    height += padding;

    countLine -= padding;
  }
  // std::cout << "POWINNO BYĆ -170 a jest " << countLine << std::endl;
  bottomheightChat = countLine + padding * 3;
  wrefresh(new_win);
}
void windowFileRelated::drawOnScreen(int side, WindowParts *MyParts,
                                     int &height, const char *msg,
                                     WINDOW *new_win,
                                     windowShowProgra &CreateDestroyWin,
                                     std::string who) {

  std::string tekst = msg;
  int primarySide = side;
  std::string tekst2 = "";
  std::vector<std::string> tesktWriteVec = {}; // text
  std::vector<int> sideVec = {};               // for readen side (X) for each
  std::vector<int> heightVec = {};             // for readen text height of each
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
    1                              heightVec.push_back(starty);*/

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

  WINDOW *textWindow; // pointer do jednego okna

  bool once{true};
  for (int i{}; i < tesktWriteVec.size(); i++) { // po kawalkach tekstu lecimy
    if (heightVec.at(i) < 1) {
      continue;
      // text is too height , save it in another file?
    } else if (heightVec.at(i) > MyParts->upper_window_height + 4) {
      continue;
    }
    if (once) {
      textWindow = subwin(
          new_win, 3 * tesktWriteVec.size(),
          tesktWriteVec.at(0).size() +
              5, // takes tekstwrite num and *3 // width is size of first plus 5
          heightVec.at(0) - 5, // height is height of first - 5 on the top
          sideVec.at(0));      //  and side is side
      box(textWindow, 0, 0);   /* 0, 0 gives default characters
                                * for the vertical and horizontal
                                * lines			*/
      mvwprintw(textWindow, 0, 0, who.c_str());
    }

    wrefresh(new_win);
    mvwprintw(textWindow, 1, 3, tesktWriteVec.at(i).c_str());
    wrefresh(textWindow);
  }
}
