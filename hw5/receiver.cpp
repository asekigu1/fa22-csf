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
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    std::cerr << "Couldn't connect to server" << std::endl;
    return 1;
  }

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  
  //Login
  Message login_msg;
  login_msg.tag = "rlogin";
  login_msg.data = username;
  bool success = conn.send(login_msg);
  if (!success) {
    std::cerr << "Unsuccessful send attempt at login" << std::endl;
    return 1;
  }
  // read response from server
  Message received;
  success = conn.receive(received);
  if (!success) {
    std::cerr << "Unsuccessful receive attempt at login" << std::endl;
    return 1;
  }
  if (received.tag == "ok") {
    // good
  } else if (received.tag == "err") {
    std::cerr << received.data;
    conn.close();
    return 1;
  }

  //Join
  Message join_msg;
  join_msg.tag = "join";
  join_msg.data = room_name;
  success = conn.send(join_msg);
  if (!success) {
    std::cerr << "Unsuccessful send attempt at join" << std::endl;
    return 1;
  }
  // read response from server
  success = conn.receive(received);
  if (!success) {
    std::cerr << "Unsuccessful receive attempt at join" << std::endl;
    return 1;
  }
  if (received.tag == "ok") {
    // good
  } else if (received.tag == "err") {
    std::cerr << received.data;
    conn.close();
    return 1;
  }

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  while (conn.receive(received)) {
    if (received.tag == "delivery") {
      std::string delimiter = ":";
      std::string temp = received.data.substr(received.data.find(delimiter)+1, received.data.length());
      std::string name = temp.substr(0, temp.find(delimiter)+1); // include colon
      std::string text = temp.substr(temp.find(delimiter)+1, temp.length()); // exclude colon
      std::cout << name << " " << text;
    }
  }


  conn.close();
  return 0;
}
