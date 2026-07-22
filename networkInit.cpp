#include "server.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
int Server::initilizeNetwork() {

  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  // specifying the address
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(49157);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  // binding socket.
  bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

  // listening to the assigned socket
  listen(serverSocket, 5);

  // accepting connection request
  char buffer[1024] = {};
  int clientSocket = accept(serverSocket, nullptr, nullptr);
  return clientSocket;
}
void Server::closeNetwork(int clientSocket) { close(clientSocket); }

std::string Server::readValue(int socket) {
  char buffer[1024] = {0};
  ssize_t recieve = recv(socket, buffer, sizeof(buffer), 0);
  std::string StringBuffer = buffer;
  return StringBuffer;
}

void Server::sendValue(int socket, const char *message) {
  send(socket, message, strlen(message), 0);
}
