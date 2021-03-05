#pragma once


#ifndef GAME_MASTER_H
#define GAME_MASTER_H


#include <string>
#include <vector>

#include "player.h"


class GameMaster {
    private:
        
    public:

        static void print_board(Player player);
        static void set_ships(Player player);

};


#endif