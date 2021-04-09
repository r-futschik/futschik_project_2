
#include <iostream>
#include <thread>

#include "base64.h"
#include "asio.hpp"
#include "player.h"
#include "game_master.h"
#include "CLI11.hpp"
#include "player_message.pb.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include "tabulate/tabulate.hpp"
#pragma GCC diagnostic pop


using namespace std;
using namespace asio;
using namespace asio::ip;



int main(int argc, char* argv[]) {

    short unsigned int port{1113};

    CLI::App app("Battleships");

    app.add_option("-p,--port", port, "Port to connect to(Default 1113)", true);

    CLI11_PARSE(app, argc, argv);

    vector<tcp::iostream*> streams;
    cout << "Server is running." << endl;
    for(unsigned int i = 0; i < 2; i++){
        try {
            io_context ctx;
            tcp::endpoint ep{tcp::v4(), port};
            tcp::acceptor acceptor{ctx, ep};
            
            acceptor.listen();
        
            tcp::iostream* strm{new tcp::iostream(acceptor.accept())};
            string data;
            *strm >> data;

            cout << data << endl;
            streams.push_back(strm);


            
            
        } catch (asio::system_error& e) {
            return 0;
        }
    }

    for(const auto strm: streams) {
        OpponentMsg msg;
        msg.set_name("Game is starting...");
            
        (*strm) << Base64::to_base64(msg.SerializeAsString()) << "\n";
    }
    
    
}
