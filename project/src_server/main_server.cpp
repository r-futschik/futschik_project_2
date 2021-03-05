
#include <iostream>
#include <thread>

#include "player.h"
#include "game_master.h"

#include "asio.hpp"

using namespace std;
using namespace asio::ip;


int main() {

    Player player = Player("test");



    GameMaster::print_board(player);
    GameMaster::set_ships(player);
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
