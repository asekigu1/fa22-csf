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
  
  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];

  Connection conn;

  // TODO: connect to server
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    std::cerr << "Couldn't connect to server" << std::endl;
    return 1;
  }
  

  // TODO: send slogin message
  //Login
  Message login_msg;
  login_msg.tag = "slogin";
  login_msg.data = username;
  bool success = conn.send(login_msg);
  if (!success) {
    std::cerr << "Unsuccessful send attempt" << std::endl;
  }
  
  // read response from server
  Message received;
  success = conn.receive(received);
  if (!success) {
    std::cerr << "Unsuccessful receive attempt" << std::endl;
  }
  if (received.tag == "ok") {
    // good
  } else if (received.tag == "err") {
    std::cerr << received.data;
    return 1;
  }

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  int c = 1;
  while (c) {
    
    std::string s;
    getline(std::cin, s);
    
    if ((s.back() == '\0') || (s.back() == '\r')) {
      // trim new line or CR if it exists
      s.pop_back();
    }
    
    Message sending;
    if (s.front() == '/') {
      
      std::string delimiter = " ";
      
      // save command without "/"
      sending.tag = s.substr(1, s.find(delimiter)-1);
      if (sending.tag != "join" && sending.tag != "quit" && sending.tag != "leave") {
        std::cerr << "Valid commands are /join /quit and /leave" << std::endl;
        continue;
      }
      
      // part after command
      sending.data = s.substr(s.find(delimiter)+1);
      
    }else{
      // sending message
      sending.tag = "sendall";
      sending.data = s;

    }
    
    // send message to server
    bool success = conn.send(sending);
    if (!success) {
      std::cerr << "Unsuccessful send attempt" << std::endl;
    }
    // read response from server
    success = conn.receive(received);
    if (!success) {
      std::cerr << "Unsuccessful receive attempt" << std::endl;
    }
    if (received.tag == "ok") {
      // good
    } else if (received.tag == "err") {
      std::cerr << received.data;
    }

    if (sending.tag == "quit") {
      sending.data = "bye";
      conn.close();
      return 0;
    }

  }

  conn.close();
  return 0;
}
