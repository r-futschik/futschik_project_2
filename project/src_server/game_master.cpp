#include <iostream>

#include <string>
#include <vector>
#include <map>


#include "game_master.h"
#include "player.h"


#include "tabulate/table.hpp"



using namespace tabulate;


using namespace std;





void GameMaster::print_board(Player player){
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

    for (int i = 1; i < 10; i++){
        for (int j = 1; j < 10; j++) {
            if (player.has_ship(table[i][0].get_text() + table[0][j].get_text())){
                 table[i][j].set_text("*");
                 table[i][j].format()
                            .font_background_color(Color::blue);
            } 

        }
    }



    // Set width of cells in each column


    // Iterate over cells in the first row
    

    // Iterate over rows in the table
    

    std::cout << table << std::endl;

    

}

bool check_geometry(string start, string end){
    bool horizontal{false};
    cin.clear();
    cin.ignore(1);



    if (start[0] == end[0]) {
        horizontal = true;
    } else if(start[1] != end[1]){
        cout << "Wrong input Ship can only be horizontal or vertical. Press Enter to proceed.";
        cin.get();

        return false;
    }
    if (int(start[0]) < 65 || int(start[0]) > 74){
        cout << "Wrong input, only letters between A and J are allowed. Press Enter to proceed.";
        cin.get();
        return false;
    } else if(stoi(start.substr(1, start.size() - 1)) < 1 || stoi(end.substr(1, end.size() - 1)) > 10){
        cout << "Wrong input, only numbers between 1 and 10 are allowed. Press Enter to proceed.";
        cin.get();
        return false;
    } else if (horizontal){
        if (stoi(start.substr(1, start.size() - 1)) > stoi(end.substr(1, end.size() - 1))){
            cout << "Wrong input Ship. Press Enter to proceed. ";
            cin.get();
            return false;
        } 
    } else {
        if (int(start[0]) > int(end[0])){
            cout << "Wrong input Ship. Press Enter to proceed. ";
            cin.get();
            return false;
        } 
    }


    return true;
}

bool check_ship_size(vector<string> location, Player player) {
    if (count(player.ship_sizes_left.begin(), player.ship_sizes_left.end(), location.size())){
        return true;
    } else {
        cout << "Player doesnt have any ships of this size left. Press Enter to proceed";
        cin.get();
        return false;
    }

}


bool check_overlap(vector<string> location, Player player){
    for (unsigned int i = 0; i < location.size(); i++){
        if (player.has_ship(location[i])){
            cout << "Ship cant overlap with an already existing one. Press Enter to proceed";
            cin.get();
            return false;
        }
    }
    return true;
}



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

        
            location.push_back(t + start[1]);

            
        }

    }

    return location;
}

void GameMaster::set_ships(Player player){
    while (player.ship_sizes_left.size() != 0){
        print_board(player);

    
        string start;
        string end;
        vector<string> location;
        cout << "Set up your Ships!" << endl;
        cout << "Ships left: " << endl;
        player.print_ship_sizes_left();
        cout << "Begin with entering the start point bla bla bla" << endl;
        cin >> start;
        cout << "Enter the end" << endl;
        cin >> end;

        if (check_geometry(start, end)){

            cout << endl;
            
            location = createShipLocation(start, end);

            if (check_ship_size(location, player) && check_overlap(location, player)){
                player.add_ship(location);
            }

        } 
    }
}


