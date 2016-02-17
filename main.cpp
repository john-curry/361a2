#include "connection.h"
int main(int argc, char **argv) {
  // get packet from libpcap
  // initialize my packet class
  // pass packet class to connection class
  // set connection class state
  packet p;
  p.port = 32;
  s0f0 s1;
  connection conn(s1);
  conn.recv_packet(p);
  return 0;
}
