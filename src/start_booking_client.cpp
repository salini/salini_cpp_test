#include <iostream>
#include <string>
#include <thread>

#include <zmq.hpp>
#include <argparse/argparse.hpp>

#include "booking_server/BookingClient.h"
#include "booking_server/string_operations.h"

using namespace std;
using namespace booking_server;

/**
 * Start the booking client and send request
 *
 *  This starts the booking client and set the ZMQ communication to send
 * a request to the server.
 * 
 * It acepts the following parameters for CLI:
 * --host: the zmq host used to communicate with the server
 * --port: the zmq port used to communicate with the server
 * --request:
 */
int main(int argc, char *argv[]) {
    // set CLI arguments
    argparse::ArgumentParser program("start_booking_client");
    program.add_argument("--host")
        .help("zmq server host")
        .default_value("localhost");

    program.add_argument("--port")
        .help("zmq server port")
        .default_value(8888)
        .scan<'i', int>();

    program.add_argument("--request")
        .help("request to get information")
        .nargs(argparse::nargs_pattern::any);

    try {
        program.parse_args(argc, argv);
    }
        catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    // start client
    BookingClient client(
        program.get<string>("--host"),
        program.get<int>("--port")
    );


    // interpret request
    string request = join_vector_strings(program.get<vector<string>>("--request"));

    // send request and print reply
    booking::STATUS status = client.send_request(request);
    if (status == booking::STATUS::OK) {
        auto reply = client.get_last_reply();
        if (reply.size() > 0) {
            string rep = join_vector_strings(client.get_last_reply());
            cout<< "REPLY: " << rep << endl;    
        }
    }
}