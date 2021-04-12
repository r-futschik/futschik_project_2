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
        std::vector<std::tuple<std::string, bool>> opponent_guesses;
    public:
        std::vector<std::string> get_ships();
        std::vector<int> ship_sizes_left{1, 1, 1, 1, 2, 2, 2, 3, 3, 4};
        std::string name;
        std::string opponent_name;
        int number;
        Player(std::string name) : name(name){};
        void add_ship(std::vector<std::string> ship);
        bool has_ship(std::string position);
        void print_ship_sizes_left();
        void make_a_guess(std::string position, bool sunk);
        void save_opponent_guess(std::string position, bool sunk);
        int has_a_guess(std::string position);
        int opponent_has_a_guess(std::string position);
        std::vector<std::tuple<std::string, bool>> get_guesses();
        std::vector<std::tuple<std::string, bool>> get_opponent_guesses();
};


#endif