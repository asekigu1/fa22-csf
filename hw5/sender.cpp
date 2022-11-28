#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];
  //get sever host name as const char*
  const char* server_hostname_const = server_hostname.c_str();
  //get port as const char*
  std::string temp = std::to_string(server_port);
  const char* server_port_const = temp.c_str();

  // TODO: connect to server
  int fd = open_clientfd(server_hostname_const, server_port_const);
  if (fd < 0) {
    std::cerr << "Couldn't connect to server" << std::endl;
  }
  // TODO: send slogin message
  std::string sender_login= "slogin:" + username;
  const char* sender_login_c = sender_login.c_str();
  rio_writen(fd, sender_login_c, strlen(sender_login_c));
  rio_writen(fd, "\n", 1);
  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  int c = 1;
  while (c) {
    std::string s;
    std::cin >> s;
    if (s == "quit") {

    }
    const char* s_c = s.c_str();
    //std::string s_length = std::to_string(strlen(s_c));
    //const char* s_length_const = s
    rio_writen(fd, s_c, strlen(s_c));
    rio_writen(fd, "\n", 1);


    

  }

  
  close(fd);
  return 0;
}
