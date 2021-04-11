#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>

#include "asio.hpp"
#include "game_master.h"
#include "player.h"
#include "tabulate/table.hpp"
#include "player_message.pb.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include "tabulate/tabulate.hpp"
#pragma GCC diagnostic pop

using namespace tabulate;
using namespace std;
using namespace asio;
using namespace asio::ip;



vector<string> GameMaster::player1_ship_locations;
vector<string> GameMaster::player2_ship_locations;

auto fileErrorLogger =  spdlog::basic_logger_mt("errorLogWriter", "../error_logs.txt");

void clear_screen(){
    cout << string( 100, '\n' );
}

// store the ships of both players
void GameMaster::store_ships(vector<string> player1_ship_locations, vector<string> player2_ship_locations){
    GameMaster::player1_ship_locations = player1_ship_locations; 
    GameMaster::player2_ship_locations = player2_ship_locations;
}

// prints the game board only for setting up ships manually
void GameMaster::print_setup_board(Player player){
    Table table;

    table.add_row({" ", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
    table.add_row({"A", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"C", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"D", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"E", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"F", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"G", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"H", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"I", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"J", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});

    for (int i = 1; i < 11; i++){
        for (int j = 1; j < 11; j++) {
            if (player.has_ship(table[i][0].get_text() + table[0][j].get_text())){
                 table[i][j].set_text("*");
                 table[i][j].format()
                            .font_background_color(Color::blue);
            } 

        }
    }



    std::cout << table << std::endl;

    

}

// prints the game board during playing
void GameMaster::print_game_board(Player player){
    cout << "flushing" << flush;
    clear_screen();
    Table table;

    table.add_row({" ", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "                         ", " ", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
    table.add_row({"A", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "                         ","A", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "                         ","B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"C", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "        Schiffe          ","C", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"D", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "       Versenken         ","D", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"E", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "                         ","E", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"F", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "                         ","F", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"G", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "                         ","G", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"H", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "                         ","H", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"I", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "                         ","I", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});
    table.add_row({"J", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "                         ","J", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "});

    for (int i = 1; i < 11; i++){
        for (int j = 1; j < 11; j++) {
            if (player.has_ship(table[i][0].get_text() + table[0][j].get_text())){
                 table[i][j].set_text("*");
                 table[i][j].format()
                            .font_background_color(Color::green);
            } 

            switch (player.opponent_has_a_guess(table[i][0].get_text() + table[0][j].get_text())) {
                case 1:
                    table[i][j].format()
                                .font_background_color(Color::red);
                    break;
                case 2:
                    table[i][j].set_text("X");
                    table[i][j].format()
                            .font_background_color(Color::red);
                    break;

            }

        }
    }

    for (int i = 1; i < 11; i++){
        for (int j = 12; j < 22; j++) {


            switch (player.has_a_guess(table[i][0].get_text() + table[0][j].get_text())) {
                case 1:
                    table[i][j].format()
                                .font_background_color(Color::red);
                    break;
                case 2:
                    table[i][j].set_text("X");
                    table[i][j].format()
                            .font_background_color(Color::red);
                    break;
            }

        }
    }

    for (int i = 1; i < 11; i++){
        table[i][11].format()
                    .font_align(FontAlign::center)
                    .font_background_color(Color::blue);
    }



    std::cout << table << std::endl;

    

}

// checks if the given ship location is correct
bool check_geometry(string start, string end, bool toml){
    bool horizontal{false};
    if (not toml) {
        cin.clear();
        cin.ignore(1);
    }
    

    if (start.size() < 2 || start.size() > 3 || end.size() < 2 || end.size() > 3){
        if(not toml){
            cout << "Wrong Ship input. Press Enter to proceed.";
            cin.get();
        } else {
            fileErrorLogger->error("One or more Ships have too many/little characters");
        }
        return false;
    }

    if (start[0] == end[0]) {
        horizontal = true;
    } else if(start[1] != end[1]){
        if(not toml){
            cout << "Wrong input Ship can only be horizontal or vertical. Press Enter to proceed.";
            cin.get();
        } else {
            fileErrorLogger->error("Wrong input Ship can only be horizontal or vertical.");
        }

        return false;
    }
    if (int(start[0]) < 65 || int(start[0]) > 74){
        if (not toml){
            cout << "Wrong input, only letters between A and J are allowed. Press Enter to proceed.";
            cin.get();
        } else {
            fileErrorLogger->error("Wrong input, only letters between A and J are allowed.");
        }
        
        return false;
    } else if(stoi(start.substr(1, start.size() - 1)) < 1 || stoi(end.substr(1, end.size() - 1)) > 10){
        if (not toml){
            cout << "Wrong input, only numbers between 1 and 10 are allowed. Press Enter to proceed.";
        cin.get();
        } else {
            fileErrorLogger->error("Wrong input, only numbers between 1 and 10 are allowed.");
        }
        
        return false;
    } else if (horizontal){
        if (stoi(start.substr(1, start.size() - 1)) > stoi(end.substr(1, end.size() - 1))){
            if (not toml) {
                cout << "Wrong input Ship. Press Enter to proceed. ";
                cin.get();
            } else {
                fileErrorLogger->error("Wrong input, Ships have to be formated like this: (A1, A3), rather than this (A3, A1)");
            }
            
            return false;
        } 
    } else {
        if (int(start[0]) > int(end[0])){
            if (not toml) {
                cout << "Wrong input Ship. Press Enter to proceed. ";
                cin.get();
            } else {
                fileErrorLogger->error("Wrong input, Ships have to be formated like this: (A1, C1), rather than this: (C1, A1)");
            
            }
            
            return false;
        } 
    }



    return true;
}

// checks if the given ship size is correct
bool check_ship_size(vector<string> location, Player player, bool toml) {
    if (count(player.ship_sizes_left.begin(), player.ship_sizes_left.end(), location.size())){
        return true;
    } else {
        if (not toml){
            cout << "Player doesnt have any ships of this size left. Press Enter to proceed";
            cin.get();
        } else {
            fileErrorLogger->error("Wrong number of Ships");
            
        }
        
        return false;
    }

}

// checks if the given ship location overlaps with an existing ship
bool check_overlap(vector<string> location, Player player, bool toml){
    for (unsigned int i = 0; i < location.size(); i++){
        if (player.has_ship(location[i])){
            if (not toml){
                cout << "Ship cant overlap with an already existing one. Press Enter to proceed";
                cin.get();
            } else {
                fileErrorLogger->error("A Ship overlaps with an already existing one");
            
            }
            
            return false;
        }
    }
    return true;
}


// creates the location vektor to store
vector<string> createShipLocation(string start, string end){
    vector<string> location;
    bool horizontal = false;

    location.push_back(start);

    if (start[0] == end[0]) {
        horizontal = true;
    }


    for(int i = 0; location[i] != end; i++) {


        if (horizontal){


            location.push_back(start[0] + to_string(int(start[1]) - 48 + i + 1));


        } else {
            string t;
            t.push_back(char (int(start[0]) + i + 1));

            if (start.size() == 2){
                location.push_back(t + start[1]);
            } else {
                location.push_back(t + start[1] + start[2]);
            }

            

            
        }

    }

    return location;
}

// set ships manually
void GameMaster::set_ships(Player& player){
    while (player.ship_sizes_left.size() != 0){
        print_setup_board(player);


        string start;
        string end;
        vector<string> location;
        cout << "Set up your Ships!" << endl;
        cout << "Ships left: " << endl;
        player.print_ship_sizes_left();
        cout << "Begin with entering the start point of your ship" << endl;
        cin >> start;
        cout << "Enter the end" << endl;
        cin >> end;

        if (check_geometry(start, end, false)){

            cout << endl;
            
            location = createShipLocation(start, end);

            if (check_ship_size(location, player, false) && check_overlap(location, player, false)){
                player.add_ship(location);
            }

        } 
    }
}


// set ships with toml
void GameMaster::set_ships_with_toml(Player& player, vector<vector<string>> ships){


    for (unsigned int i = 0; i < ships.size(); i++){
        vector<string> location;

        if (check_geometry(ships[i][0], ships[i][1], true)){
            location = createShipLocation(ships[i][0], ships[i][1]);

            if (check_ship_size(location, player, true) && check_overlap(location, player, true)){
                player.add_ship(location);
            } else {

                throw "Toml file is not set up correctly, check README.md on how to set up toml files";
            }
        } else {
            throw "Toml file is not set up correctly, check README.md on how to set up toml files";
        }
    }

}

// check if the guess is correct
bool GameMaster::check_guess(string guess, int player){

    if (player){
        if (count(player1_ship_locations.begin(), player1_ship_locations.end(), guess)){
            player1_ship_locations.erase(find(player1_ship_locations.begin(), player1_ship_locations.end(), guess));
            return true;
        } else {
            return false;
        }
    } else {
        if (count(player2_ship_locations.begin(), player2_ship_locations.end(), guess)){
            player2_ship_locations.erase(find(player2_ship_locations.begin(), player2_ship_locations.end(), guess));
            return true;
        } else {
            return false;
        }
    }
}



int GameMaster::get_player1_ships_left(){
    return player1_ship_locations.size();
}

int GameMaster::get_player2_ships_left(){
    return player2_ship_locations.size();
}