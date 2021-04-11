#pragma once


#ifndef GAME_MASTER_H
#define GAME_MASTER_H


#include <string>
#include <vector>
#include "asio.hpp"

#include "player.h"

using namespace asio::ip;


class GameMaster {
    private:
        static std::vector<std::string> player1_ship_locations;
        static std::vector<std::string> player2_ship_locations;
    public:
        static void print_setup_board(Player player);
        static void print_game_board(Player player);
        static void set_ships(Player& player);
        static void store_ships(std::vector<std::string> player1_ship_locations, std::vector<std::string> player2_ship_locations);
        static void start_game(tcp::iostream& strm, Player& player);
        static bool check_guess(std::string guess, int player);
        static int get_player1_ships_left();
        static int get_player2_ships_left();
};


#endif