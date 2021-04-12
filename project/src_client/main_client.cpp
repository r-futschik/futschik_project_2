
#include <iostream>
#include <thread>
#include <vector>
#include <fstream>


#include "game_master.h"
#include "player.h"
#include "asio.hpp"
#include "base64.h"
#include "toml.hpp"
#include "CLI11.hpp"
#include "player_message.pb.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "json.hpp"



using namespace std;
using namespace asio::ip;
using jsonf = nlohmann::json;


auto consoleLogger = spdlog::stderr_color_mt("console");



auto resultFileLogger =  spdlog::basic_logger_mt("logWriter", "../logs.txt");



// main game cycle, in each cycle it is either the clients turn, or not in which case he has to wait.
int start_game(tcp::iostream& strm, Player& player, bool save_game, string save_game_path){
    auto fileErrorLogger =  spdlog::basic_logger_mt("DisconnectErrorLogWriter", "../error_logs.txt");
    string guess;
    string data;
    getline(strm, data);

    StartGameMessage msg;
    msg.ParseFromString(Base64::from_base64(data));

    int game_over{0};

    bool your_turn = msg.your_turn();
    NextTurnMessage next_msg;
    cout << msg.disconnect() << endl;
    if (not msg.disconnect()){
        while (game_over == 0){
            
            if (not next_msg.disconnect()){


                GameMaster::print_game_board(player);
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
            } else {
                fileErrorLogger->error("Your Opponent disconnected");
                throw "Your Opponent disconnected";
            }
        
        if (save_game){
            jsonf jsonfile;

            jsonfile["opponent_guesses"] = player.get_opponent_guesses();
            jsonfile["my_guesses"] = player.get_guesses();
            
            if (save_game_path != "") {
                ofstream file(save_game_path);
                file << jsonfile;

            } else {
                if (player.number == 1){
                    ofstream file("../../stats/player1/game.json");
                    file << jsonfile;
                } else {
                    ofstream file("../../stats/player2/game.json");
                    file << jsonfile;
                }
            }
        }
        
        }
    } else {
        
        fileErrorLogger->error("Your Opponent disconnected");
        throw "Your Opponent disconnected";
    }

    return next_msg.game_over();
}

// Client sends his name to the server and registers to play
void connect_to_server(tcp::iostream& strm, string name, Player& player){
    if(strm) {
        cout << "Waiting for a Opponent to join..." << endl;
        strm << name << endl; 

        string data;
        
        getline(strm, data);

        ConnectMessage msg;
        msg.ParseFromString(Base64::from_base64(data));
        cout << msg.name() << endl;
        player.number = msg.number();
        player.opponent_name = msg.opponent_name();


    } else {
        auto fileErrorLogger =  spdlog::basic_logger_mt("ClientErrorLogWriter", "../error_logs.txt");
        fileErrorLogger->error("Server is offline");
        throw "Couldnt connect to server!";
    }

}

// Client sets up his ships with toml
void set_toml_ships(string toml_path, Player& player){

    const auto data  = toml::parse(toml_path);
    const auto ships = toml::find<vector<vector<string>>>(data, "ships");

    GameMaster::set_ships_with_toml(player, ships);

}

// Client sends his ships after setup
void send_ships_to_server(tcp::iostream& strm, Player& player){

    if(strm) {
        SetupMsg msg;

        msg.mutable_ships()->Reserve(player.get_ships().size());

        
        for(const auto ship: player.get_ships()) {

            msg.add_ships(ship);

        }

        strm << Base64::to_base64(msg.SerializeAsString()) << "\n";
    }


}

int main(int argc, char* argv[]) {

    string port{"1113"};
    string name;

    bool save{false};

    bool save_game{false};
    string save_path{""};
    string path{""};
    
    
    CLI::App app("Battleships");

    app.add_option("-n,--name", name, "Your Username", true)->required();
    app.add_option("-p,--port", port, "Port to connect to(Default 1113)", false);
    app.add_option("--toml_path", path, "(Recommended) Path to the toml configuration file. Check how to set up a toml file in README.md")->check(CLI::ExistingFile);
    app.add_flag("-s, --save", save, "Save your wins into an json file");
    app.add_flag("--save_game", save_game, "Saves the whole game in a JSON file");
    app.add_option("--save_game_path", save_path, "Specify the file you want to save your stats in. Saves will otherwise use the default path")->check(CLI::ExistingFile);

    CLI11_PARSE(app, argc, argv);


    Player player = Player(name);
    tcp::iostream strm{"localhost", port};


    try {
        if (path != "") {
            set_toml_ships(path, player);
        }
        
        
        connect_to_server(strm, name, player);
        
        if (path == ""){
            GameMaster::set_ships(player);
        } else {
            cout << "Waiting for the other players to set up his ships..." << endl;
        }

        send_ships_to_server(strm, player);
        
        int won = start_game(strm, player, save_game, save_path);

        if (won == player.number){
            consoleLogger->info("You won the Game!");
        } else {
            consoleLogger->info("You lost the Game.");
        }
        if (save){
            jsonf jsonfile;

            if (won == player.number){
                jsonfile["game"] = "You won vs " + player.opponent_name;
            } else {
                jsonfile["game"] = "You lost vs " + player.opponent_name;
            }


            if (player.number == 1){
                ofstream file("../../stats/player1/stats.json");
                file << jsonfile;
            } else {
                ofstream file("../../stats/player2/stats.json");
                file << jsonfile;
            }
                
                
            
        }
        

    } catch (const char* msg) {
        consoleLogger->set_level(spdlog::level::err);
        consoleLogger->error(msg);
        consoleLogger->error("Check the Error-log for more Information");
        return EXIT_FAILURE;
    }


    


    

    
    

    
}
