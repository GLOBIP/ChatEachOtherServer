#include "server.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <ncurses.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
int Server::initilizeNetwork(int port) {

  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  // specifying the address
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);
  serverAddress.sin_addr.s_addr =
      INADDR_ANY; // argument to bind that tells the socket to listen on all
                  // available interfaces.

  // binding socket.
  bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

  // listening to the assigned socket
  listen(serverSocket, 5);

  // accepting connection request
  // socket accepting plus getting addres ip protocol
  // for more check
  // https://stackoverflow.com/questions/2064636/getting-the-source-address-of-an-incoming-socket-connection

  struct sockaddr_storage addr;
  socklen_t len = sizeof addr;
  char ipstr[INET6_ADDRSTRLEN];
  int clientSocket = accept(serverSocket, (struct sockaddr *)&addr, &len);
  if (addr.ss_family == AF_INET) {
    struct sockaddr_in *s = (struct sockaddr_in *)&addr;
    port = ntohs(s->sin_port);
    inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
  } else { // AF_INET6
    struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
    port = ntohs(s->sin6_port);
    inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
  }
  // end of protocol

  std::string addressIpstr = ipstr;
  char buffer[1024] = {};
  std::string message = "Connected to ";
  message += addressIpstr;
  mvprintw(1, 1, message.c_str());
  refresh();
  return clientSocket;
}
void Server::closeNetwork(int clientSocket) { close(clientSocket); }

std::string Server::readValue(int socket) {
  char buffer[1024] = {0};
  ssize_t recieve = recv(socket, buffer, sizeof(buffer), MSG_DONTWAIT);
  std::string StringBuffer = buffer;
  return StringBuffer;
}

void Server::sendValue(int socket, const char *message) {
  send(socket, message, strlen(message), 0);
}
std::string Server::catchInput(WINDOW *my_win) {
  int letter;
  std::string message;
  while (true) {
    letter = getch();
    if (letter == 10 || letter == KEY_ENTER) {
      std::string messageOutput = "Selected: ";
      messageOutput += message;
      mvwprintw(my_win, 5, 3, messageOutput.c_str());
      wrefresh(my_win);
      return message;
    } else if (letter == 127 || letter == KEY_BACKSPACE) {
      message.pop_back();
    } else if (letter >= 32 && letter <= 127) {
      message.push_back(letter);
    }
    mvwprintw(my_win, 5, 3, message.c_str());
    wrefresh(my_win);
  }
  clrtoeol();
}
