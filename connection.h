#ifndef CONNECTION_H
#define CONNECTION_H
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "packet.h"
class connection;

class connection_state {
  public:
    virtual void recv_packet(const packet p, connection * c) = 0;
};

class connection {
   
  public:
    connection(std::shared_ptr<connection_state> init) : state(init) { };
    void recv_packet(const packet p);
    void change_state(std::shared_ptr<connection_state> s);
    
  private:
    std::shared_ptr<connection_state> state;
};

void connection::recv_packet(const packet p) {
  this->state->recv_packet(p, this);
}

void connection::change_state(std::shared_ptr<connection_state> s) {
  this->state = s;
}

#endif
