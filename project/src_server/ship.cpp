#include <iostream>

#include <string>
#include <vector>

#include "ship.h"

using namespace std;


Ship::Ship(int size, string start, string end){
    bool horizontal = false;



    if (start[0] == end[0]) {
        horizontal = true;
    }

    for(int i = 0; i < size; i++) {
        if (horizontal){

            location.push_back(start[0] + to_string(int(start[1]) - 48 + i));
        } else {
            location.push_back(to_string(int(start[0]) + 1) + start[1]);
        }
    }
}

