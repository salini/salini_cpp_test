#include <iostream>
#include <string>
#include <thread>

#include <zmq.hpp>
#include <argparse/argparse.hpp>

#include "booking_server/BookingServer.h"

using namespace std;
using namespace booking_server;

/**
 * Start the booking server
 *
 *  This starts the booking server and set the ZMQ communication with clients
 * 
 * It acepts the following parameters for CLI:
 * --filepath: the path to the json file initializing the seats occupancy
 * --port: the zmq port used to communicate with the server
 */
int main(int argc, char *argv[]) {
    // set CLI arguments
    argparse::ArgumentParser program("start_booking_server");
    program.add_argument("--filepath")
        .help("movie session filepath")
        .default_value(string(TEST_DIR) + "/test_movie_sessions.json");

    program.add_argument("--port")
        .help("zmq server port")
        .default_value(8888)
        .scan<'i', int>();
    
    try {
        program.parse_args(argc, argv);
    }
        catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    // start server
    BookingServer server(
        program.get<string>("--filepath"),
        program.get<int>("--port")
    );

    server.run_loop();
}

