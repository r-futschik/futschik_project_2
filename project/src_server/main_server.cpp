
#include <iostream>
#include <thread>

#include "asio.hpp"
#include <typeinfo> 

using namespace std;
using namespace asio::ip;


int main() {
    tcp::iostream strm{"localhost", "9999"};
    if (strm) { 
        strm << "ping-pong" << endl;
        string data;
        getline(strm, data);
        cout << data << endl;
        strm.close();
    } else { 
        cout << "no connection" << endl; 
    }
}
