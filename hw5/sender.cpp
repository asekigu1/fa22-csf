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
    std::cerr << received.data << std::endl;
    conn.close();
    return 1;
  }

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  int c = 1;
  while (c) {
    std::string s;
    std::cin >> s;
    if (s == "quit") {
      conn.close();
      return 0;
    }

    Message sending; 
    if (s == "leave") {
      sending.tag = "leave";
      sending.data = "";
    } else if (s.substr(0,4) == "join") {
      sending.tag = "join";
      sending.data = s.substr(5, s.length());
    } else {
      sending.tag = username;
      sending.data = s;
    }

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
      std::cerr << received.data << std::endl;
      conn.close();
      return 1;
    }

  }

  conn.close();
  return 0;
}
