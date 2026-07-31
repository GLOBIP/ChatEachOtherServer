
#include "server.h"
#include <fstream>
#include <iostream>
#include <string>
void files::sendFileServer(std::string message) {
  try {
    std::ofstream fileWrite("comm.txt", std::ios::app);
    if (fileWrite.is_open()) {
      std::cout << "OPENED";
    } else {
      perror("failed");
    }
    std::string writeToFile = "Server\n";
    time_t timestamp;
    time(&timestamp);
    std::string time = ctime(&timestamp);
    writeToFile += time;
    writeToFile += message;
    writeToFile.push_back('\n');
    fileWrite.write(writeToFile.c_str(), writeToFile.size());
    fileWrite.close();
  } catch (int i) {
    printf("Something went wrong while writing to file ");
  }
}
void files::sendFileClient(std::string message) {
  try {
    std::ofstream fileWrite("comm.txt", std::ios::app);
    if (fileWrite.is_open()) {
      std::cout << "OPENED";
    } else {
      perror("failed");
    }
    std::string writeToFile = "Client\n";
    time_t timestamp;
    time(&timestamp);
    std::string time = ctime(&timestamp);
    writeToFile += time;
    writeToFile += message;
    writeToFile.push_back('\n');
    fileWrite.write(writeToFile.c_str(), writeToFile.size());
    fileWrite.close();
  } catch (int i) {
    printf("Something went wrong while writing to file ");
  }
}
