#ifndef PACKET_H
#define PACKET_H
#include <pcap/pcap.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <iostream>
#include <bitset>
#include <stdexcept>
// the wonderful power of being able to store unsigned chars in an object
// say goodbye to memcpy and strlen!!

typedef std::basic_string<u_char> ustring;
typedef std::bitset<4> short_bit;
typedef std::bitset<8> l_short_bit;

class bad_packet_error : public std::exception {
  public:
    bad_packet_error(const char * m) { method += std::string(m); }
    
    char const *what() const noexcept{
      return method.c_str();
    }
  private:
    std::string method = "Malformed_packet: " ;
};

class packet { 
  friend std::ostream& operator<<(std::ostream& os, const packet& p);
  public:
    packet(const u_char * p, struct timeval ts, unsigned int cap_len);
    bool syn() const;
    bool fin() const;
    bool rst() const;
    bool ack() const;
    tcp_seq ack_number() const;
    tcp_seq seq_number() const;
    bool complete() const { return completed; }
    u_short src_port() const;
    u_short dst_port() const;
    std::string src_addr() const;
    std::string dst_addr() const;
    int size();
    int data_offset();
    int window_size();
    time_t ts_sec() const;
    time_t ts_milli() const;
    ustring get_data() const;
    unsigned int data_size() const;
  private:
    ustring p_string; // string varient of the packet
    ustring data; // string varient of the data
    unsigned int capture_length; // length of packet
    unsigned int d_size; // length of packet
    std::string saddr; // ip addresses
    std::string daddr;
    l_short_bit flags;
    u_short sport; // port numbers
    u_short dport;
    u_short win; // window size
    tcp_seq ack_num;
    tcp_seq seq_num;
    u_char ip_hdr_len;
    u_char tcp_hdr_len;
    time_t time_stamp_sec;
    suseconds_t time_stamp_milli;
    bool completed = false; // flag is set of packet is not malformed
    bool has_data;
    bool too_short(unsigned int length); // packet length check
};
std::ostream& operator<<(std::ostream& os, const packet& p);
void ConvertToBinary(int n);

u_short short_swap( u_short s );
#endif
