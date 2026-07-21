#include "server.h"
#include <arpa/inet.h>
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
  serverAddress.sin_addr.s_addr = inet_addr("192.168.0.67");

  // binding socket.
  bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

  // listening to the assigned socket
  listen(serverSocket, 5);

  // accepting connection request
  int clientSocket = accept(serverSocket, nullptr, nullptr);
  std::cout << "primary clientSOcket is " << clientSocket << std::endl;
  return clientSocket;
}
void Server::closeNetwork() { close(clientSocket); }
