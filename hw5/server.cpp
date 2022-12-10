#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

void *worker(void *arg) {
  pthread_detach(pthread_self());

  // TODO: use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)
  
  Info* info = (Info*) arg;
  
  // TODO: read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response
  //server object and connection
  Message request;
  
  bool success = info->conn_info->receive(request);
  if (!success) {
    std::cerr << "Error receiving message";
  }
  else if (request.tag == TAG_SLOGIN) {
    User* user = new User(request.data);
    info->server->chat_with_sender(info,user);
    Message login;
    login.tag = "ok";
    login.data = "logged in as " + request.data;
    info->conn_info->send(login);
    
  }
  else if (request.tag == TAG_RLOGIN) {
    User* user = new User(request.data);
    Message login;
    login.tag = "ok";
    login.data = "logged in as " + request.data;
    info->conn_info->send(login);
    info->server->chat_with_receiver(info,user);
  } else {
    Message error;
    error.tag = "err";
    error.data = "Invalid login attempt!";
    info->conn_info->send(error);
  }
  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)

  return nullptr;
}


}

void Server::chat_with_sender(Info* info,User* user) {
  //
  while(1) {
    Message request;
    bool success = info->conn_info->receive(request);
    if (!success) {
      std::cerr << "Error receiving message";
    }
    if (request.tag == "join") {
      Room* room = find_or_create_room(request.data);
      room->add_member(user);
      user->users_room = room;
      Message join_room_message;
      join_room_message.tag = "ok";
      join_room_message.data = "room successfully joined";
      info->conn_info->send(join_room_message);
      
    } else if (request.tag == "sendall") {
      user->users_room->broadcast_message(user->username, request.data);
      Message sent_message;
      sent_message.tag = "ok";
      sent_message.data = "message sent";
      info->conn_info->send(sent_message);
    } else if (request.tag == "leave") {
      if (user->users_room == nullptr) {
        Message not_in_room;
        not_in_room.tag = "err";
        not_in_room.data = "You are not in a room!";
        info->conn_info->send(not_in_room);
      }
      else {
        user->users_room->remove_member(user);
        user->users_room = nullptr;
        Message left_room;
        left_room.tag = "ok";
        left_room.data = "left room";
        info->conn_info->send(left_room);
      }



    } else if (request.tag == "quit") {
      Message quit_message;
      quit_message.tag = "ok";
      quit_message.data = "logging you out";
      info->conn_info->close();
    } else {
      Message error;
      error.tag = "err";
      error.data = "Invalid command!";
      info->conn_info->send(error);
    }

    



  }
  
  

  
  

}

void Server::chat_with_receiver(Info* info,User* user) {
  while(1) {
    Message request;
    bool success = info->conn_info->receive(request);
    if (!success) {
      std::cerr << "Error receiving message";
    }

    if (request.tag == "join") {
      Room* room = find_or_create_room(request.data);
      room->add_member(user);
      user->users_room = room;
      Message join_room_message;
      join_room_message.tag = "ok";
      join_room_message.data = "room successfully joined";
      info->conn_info->send(join_room_message);
      
    } else if (request.tag == "leave") {
      if (user->users_room == nullptr) {
        Message not_in_room;
        not_in_room.tag = "err";
        not_in_room.data = "You are not in a room!";
        info->conn_info->send(not_in_room);
      }
      else {
        user->users_room->remove_member(user);
        user->users_room = nullptr;
        Message left_room;
        left_room.tag = "ok";
        left_room.data = "left room";
        info->conn_info->send(left_room);
      }
    } else if (request.tag == "quit") {
      Message quit_message;
      quit_message.tag = "ok";
      quit_message.data = "logging you out";
      info->conn_info->close();
    } else {
      Message error;
      error.tag = "err";
      error.data = "Invalid command!";
      info->conn_info->send(error);
    }



  }
  

}

Info::Info(Connection* conn) {
  this->conn_info = conn;
  
}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
  pthread_mutex_init(&m_lock, NULL);
}

Server::~Server() {
  // TODO: destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  std::string s = std::to_string(m_port);
  char const *c_m_port = s.c_str();
  m_ssock = open_listenfd(c_m_port);
  if (m_ssock < 0) {
    return false;
  }
  return true;


}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  while(1) {
    
    int clientfd = Accept(m_ssock, NULL, NULL);
    
    if (clientfd < 0) {
      std::cerr << "Error accepting client connection";
    }
    Connection* con_info = new Connection(clientfd);
    Info* info = new Info(con_info);
    info->server = this;
    
    pthread_t thread;
    int rc = pthread_create(&thread, NULL, worker, info);


    //server object
    


  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  if (m_rooms.find(room_name)!= m_rooms.end()) {
    return m_rooms.at(room_name);
  }
  Room* new_room = new Room(room_name);
  m_rooms.insert(std::pair<std::string,Room*>(room_name, new_room));
  return m_rooms.at(room_name);

}
