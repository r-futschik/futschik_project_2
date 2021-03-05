#pragma once


#ifndef PLAYER_H
#define PLAYER_H


#include <string>
#include <vector>


class Player {
    private:
        std::string name;

        std::vector<Ship> ships;
    
    public:
        Player(std::string name) : name(name){};
        void add_ship(Ship ship);
};


#endif