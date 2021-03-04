
#include <iostream>
#include <thread>

#include "asio.hpp"
#include <typeinfo> 

using namespace std;
using namespace asio;


int main() {
    ip::tcp::endpoint ep1{
    ip::address_v4::any(), 80};

    cout << ep1 << endl;
    cout<< ep1.address()<< ":"<< ep1.port()<< endl;


   
}
