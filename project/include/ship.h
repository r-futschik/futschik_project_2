#pragma once


#ifndef SHIP_H
#define SHIP_H


#include <string>
#include <vector>


class Ship {
    private:
    public:
        std::vector<std::string> location; // for testing
        Ship(int size, std::string start, std::string end);
};


#endif