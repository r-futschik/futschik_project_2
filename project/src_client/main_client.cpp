
#include <iostream>
#include <thread>
#include <vector>
#include "asio.hpp"
#include "base64.h"
#include "player.h"
#include "game_master.h"
#include "CLI11.hpp"
#include "player_message.pb.h"

using namespace std;
using namespace asio::ip;


int main(int argc, char* argv[]) {

    string port{"1113"};
    string name;


    
    CLI::App app("Battleships");

    app.add_option("-n,--name", name, "Your Username", true)->required();
    app.add_option("-p,--port", port, "Port to connect to(Default 1113)", true);

    CLI11_PARSE(app, argc, argv);

    cout << "Waiting for other player to join..." << endl;
    try {

        tcp::iostream strm{"localhost", port};

        if(strm) {
            strm << name << endl; 


            string data;
            
            getline(strm, data);

            OpponentMsg msg;
            msg.ParseFromString(Base64::from_base64(data));
            cout << msg.name() << endl;
            


            

        } else {
            cout << "Couldnt not connect to server!" << endl;
            return 1;
        }

        Player player = Player(name);

        GameMaster::set_ships(player);

        

        if(strm) {

            SetupMsg msg;

            msg.mutable_ships()->Reserve(player.get_ships().size());

            
            for(const auto ship: player.get_ships()) {

                msg.add_ships(ship);

            }

            strm << Base64::to_base64(msg.SerializeAsString()) << "\n";



                
        } else {
            cout << "Couldnt not connect to server!" << endl;
            return 1;
        }
    
        if (strm){

            string data;
            
            getline(strm, data);

            StartGameMsg msg;
            msg.ParseFromString(Base64::from_base64(data));
            cout << msg.message() << endl;

            GameMaster::start_game(strm);
        }
    
    } catch (exception& e) {
        cout << "Exception: {}\n" << e.what();
    }

    
}
