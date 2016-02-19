#include "packet.h"
#include <cassert>
packet::packet(const u_char * p, struct timeval ts, unsigned int cap_len) {
  using namespace std;
  this->completed = false;

  struct ip *mip;
  struct tcphdr *mtcp;
  unsigned int eth_header_length = sizeof(struct ether_header); 
  unsigned int ip_header_length;
  unsigned int tcp_header_length;
  this->p_string = ustring(p);

  this->capture_length = cap_len;

  if (too_short(eth_header_length)) {
    cerr << "Captured packet too small: ethhdr." << endl;
  } else { 
    // jump over ethernet header
    p += eth_header_length;
    cap_len -= eth_header_length;
    
    mip = (struct ip*)p;
    // rip addresses out of ip header    
    this->saddr = string(inet_ntoa(mip->ip_src));
    this->daddr = string(inet_ntoa(mip->ip_dst));

    ip_header_length = mip->ip_hl * 4;

    assert(ip_header_length >= 20 && ip_header_length <= 60);

    this->ip_hdr_len = ip_header_length;
     
    if (too_short(ip_header_length)) {
      cerr << "Captured packet too small: iphdr." << endl;
    } else {
      // jump over ip header to the tcp header
      p += ip_header_length;
      cap_len -= ip_header_length;
      
      mtcp = (struct tcphdr*)p;     
      tcp_header_length = mtcp->th_off * 4; // might be wrong
      this->tcp_hdr_len = tcp_header_length;

      if (too_short(tcp_header_length)) {
        cerr << "Captured packet too small: tcphdr." << endl;
      } else {
        // rip data out of the tcp header
        this->sport   = mtcp->th_sport; 
        this->dport   = mtcp->th_dport; 
        this->ack_num = mtcp->th_ack;
        this->seq_num = mtcp->th_seq;

        // jump over the tcp header
        p += tcp_header_length;
        cap_len -= tcp_header_length;
        // if there is data, copy it over
        if (cap_len == 0) {
          this->has_data = false;
        } else {
          this->has_data = true;
          this->data = ustring(data);
        }
        // packet is not malformed in any way
        this->completed = true;
      }
    }
  }
}

ustring packet::get_data() const {
  if (!completed) throw bad_packet_error("get_data");
  return data;
}

bool packet::too_short(unsigned int length) {
  if (capture_length < length) return true;
  return false;
}
    
std::string packet::src_addr() const {
  if (!completed) throw bad_packet_error("src_addr");
  return saddr;
}

std::string packet::dst_addr() const {
  if (!completed) throw bad_packet_error("dst_addr");
  return daddr;
}

u_short packet::dst_port() const {
  if (!completed) throw bad_packet_error("dst_port");
 return dport;
}

u_short packet::src_port() const {
  if (!completed) throw bad_packet_error("src_port");
 return sport;
}

bool packet::syn() const {
  return flags|TH_SYN;
}

bool packet::fin() const {
  return flags|TH_FIN;
}

bool packet::rst() const {
  return flags|TH_RST;
}

tcp_seq packet::ack() const {
  return ack_num;
}

tcp_seq packet::seq() const {
  return seq_num;
}

std::ostream& operator<<(std::ostream& os, const packet& p) {
  os << " src_port: " << (unsigned int) p.src_port() 
     << " dst_port: " << (unsigned int) p.dst_port() 
     << " src_addr: " << p.src_addr() 
     << " dst_addr: " << p.dst_addr()
     << " ack: " << p.ack()
     << " syn: " << p.syn()
     << " fin: " << p.fin()
     << " rst: " << p.rst()
     << " ip_hdr_len: " << (unsigned int)p.ip_hdr_len
     << " tcp_hdr_len: " << (unsigned int)p.tcp_hdr_len
     << " completed: " << p.complete();
  return os;
}
