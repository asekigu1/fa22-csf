#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include <iostream>
Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, fd);
}

void Connection::connect(const std::string &hostname, int port) {
  // TODO: call open_clientfd to connect to the server
  // TODO: call rio_readinitb to initialize the rio_t object
  const char* hostname_const = hostname.c_str();
  
  std::string temp = std::to_string(port);
  const char* port_const = temp.c_str();
  m_fd = open_clientfd(hostname_const, port_const);
  rio_readinitb(&m_fdbuf, m_fd);
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  ::close(m_fd);
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return m_fd >= 0;
}

void Connection::close() {
  // TODO: close the connection if it is open
  ::close(m_fd);
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  std::string send = msg.tag + ":" + msg.data + "\n";
  
  const char* send_c = send.c_str();
  ssize_t n = rio_writen(m_fd, send_c, strlen(send_c));
  if (n<0) {
    m_last_result = EOF_OR_ERROR;
    return false;
  }

  m_last_result = SUCCESS;
    return true;
}

bool Connection::receive(Message &msg) {
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  
  char new_msg[msg.MAX_LEN];
  size_t n = 0;
  
  n = rio_readlineb(&m_fdbuf, new_msg, sizeof(new_msg));
  
  if (n > 0) {
    m_last_result = SUCCESS;

    std::string s = new_msg;
    std::string delimiter = ":";
    msg.tag = s.substr(0, s.find(delimiter));
    msg.data = s.substr(s.find(delimiter)+1, s.length());
    if ((msg.data.back() == '\0') || (msg.data.back() == '\r')) {
      // trim new line or CR if it exists
      msg.data.pop_back();
    }
    
    // successfully received

  } else {
    
    // if string lngth is <0, then error.
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  
  return true;
}

