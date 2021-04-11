
#include <iostream>
#include <thread>




#include "player.h"
#include "game_master.h"
#include "asio.hpp"
#include "base64.h"
#include "CLI11.hpp"
#include "player_message.pb.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"


using namespace std;
using namespace asio;
using namespace asio::ip;

// main game cycle for server, server sends each client the notification to start the game and the next turn
void start_game_server(vector<tcp::iostream*>& streams){
    int i{1};
    string data;
    NextTurnMessage next_msg;
    next_msg.set_game_over(0);
    while (next_msg.game_over() == 0){
        i = not i;
        if (*streams[i]){
            *streams[i] >> data;
            EndTurnMessage end_msg;
            
            end_msg.ParseFromString(Base64::from_base64(data));


            
            next_msg.set_guess(end_msg.guess());
            next_msg.set_sunk(GameMaster::check_guess(end_msg.guess(), i));
            next_msg.set_disconnect(not (*streams[0] && *streams[1]));
            if (GameMaster::get_player1_ships_left() == 0){
                next_msg.set_game_over(2);
            }

            if (GameMaster::get_player2_ships_left() == 0){
                next_msg.set_game_over(1);
            }




            for (int i = 0; i < 2; i++){
                *streams[i] << Base64::to_base64(next_msg.SerializeAsString()) << endl;
            }
        } else {
            break;
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

    for (unsigned int i = 0; i < streams.size(); i++){
        msg.set_your_turn(not i);


        msg.set_disconnect(not (*streams[0] && *streams[1]));
        
        *streams[i] << Base64::to_base64(msg.SerializeAsString()) << endl;

    }
    

    
    
    start_game_server(streams);
    
}
