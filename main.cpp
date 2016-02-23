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
  
  vector<connection*> conns;
  int resets = 0;
  //connection::capture_start = -1;

  while ((mpacket = pcap_next(pcap, &header)) != NULL) {
    auto new_packet = packet(mpacket, header.ts, header.caplen);

    if (global_hack::capture_start < 0) {
      global_hack::capture_start = 
        new_packet.ts_milli() + new_packet.ts_sec()*1000000;
    }

    if (new_packet.syn() && !new_packet.ack()) {
      conns.push_back(new connection(new_packet));
    } else if (new_packet.rst()) {
      resets++;
      conns.push_back(new connection(new_packet));  
    } else {
      for (auto c: conns) {
        if (c->check_packet(new_packet)) {
          c->recv_packet(new_packet);
        }
      }
    }
  }
  for (auto c: conns) {
    cout << *c << endl;
  }
  cout << "Number of connections: " <<  conns.size() << endl;
  cout << "Number of resets: " <<  resets << endl;
  return 0;
}
