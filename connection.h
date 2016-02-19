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
    virtual void recv_packet(packet p, connection * c) = 0;
};

class connection {
  friend class connection_state;
  public:
    connection(std::shared_ptr<connection_state> init) : state(init) { };
    connection(packet p) : 
      src_addr(p.src_addr()), dst_addr(p.dst_addr()),
      src_port(p.src_port()), dst_port(p.dst_port()) { }

    void change_state(std::shared_ptr<connection_state> s);
    bool check_packet(packet p);
    void recv_packet(packet p);
  private:
    std::shared_ptr<connection_state> state;
    std::string src_addr;
    std::string dst_addr;
    int src_port;
    int dst_port;
};
#endif
