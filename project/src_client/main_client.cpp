
#include <iostream>
#include <thread>
#include <vector>


#include "game_master.h"
#include "player.h"
#include "asio.hpp"
#include "base64.h"
#include "CLI11.hpp"
#include "player_message.pb.h"

using namespace std;
using namespace asio::ip;

void start_game(tcp::iostream& strm, Player& player){
    string guess;
    string data;
    getline(strm, data);

    StartGameMessage msg;
    msg.ParseFromString(Base64::from_base64(data));

    int game_over{0};

    bool your_turn = msg.your_turn();

    while (game_over == 0){
        GameMaster::print_game_board(player);
        NextTurnMessage next_msg;
        if (your_turn){

            cout << "Enter your guess!" << endl;
            cin >> guess;

            EndTurnMessage end_msg;
            end_msg.set_guess(guess);

            strm << Base64::to_base64(end_msg.SerializeAsString()) << "\n";

            string next_turn;
            getline(strm, next_turn);

            next_msg.ParseFromString(Base64::from_base64(next_turn));
            player.make_a_guess(next_msg.guess(), next_msg.sunk());
            your_turn = false;

        } else {
            cout << "Its your Opponents turn!" << endl;
            string next_turn;
            getline(strm, next_turn);


            next_msg.ParseFromString(Base64::from_base64(next_turn));
            player.save_opponent_guess(next_msg.guess(), next_msg.sunk());
            your_turn = true;
        }
        game_over = next_msg.game_over();
    }
}

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
            cout << "Couldnt connect to server!" << endl;
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

            cout << "Waiting for the other players to set up his ships..." << endl;

            start_game(strm, player);

        }
    
    } catch (exception& e) {
        cout << "Exception: {}\n" << e.what();
    }

    
}
