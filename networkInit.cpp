#include "server.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <ncurses.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
void Err(std::string type) {
  char bufferErr[256];
  char *error =
      strerror_r(errno, bufferErr,
                 256); // get string message from errno, XSI-compliant version
  std::cout << error;
  std::string errorMsg = error;
  if (type == "socket") {
    if (errorMsg == "EACCES") {
      printf("Access Denied");
    } else if (errorMsg == "EAFNOSUPPORT")
      printf("No support IPv6");
  }

  else if (type == "bind") {
    if (errorMsg == "EACCES")
      printf("Address protected, user not superuser/root");
    else if (errorMsg == "EADDRINUSE") {
      printf("address already used");
    } else if (errorMsg == "EADDRINUSE") {
      printf("all port used");
    }
  } else if (type == "listen") {
    if (errorMsg == "EADDRINUSE")
      printf("another socket already listens");
  } else if (type == "connection") {
    if (errorMsg == "EAGAIN" || errorMsg == "EWOULDBLOCK")
      printf("socket is marked nonblocking and no connection are present to be "
             "accepted");
    if (errorMsg == "ECONNABORTED")
      printf("connection has been aborted");
    if (errorMsg == "ENOTSOCK")
      printf("sockfd is not a socket");
    if (errorMsg == "EPERM")
      printf("FIREWALL BLOCKS");
  } else if (type == "recv") {
    if (errorMsg == "EAGAIN" || errorMsg == "EWOULDBLOCK")
      printf(
          "socket is amrked nonblocking and recieve operation would block or "
          "too late arrived");
    if (errorMsg == "ECONNREFUSED")
      printf("A remote host refused to allow the network connection typically "
             "because it is not running the requested service");
  } else if (type == "send") {
    if (errorMsg == "EACCES")
      printf("write permission is denied on the destination socket file");
    if (errorMsg == "EBADF")
      printf("invalid descriptor");
    if (errorMsg == "EISCONN")
      printf("connection-mode socket was connected already but a recipient was "
             "specified");
    if (errorMsg == "ENOTCONN")
      printf("socket not connected no target");
    if (errorMsg == "ENOTSOCK")
      printf("problem with socket");
  }
  exit(1);
}
std::string connectAndGetIp(int &port, int serverSocket, int &clientSocket) {
  struct sockaddr_storage addr;
  socklen_t len = sizeof addr;
  char ipstr[INET6_ADDRSTRLEN];
  clientSocket = accept(serverSocket, (struct sockaddr *)&addr, &len);

  if (addr.ss_family == AF_INET) {
    struct sockaddr_in *s = (struct sockaddr_in *)&addr;
    port = ntohs(s->sin_port);
    inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
  } else { // AF_INET6
    struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
    port = ntohs(s->sin6_port);
    inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
  }

  std::string addressIpstr = ipstr;
  return addressIpstr;
}

int Server::initilizeNetwork(int port) {

  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    Err("socket");
  }

  // specifying the address
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);
  serverAddress.sin_addr.s_addr =
      INADDR_ANY; // argument to bind that tells the socket to listen on all
                  // available interfaces.

  // binding socket.
  int bindNum = bind(serverSocket, (struct sockaddr *)&serverAddress,
                     sizeof(serverAddress));
  if (bindNum == -1)
    Err("bind");

  // listening to the assigned socket
  int listenNum = listen(serverSocket, 5);
  if (listenNum == -1)
    Err("listen");

  // clientSocket = accept(serverSocket, nullptr, nullptr); // TO DZIALA!!!
  // accepting connection request
  // socket accepting plus getting addres ip protocol
  // for more check
  // https://stackoverflow.com/questions/2064636/getting-the-source-address-of-an-incoming-socket-connection
  // end of protocol

  int clientSocket;
  std::string address = connectAndGetIp(port, serverSocket, clientSocket);
  std::string message = "Connected to ";
  message += address;
  mvprintw(1, 1, message.c_str());
  refresh();
  return clientSocket;
}
void Server::sendValue(int socket, const char *message) {
  ssize_t sender = send(socket, message, strlen(message), 0);
  if (sender == -1)
    Err("send");
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
      if (!message.empty())
        message.pop_back();
    } else if (letter >= 32 && letter <= 127) {
      message.push_back(letter);
    }
    mvwprintw(my_win, 5, 3, message.c_str());
    wrefresh(my_win);
    wclrtoeol(my_win);
  }
}
std::string Server::readValue(int socket) {
  char buffer[1024] = {0};
  ssize_t recieve = recv(socket, buffer, sizeof(buffer), MSG_DONTWAIT);
  if (recieve == -1)
    Err("recv");
  std::string StringBuffer = buffer;
  return StringBuffer;
}
void Server::closeNetwork(int clientSocket) { close(clientSocket); }
