#include <iostream>

#include <string>
#include <vector>
#include <algorithm>


#include "player.h"

using namespace std;


void Player::add_ship(vector<string> ship){

    for (unsigned int i = 0; i < ship.size(); i++){
        ship_locations.push_back(ship[i]);
    }

    for (int i = 0; ship_sizes_left.size(); i++){
        if (ship_sizes_left[i] == int(ship.size())){
            ship_sizes_left.erase(ship_sizes_left.begin() + i);
            break;
        }
    }
}

bool Player::has_ship(string position){
    if (count(ship_locations.begin(), ship_locations.end(), position)){
        return true;
    } else {
        return false;
    }
    
}

void Player::print_ship_sizes_left(){
    cout << count(ship_sizes_left.begin(), ship_sizes_left.end(), 1) << " x 1 size Ships" << endl;
    cout << count(ship_sizes_left.begin(), ship_sizes_left.end(), 2) << " x 2 size Ships" << endl;
    cout << count(ship_sizes_left.begin(), ship_sizes_left.end(), 3) << " x 3 size Ships" << endl;
    cout << count(ship_sizes_left.begin(), ship_sizes_left.end(), 4) << " x 4 size Ships" << endl;
}