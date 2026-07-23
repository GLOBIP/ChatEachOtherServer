
#include "server.h"
#include <fstream>
#include <iostream>
#include <string>
void files::sendFile(std::string message) {

  std::ofstream fileWrite("comm.txt", std::ios::app);
  if (fileWrite.is_open()) {
    std::cout << "OPENED";
  } else {
    perror("failed");
  }

  message.push_back('\n');
  fileWrite.write(message.c_str(), message.size());
}
