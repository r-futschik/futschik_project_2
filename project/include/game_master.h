#pragma once


#ifndef GAME_MASTER_H
#define GAME_MASTER_H


#include <string>
#include <vector>

#include "player.h"


class GameMaster {
    private:
        static std::vector<std::string> player1_ship_locations;
        static std::vector<std::string> player2_ship_locations;
    public:
        static void print_setup_board(Player player);
        static void print_game_board(Player player);
        static void set_ships(Player player);
        static void store_ships(std::vector<std::string> player1_ship_locations, std::vector<std::string> player2_ship_locations);

};


#endif