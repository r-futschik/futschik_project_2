
#include <iostream>
#include <thread>

#include "asio.hpp"
#include "base64.h"
#include "player.h"
#include "game_master.h"
#include "asio.hpp"
#include "CLI11.hpp"
#include "player_message.pb.h"

using namespace std;
using namespace asio::ip;


int main(int argc, char* argv[]) {

    string port{"1113"};
    string name;
    bool ready {false};
    CLI::App app("Battleships");

    app.add_option("-n,--name", name, "Your Username", true)->required();
    app.add_option("-p,--port", port, "Port to connect to(Default 1113)", true);

    CLI11_PARSE(app, argc, argv);

    cout << "Waiting for other player to join..." << endl;
    try {

        tcp::iostream strm{"localhost", port};

        if(strm) {
            strm << name << "\n"; 
            // strm << Base64::to_base64(rpmsg.SerializeAsString()) << "\n";

            string data;
            
            getline(strm, data);

            OpponentMsg msg;
            msg.ParseFromString(Base64::from_base64(data));
            cout << msg.name() << endl;
            strm.close();

            ready = true;

            

        } else {
            cout << "Couldnt not connect to server!\n";
        }
    } catch (exception& e) {
        cout << "Exception: {}\n" << e.what();
    }

    if (ready){
        Player player = Player(name);

        GameMaster::set_ships(player);
        
    }
}
