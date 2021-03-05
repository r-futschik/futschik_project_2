#pragma once


#ifndef PLAYER_H
#define PLAYER_H


#include <string>
#include <vector>
#include <tuple>



class Player {
    private:
        
        std::vector<std::string> ship_locations;
        std::vector<std::tuple<std::string, bool>> guesses;
    public:
        std::vector<int> ship_sizes_left{1, 1, 1, 1, 2, 2, 2, 3, 3, 4};
        std::string name;
        Player(std::string name) : name(name){};
        void add_ship(std::vector<std::string> ship);
};


#endif