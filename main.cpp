#include "connection.h"
int main(int argc, char **argv) {
  // get packet from libpcap
  // initialize my packet class
  // pass packet class to connection class
  // set connection class state
  packet p;
  p.port = 32;
  s0f0 s1;
  s1f0 s2;
  vector<connection*> conns;
  conns.push_back(new connection(&s2));
  conns.push_back(new connection(&s1));
  for (auto c: conns) {
    c->recv_packet(p);
  }
  for (auto c: conns) {
    c->recv_packet(p);
  }
  return 0;
}
