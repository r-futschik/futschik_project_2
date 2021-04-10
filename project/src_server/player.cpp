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

vector<string> Player::get_ships(){
    return this->ship_locations;
}



void Player::make_a_guess(std::string position, bool sunk){
    guesses.push_back(make_tuple(position, sunk));
}
        
        
        
void Player::save_opponent_guess(std::string position, bool sunk){
    opponent_guesses.push_back(make_tuple(position, sunk));
}


int Player::has_a_guess(string position){
    for (unsigned int i = 0; i < guesses.size(); i++){
        if (position == get<0>(guesses[i])){
            if (get<1>(guesses[i])){
                return 2;
            } else {
                return 1;
            }
        }
    }
    
    return 0;
}


int Player::opponent_has_a_guess(string position){ 
    for (unsigned int i = 0; i < opponent_guesses.size(); i++){

        if (position == get<0>(opponent_guesses[i])){
            if (get<1>(opponent_guesses[i])){
                return 2;
            } else {
                return 1;
            }
        }
    }
    
    return 0;

}