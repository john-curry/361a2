#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "packet.h"
using namespace std;

class connection;

class connection_state {
  public:
    virtual void recv_packet(const packet p, std::shared_ptr<connection> c) = 0;
};

class connection : public std::enable_shared_from_this<connection> {
   
  public:
    connection(connection_state * init) : state(init) { };

    void recv_packet(const packet p);
    void change_state(connection_state * s);
    
  private:
    connection_state * state;
};

void connection::recv_packet(const packet p) {
  this->state->recv_packet(p, std::shared_ptr<connection>(this));
}

void connection::change_state(connection_state * s) {
  this->state = s;
}

class s0f0: public connection_state {
  public: 
    void recv_packet(const packet p, std::shared_ptr<connection> c) {
      std::cout << "Recieved packet in state s0f0" << std::endl;
    };
};

class s1f0: public connection_state {
  public:
    void recv_packet(const packet p, std::shared_ptr<connection> c) {
      std::cout << "Recieved packet in state s1f0" << std::endl;
      c->change_state(new s0f0);
    };

};

class s2f0: public connection_state {

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
