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

    cout << player.name;

}


bool check_validity(){
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
    while (player.ship_sizes_lefts.size() != 0){
        print_board(player);

    
        string start;
        string end;
        vector<string> location;
        cout << "Set up your Ships!" << endl;
        cout << "Ships left: " << player.ship_sizes_lefts.count() endl;
        cout << "Begin with entering the start point bla bla bla" << endl;
        cin >> start;
        cout << "Enter the end" << endl;
        cin >> end;

        if (check_validity()){
            for(auto const& value: player.ship_sizes_left) {
                std::cout << value;
            }
            cout << endl;
            location = createShipLocation(start, end);
            player.add_ship(location);


            for(auto const& value: player.ship_sizes_left) {
                std::cout << value;
            }
        }
    }
}


