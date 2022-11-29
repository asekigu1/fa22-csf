#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection conn;

  // TODO: connect to server
  // save socket descriptor
  int fd = open_clientfd(argv[1], argv[2]);
  if (fd < 0) {
    std::cerr << "Couldn't connect to server" << std::endl;
  }

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)

  //login
  std::string receiver_login= "rlogin:" + username + "\n";
  const char* receiver_login_c = receiver_login.c_str();
  rio_writen(fd, receiver_login_c, strlen(receiver_login_c)); // send to server
  
  // read response from server
  rio_t rio1; 
  rio_readinitb(&rio1, fd);
  char buf1[1000];
  ssize_t n = rio_readlineb(&rio1, buf1, sizeof(buf1));
  if (n > 0) {
    std::string s = buf1;
    std::string delimiter = ":";
    std::string first = s.substr(0, s.find(delimiter));
    std::string second = s.substr(s.find(delimiter)+1, s.length());
    
    if (first == "err") {
      std::cerr << second << std::endl;
      close(fd);
      return 1;
    }

    // successfully logged in
  }

  std::string receiver_room= "join:" + room_name + "\n";
  const char* receiver_room_c = receiver_room.c_str();
  rio_writen(fd, receiver_room_c, strlen(receiver_room_c));

  // read response from server
  rio_t rio2; 
  rio_readinitb(&rio2, fd);
  char buf2[1000];
  n = rio_readlineb(&rio2, buf2, sizeof(buf2));
  if (n > 0) {
    std::string s = buf2;
    std::string delimiter = ":";
    std::string first = s.substr(0, s.find(delimiter));
    std::string second = s.substr(s.find(delimiter)+1, s.length());
    
    if (first == "err") {
      std::cerr << second << std::endl;
      close(fd);
      return 1;
    }

    // successfully logged in
  }

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  int c = 1;
  while (c) {
    rio_t rio; 
    rio_readinitb(&rio, fd);
    char buf[1000];
    n = rio_readlineb(&rio, buf, sizeof(buf));
    if (n > 0) {
      std::string s = buf;
      std::string delimiter = ":";
      std::string first = s.substr(0, s.find(delimiter));
      std::string second = s.substr(s.find(delimiter)+1, s.length());
    
      if (first == "delivery") {
        std::cout << second << std::endl;
      }

    }

    
  }


  close(fd);
  return 0;
}
