#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <string>
#include <pthread.h>
#include "connection.h"
#include "user.h"
class Room;

struct Info;

class Server {
public:
  Server(int port);
  ~Server();

  bool listen();

  void handle_client_requests();

  Room *find_or_create_room(const std::string &room_name);

  void chat_with_sender(Info* info, User* user);

  void chat_with_receiver(Info* info, User* user);

private:
  // prohibit value semantics
  Server(const Server &);
  Server &operator=(const Server &);

  typedef std::map<std::string, Room *> RoomMap;

  // These member variables are sufficient for implementing
  // the server operations
  int m_port;
  int m_ssock;
  RoomMap m_rooms;
  pthread_mutex_t m_lock;
};

struct Info{
  Connection* conn_info;
  Server* server;
  Info(Connection*);
};

#endif // SERVER_H
