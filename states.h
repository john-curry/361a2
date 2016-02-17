#ifndef CONN_ST_H
#define CONN_ST_H
#include "connection.h"
class s0f0: public connection_state {
  public: 
    void recv_packet(const packet p, connection * c) {
      std::cout << "Recieved packet in state s0f0" << std::endl;
    };
};

class s1f0: public connection_state {
  public:
    void recv_packet(const packet p, connection * c) {
      std::cout << "Recieved packet in state s1f0" << std::endl;
      c->change_state(std::make_shared<s0f0>());
    };
};

class s2f0: public connection_state {
    void recv_packet(const packet p, connection * c) {
      std::cout << "Recieved packet in state s2f0" << std::endl;
      c->change_state(std::make_shared<s1f0>());
    };
};

class s1f1: public connection_state {

};
  
class s2f1: public connection_state {

};

class s2f2: public connection_state {

};

class s0f1: public connection_state {

};

class s0f2: public connection_state {

};

class reset: public connection_state {

};

#endif
