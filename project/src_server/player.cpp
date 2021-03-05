#include <iostream>

#include <string>
#include <vector>

#include "player.h"

using namespace std;


void Player::add_ship(Ship ship){
    ships.push_back(ship);
}

