
#include <iostream>
#include <thread>

#include "ship.h"
#include "player.h"

#include "asio.hpp"

using namespace std;
using namespace asio::ip;


int main() {

    Ship* ship = new Ship(3, "A1", "A3");

    for(int i = 0; i < 3; i++){
        cout << ship->location[i] << endl;
    }
     //location is public for testing
    /*
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
    */
}
