#include "connection.h"
#include "state.h"
#include <cassert>

connection::connection(packet p) {
  this->src_addr         = p.src_addr(); 
  this->dst_addr         = p.dst_addr();
  this->src_port         = p.src_port(); 
  this->dst_port         = p.dst_port();
  this->connection_reset = p.rst();
  this->start_time = p.ts_milli() + p.ts_sec()*1000000;
  this->change_state(std::shared_ptr<s0f0>(new s0f0));
  this->recv_packet(p);
}

bool connection::check_packet(packet p) {
  //if (p.syn() && !p.ack()) return false;
  // to server
  if (p.src_addr() == this->src_addr 
    &&p.dst_addr() == this->dst_addr
    &&p.src_port() == this->src_port
    &&p.dst_port() == this->dst_port)
  { return true; }

  // to client
  if (p.src_addr() == this->dst_addr 
    &&p.dst_addr() == this->src_addr
    &&p.src_port() == this->dst_port
    &&p.dst_port() == this->src_port)
  { return true; }
  return false;
}

void connection::recv_packet(packet p) {
  using namespace std;
  assert(this->check_packet(p));

  this->packet_num++;
  this->byte_total += p.data_size();
  this->window_sizes.push_back(p.window_size());

  if (this->src_to_dst(p)) {
    this->packet_src_to_dst_num++;
    this->byte_src_to_dst_num += p.data_size();
  }

  if (this->dst_to_src(p)) {
    this->packet_dst_to_src_num++;
    this->byte_dst_to_src_num += p.data_size();
  }

  if (p.rst()) {
    this->connection_reset = true;
    this->change_state(std::shared_ptr<reset>(new reset));
  } 

  this->state->recv_packet(p, this);

  // do RTT calculations
  if (this->src_to_dst(p)) {
    if (p.syn() && !p.ack()) {
      cout << "Syn1: " << p << endl;
      this->seq_num = p.seq_number();
      this->nxt_ack = p.seq_number() + 1;
    }
  }
  if (this->dst_to_src(p)) {
    if (p.syn() && p.ack()) {
        this->ack_num = p.ack_number();
        cout << "Syn2: " << p << endl;
      if (p.ack_number() == (this->seq_num + 1)) {
        // calculate rtt for first packet
      }
    }
  }
    
  // for every packet i need to store its seq number and timestamp
  // when i recieve the appropriate ack number
  // calculate the rtt
}

bool connection::is_completed() { return this->complete; }

void connection::set_completed(bool c) { this->complete = c; }

void connection::set_reset(bool r) { this->connection_reset = r; }

bool connection::is_reset() { return connection_reset; }

void connection::change_state(std::shared_ptr<connection_state> s) {
  this->state = s;
}

bool connection::src_to_dst(packet p) {
  return p.src_addr() == this->src_addr
      && p.dst_addr() == this->dst_addr;
}

bool connection::dst_to_src(packet p) {
  return p.dst_addr() == this->src_addr
      && p.src_addr() == this->dst_addr;
}

void connection::set_end_time(suseconds_t t) {
  this->end_time = t;
}

float connection::start() {
  return (float)(this->start_time - this->beginning)/((float)(1000000));
}

float connection::end() {
  return (float)(this->end_time - this->beginning)/((float)(1000000));
}

float connection::duration() {
  return (float)(this->end_time - this->start_time)/((float)(1000000));
}
void connection::configure_timestamp(suseconds_t begin) {
  this->beginning = begin;
}

bool connection::reseted() { return this->connection_reset; }

std::string connection::state_name() { return state->name(); }

std::ostream& operator<<(std::ostream& os, connection& c) {
  using namespace std;
  os << " Source Address: "          << c.src_addr                      << endl
     << " Destination Address: "     << c.dst_addr                      << endl
     << " Source Port: "             << c.src_port                      << endl
     << " Destination port: "        << c.dst_port                      << endl
     << " Status: "                  << *c.state                        << endl
     << " seq_num: "                 << c.seq_num                      << endl
     << " ack_num: "                 << c.ack_num                      << endl
     << " nxt_ack: "                 << c.nxt_ack                      << endl
  ;
  if (c.complete) {
   os<< " Start time: "              << c.start()                       << endl
     << " End time: "                << c.end()                         << endl
     << " Duration: "                << c.duration()                    << endl
     << " packets src to dst: "      << c.packet_src_to_dst_num         << endl
     << " packets dst to src: "      << c.packet_dst_to_src_num         << endl
     << " packets: "                 << c.packet_num                    << endl
     << " data bytes src to dst: "   << c.byte_dst_to_src_num           << endl
     << " data bytes dst to src: "   << c.byte_dst_to_src_num           << endl
     << " data bytes total: "        << c.byte_total                    << endl
  ;
  }
  return os;
}
