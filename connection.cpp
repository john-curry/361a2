#include "connection.h"

bool connection::check_packet(packet p) {
  if ((p.src_addr() == this->src_addr  && //to server packet
       p.dst_addr() == this->dst_addr) ||
      (p.dst_addr() == this->src_addr  && //to client packet
       p.src_addr() == this->dst_addr)) {
      return true;
  }
  return false;
}

void connection::recv_packet(packet p) {
  this->state->recv_packet(p, this);
}

void connection::change_state(std::shared_ptr<connection_state> s) {
  this->state = s;
}
