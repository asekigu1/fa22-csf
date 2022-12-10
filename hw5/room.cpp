#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // TODO: initialize the mutex
  pthread_mutex_init(&lock, NULL);
}

Room::~Room() {
  // TODO: destroy the mutex
  pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  // TODO: add User to the room
  members.insert(user);

}

void Room::remove_member(User *user) {
  // TODO: remove User from the room
  members.erase(user);
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // TODO: send a message to every (receiver) User in the room

  for (User* user: members){
    if (user->username != sender_username) {
      std::string data = room_name + ":" + sender_username + ":" + message_text;
      Message* message = new Message("delivery", data);
      user->mqueue.enqueue(message);
      user->user_info->conn_info->send(*user->mqueue.dequeue());
    }
  }
}

