
#include <iostream>
#include <thread>

#include "asio.hpp"


using namespace std;
using namespace asio::ip;


int main() {
    asio::io_context ctx;
    tcp::endpoint ep{tcp::v4(), 9999};
    tcp::acceptor acceptor{ctx, ep}; 
    acceptor.listen();
    tcp::socket sock{ctx};
    acceptor.accept(sock);
    tcp::iostream strm{std::move(sock)};

    string data;
    strm >> data; 
    strm << data;
    strm.close(); 
}
