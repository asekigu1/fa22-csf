#ifndef USER_H
#define USER_H

#include <string>
#include "message_queue.h"
#include "room.h"
#include "server.h"

struct Info;

struct User {
  std::string username;

  // queue of pending messages awaiting delivery
  MessageQueue mqueue;
  Room* users_room;
  Info* user_info;
  User(const std::string &username) : username(username) {
    users_room = nullptr; // initialize room to nullptr
  }
};

#endif // USER_H
