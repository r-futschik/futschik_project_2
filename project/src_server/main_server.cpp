
#include <iostream>
#include <thread>




#include "player.h"
#include "game_master.h"
#include "asio.hpp"
#include "base64.h"
#include "CLI11.hpp"
#include "player_message.pb.h"



using namespace std;
using namespace asio;
using namespace asio::ip;


void start_game_server(vector<tcp::iostream*>& streams){
    int i{1};
    string data;
    while (true){
        i = not i;
        *streams[i] >> data;
        EndTurnMessage end_msg;
        end_msg.ParseFromString(Base64::from_base64(data));

        cout << end_msg.guess() << endl;
        NextTurnMessage next_msg;
        next_msg.set_guess(end_msg.guess());
        next_msg.set_sunk(GameMaster::check_guess(end_msg.guess(), i));

        cout << i << " "<< next_msg.sunk() << endl;
        for (int i = 0; i < 2; i++){
            *streams[i] << Base64::to_base64(next_msg.SerializeAsString()) << endl;
        }
    }
}


int main(int argc, char* argv[]) {

    short unsigned int port{1113};

    CLI::App app("Battleships");

    app.add_option("-p,--port", port, "Port to connect to(Default 1113)", true);

    CLI11_PARSE(app, argc, argv);

    vector<tcp::iostream*> streams;
    cout << "Server is running." << endl;
    for(unsigned int i = 0; i < 2; i++){
        try {
            io_context ctx;
            tcp::endpoint ep{tcp::v4(), port};
            tcp::acceptor acceptor{ctx, ep};
            
            acceptor.listen();
        
            tcp::iostream* strm{new tcp::iostream(acceptor.accept())};
            string data;
            *strm >> data;

            cout << data << endl;
            streams.push_back(strm);


            
            
        } catch (asio::system_error& e) {
            return 0;
        }
    }

    for(const auto strm: streams) {
        OpponentMsg msg;
        msg.set_name("Game is starting...");
            
        (*strm) << Base64::to_base64(msg.SerializeAsString()) << endl;
    }

    vector<string> player1_ships;
    vector<string> player2_ships;
    for(int i = 0; i < 2; i++) {
        string data;
        
        *streams[i] >> data;

        SetupMsg msg;
        msg.ParseFromString(Base64::from_base64(data));

        for (int j = 0; j < msg.ships_size(); j++){
            if (i == 0){
                player1_ships.push_back(msg.ships(j));
            } else {
                player2_ships.push_back(msg.ships(j));
            }
            

        }

    


    }
    
    GameMaster::store_ships(player1_ships, player2_ships);

    StartGameMessage msg;

    for (int i = 0; i < 2; i++){
        msg.set_your_turn(not i);
        *streams[i] << Base64::to_base64(msg.SerializeAsString()) << endl;
    }
    

    
    
    start_game_server(streams);
    
}
