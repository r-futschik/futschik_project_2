
#include <iostream>
#include <thread>

#include "asio.hpp"
#include <typeinfo> 

using namespace std;
using namespace asio;


int main() {
    auto a1{ip::make_address("127.0.0.1")};
    cout << a1.to_string() << ", " << a1 << endl;
    cout << a1.is_loopback() << endl;
    cout<< a1.is_v4()<< ", "<< a1.is_v6() << endl;
    cout<< a1.to_v4()<< endl;
}
