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
  request.data.pop_back();

  if ( (request.tag != TAG_SLOGIN) && (request.tag != TAG_RLOGIN)) {
    Message error1(TAG_ERR, "Invalid login attempt!");
    info->conn_info->send(error1);
    delete info;
    return nullptr;
  }

  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)

  User* user = new User(request.data);
  Message login(TAG_OK, "logged in as "+request.data);
  info->conn_info->send(login);

  if (request.tag == TAG_SLOGIN) {
    info->server->chat_with_sender(info,user, info->server);
  } else if (request.tag == TAG_RLOGIN) {
    info->server->chat_with_receiver(info,user, info->server);
  }

  delete user;
  delete info;
  return nullptr;
}


}

void Server::chat_with_sender(Info* info,User* user, Server* server) {
  
  while(1) {
    Message request;
    bool success = info->conn_info->receive(request);
    
    if (!success) {
      Message error2(TAG_ERR, "Failed receiving message");
      info->conn_info->send(error2);
    }

    request.data.pop_back();
    if (request.tag == "join") {
      Room* room = server->find_or_create_room(request.data);
      room->add_member(user);
      user->users_room = room;
      Message join_room_message(TAG_OK, "room successfully joined");
      info->conn_info->send(join_room_message);
      
    } else if (request.tag == "sendall") {
      if (user->users_room == nullptr) {
        Message error3(TAG_ERR, "You are not in a room");
        info->conn_info->send(error3);
      }
      else {
        user->users_room->broadcast_message(user->username, request.data);
        Message sent_message(TAG_OK, "message sent");
        info->conn_info->send(sent_message);
      }

      
    } else if (request.tag == "leave") {
      if (user->users_room == nullptr) {
        Message not_in_room(TAG_ERR, "You are not in a room!");
        info->conn_info->send(not_in_room);
      }
      else {
        user->users_room->remove_member(user);
        user->users_room = nullptr;
        Message left_room(TAG_OK, "successfully left room");
        info->conn_info->send(left_room);
      }



    } else if (request.tag == "quit") {
      if (user->users_room != nullptr) {
        // user is in a room, remove this user
        user->users_room->remove_member(user);
        user->users_room = nullptr;
      }

      Message quit_message(TAG_OK, "quitting");
      info->conn_info->send(quit_message);
      return;
    } else {
      Message error(TAG_ERR, "Invalid command!");
      info->conn_info->send(error);
    }

    



  }
  
  

  
  

}

void Server::chat_with_receiver(Info* info,User* user, Server* server) {
  Message request;
  Room* room;
  info->conn_info->receive(request);
  request.data.pop_back();
  if (request.tag == "join") {
    room = server->find_or_create_room(request.data);
    room->add_member(user);
    user->users_room = room;
    Message join_room_message(TAG_OK, "room successfully joined");
    info->conn_info->send(join_room_message);
    
  }else {
    Message error(TAG_ERR, "Invalid join command from receiver!");
    info->conn_info->send(error);
  }



  while(1) {
    Message* temp = user->mqueue.dequeue();
    if (temp != nullptr) {
      bool success = info->conn_info->send(*temp);
      delete temp;
      if (!success) {
        room->remove_member(user);

      }
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
  
  for (RoomMap::iterator it = m_rooms.begin(); it != m_rooms.end(); it++) {
    delete it->second;
  }
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
    pthread_create(&thread, NULL, worker, info);



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
