#include "connection.h"
#include "states.h"
#include "packet.h"
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
int main(int argc, char **argv) {

  pcap_t *pcap;

  const unsigned char *mpacket;
  char errbuf[1000];
  struct pcap_pkthdr header;
  using namespace std;

  if (argc < 1) {
    cout << "Not enough arguements." << endl;
    return 1;
  }
  char * file = argv[0];    


  //if (pcap_open_offline(file, errbuf) == NULL) {
  //  cout << "Could not open pcap file: " << errbuf << endl;
  //  return 1;
  //}
     
  //while ((packet = pcap_next(pcap, &header)) != NULL) {

  //}

  packet p;
  vector<connection*> conns;
  conns.push_back(new connection(std::make_shared<s0f0>()));
  conns.push_back(new connection(std::make_shared<s1f0>()));
  conns.push_back(new connection(std::make_shared<s2f0>()));
  for (auto c: conns) {
    c->recv_packet(p);
  }
  for (auto c: conns) {
    c->recv_packet(p);
  }
  for (auto c: conns) {
    c->recv_packet(p);
  }
  return 0;
}
