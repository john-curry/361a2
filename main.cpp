#include "connection.h"
#include "connections.h"
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
  
  connections conns;

  while ((mpacket = pcap_next(pcap, &header)) != NULL) {
    auto new_packet = packet(mpacket, header.ts, header.caplen);
    cout << new_packet << endl;
    if (conns.empty()) {
        conns.start_time(new_packet.ts_milli() + new_packet.ts_sec()*1000000);
    }

    if (new_packet.syn() && !new_packet.ack()) {
      conns.add_connection(new connection(new_packet));
    }

    if (new_packet.rst()) {
      conns.add_connection(new connection(new_packet));  
    } 

    conns.recv_packet(new_packet);
   
  }
  cout << conns << endl;
  return 0;
}
