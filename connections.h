#ifndef CONNECTIONS_H
#define CONNECIIONS_H
#include "connection.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
class connections {
  friend std::ostream& operator<<(std::ostream& os, connections& p);
  private:
    std::vector<connection*> conns;
    suseconds_t beginning;
    size_t resets = 0;
    std::function<bool(connection*)> is_complete  = [ ] (connection * c) { return  c->is_completed(); };   
    std::function<bool(connection*)> not_complete = [ ] (connection * c) { return !c->is_completed(); };   
  public:
    connections() { };
    void add_connection(connection * c) {
      conns.push_back(c);
      c->configure_timestamp(this->beginning);
      resets = c->reseted() ? resets + 1 : resets;
    };
    bool empty() { return conns.size() == 0 ? true : false; };
    void start_time(suseconds_t b) { this->beginning = b; };
    void recv_packet(packet p) {
      for (auto c: conns) {
        if (c->check_packet(p)) {
          c->recv_packet(p);
        }
      }
    };
   std::vector<connection*> incomplete_connections() {
     std::vector<connection*> ret = conns;
     ret.erase(std::remove_if(ret.begin(), ret.end(), is_complete), ret.end());
     return ret;
   };
   std::vector<connection*> complete_connections() {
     std::vector<connection*> ret = conns;
     ret.erase(std::remove_if(ret.begin(), ret.end(), not_complete), ret.end());
     return ret;
   };

   size_t complete_size() { return this->complete_connections().size(); };
   size_t incomplete_size() { return this->incomplete_connections().size(); };

   std::vector<int> packets() {
     using namespace std;
     auto comp = this->complete_connections();
     std::vector<int> ret;
     for (auto i : comp) {
       ret.push_back(i->packet_num);
     }
     return ret;
   };

   std::vector<u_short> windows() {
     using namespace std;
     auto comp = this->complete_connections();
     std::vector<u_short> ret;
     for (auto i : comp) {
       ret.insert(ret.end(), i->window_sizes.begin(), i->window_sizes.end());
     }
     return ret;
   };

   std::vector<float> times() {
     using namespace std;
     auto comp = this->complete_connections();
     std::vector<float> ret;
     for (auto i : comp) {
       ret.push_back(i->duration());
     }
     return ret; 
   };

   u_short min_window() {
     using namespace std;
     auto p = this->windows();
     return p[distance(p.begin(), min_element(p.begin(), p.end()))];
   }

   u_short max_window() {
     using namespace std;
     auto p = this->windows();
     return p[distance(p.begin(), max_element(p.begin(), p.end()))];
   }

   u_short mean_window() {
     auto t = this->windows();
      float ret = 0;
      for (auto i : t) {
        ret += i;
      }
      return ret/t.size();
   }

   int min_packet() {
     using namespace std;
     auto p = this->packets();
     return p[distance(p.begin(), min_element(p.begin(), p.end()))];
   }

   int max_packet() {
     using namespace std;
     auto p = this->packets();
     return p[distance(p.begin(), max_element(p.begin(), p.end()))];
   }

   int mean_packet() {
     auto t = this->packets();
      float ret = 0;
      for (auto i : t) {
        ret += i;
      }
      return ret/t.size();
   }

   float min_time() {
     auto t = this->times();
     return t[std::distance(t.begin(), std::min_element(t.begin(), t.end()))];
     
   }

   float max_time() {
     auto t = this->times();
     return t[std::distance(t.begin(), std::max_element(t.begin(), t.end()))];
   }

   float mean_time() {
     auto t = this->times();
      float ret = 0;
      for (auto i : t) {
        ret += i;
      }
      return ret/t.size();
   }
};

std::ostream& operator<<(std::ostream& os, connections& p) {
  using namespace std;
  for (auto c: p.conns) {
    os << *c << std::endl;
  }
  os << "Complete TCP connections: " << endl
     << "Total number of complete connections: " << p.complete_size() << endl
     << "Number of reset TCP connections: "      << p.resets << endl
     << "Number of TCP connection that were still open when the trace capture ended: " << p.incomplete_size() << endl << endl
     << "Minimum time durations: " << p.min_time() << endl
     << "Mean time durations: " << p.mean_time() << endl
     << "Max time durations: " << p.max_time() << endl << endl
     << "Minimum number of packets: " << p.min_packet() << endl
     << "Mean number of packets: " << p.mean_packet() << endl
     << "Max number of packets: " << p.max_packet() << endl << endl
     << "Minimum window size: " << p.min_window() << endl
     << "Mean window size: " << p.mean_window() << endl
     << "Max window size: " << p.max_window() << endl << endl
     ;
  return os;
}
#endif