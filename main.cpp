#include "states.h"
#include "connection.h"
#include "packet.h"
#include <cassert>
#include <pcap/pcap.h>
int main(int argc, char **argv) {

  using namespace std;
  pcap_t *pcap;
  const u_char * mpacket;
  char errbuf[1000];
  struct pcap_pkthdr header;
  struct bpf_program fp;
  char filter_exp[] = "tcp and ip";
  vector<connection*> conns;
  //bpf_u_int32 mask;   /* The netmask of our sniffing device */
  //bpf_u_int32 net;   /* The IP of our sniffing device */
  
  if (argc < 1) {
    cout << "Not enough arguements." << endl;
    return 1;
  }
  char * file = argv[1];
  cout << file << endl;
  if ((pcap = pcap_open_offline(file, errbuf)) == NULL) {
    cout << "Could not open pcap file: " << errbuf << endl;
    return 1;
  }
  if (pcap_compile(pcap, &fp, filter_exp, 0, 0) == -1) {
    cout << "Could not compile expression: " << filter_exp << pcap_geterr(pcap) << endl; 
  }
  if (pcap_setfilter(pcap, &fp) == -1) {
    cout << "Could not install filter: " << filter_exp << pcap_geterr(pcap) << endl;
  }
  
  while ((mpacket = pcap_next(pcap, &header)) != NULL) {
    auto new_packet = packet(mpacket, header.ts, header.caplen);
    bool new_connection = true;
    for (auto c: conns) {
      //if (c->check_packet(new_packet) && !new_packet.syn()) {
      //  new_connection = false;
      //}
    }
    if (new_connection) {
      cout << new_packet << endl;
      conns.push_back(new connection(new_packet));
    }
  }
  cout << "Number of connections: " << conns.size() << endl;
  return 0;
}
