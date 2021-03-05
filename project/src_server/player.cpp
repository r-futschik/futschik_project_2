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

