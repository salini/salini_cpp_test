#include "BookingServer.h"
#include "booking/Booking.h"
#include "string_operations.h"
#include "booking/Status.h"

#include <thread>
#include <string>
#include <sstream>
#include <zmq.hpp>

#include <spdlog/spdlog.h>

using namespace booking;
using namespace booking_server;
using namespace std;


struct BookingServer::Pimpl {
    int port;
    zmq::context_t ctx;
    zmq::socket_t socket; 

    Booking seat_booking;

    thread loop_thread;

    float simulate_delay_ms;

    Pimpl(const string& filepath, int _port, float _simulate_delay_ms)
        : seat_booking(filepath)
        , port(_port)
        , simulate_delay_ms(_simulate_delay_ms)
    {
        spdlog::debug("initialize server zmq");
        socket = zmq::socket_t(ctx, zmq::socket_type::rep);
        string binding_address = string("tcp://*:") + to_string(port);
        socket.bind(binding_address.c_str());
    }

    string process_request(const string& request) {
        auto req = split_string(request);

        STATUS status = STATUS::WRONG_COMMAND;
        if (req.size() == 0) {
            return status_to_string(STATUS::WRONG_COMMAND);
        }
        else if (req[0] == "book_seats") {
            if (req.size() <= 3) {
                return status_to_string(STATUS::WRONG_COMMAND);
            }
            string movie = req[1];
            string theater = req[2];
            vector<string> seats(req.begin()+3, req.end());
            spdlog::debug("request nb seats {}", seats.size());
            status = seat_booking.book_seats(movie, theater, seats);
            return status_to_string(status);
        }
        else if (req[0] == "get_movies") {
            auto& movies = seat_booking.get_movies();
            if (movies.size() > 0) {
                spdlog::info("return OK: {}", status_to_string(STATUS::OK));
                return status_to_string(STATUS::OK) + " " + join_vector_strings(movies);
            }
            else {
                spdlog::info("return UNKNOWN_ERROR: {}", status_to_string(STATUS::UNKNOWN_ERROR));
                return status_to_string(STATUS::UNKNOWN_ERROR);
            } 
        }
        else if (req[0] == "get_theaters") {
            if (req.size() <= 1) {
                return status_to_string(STATUS::WRONG_COMMAND);
            }
            auto& theaters = seat_booking.get_theaters(req[1]);
            if (theaters.size() > 0) {
                spdlog::info("return OK: {}", status_to_string(STATUS::OK));
                return status_to_string(STATUS::OK) + " " + join_vector_strings(theaters);
            }
            else {
                spdlog::info("return MOVIE_NOT_FOUND: {}", status_to_string(STATUS::MOVIE_NOT_FOUND));
                return status_to_string(STATUS::MOVIE_NOT_FOUND);
            }
        }
        else if (req[0] == "get_seat_names") {
            if (req.size() <= 2) {
                return status_to_string(STATUS::WRONG_COMMAND);
            }
            auto seat_names = seat_booking.get_seat_names(req[1], req[2], false);
            if (seat_names.size() > 0) {
                spdlog::info("return OK: {}", status_to_string(STATUS::OK));
                return status_to_string(STATUS::OK) + " " + join_vector_strings(seat_names);
            }
            else {
                spdlog::info("return THEATER_NOT_FOUND: {}", status_to_string(STATUS::THEATER_NOT_FOUND));
                return status_to_string(STATUS::THEATER_NOT_FOUND);
            }
        }
        else if (req[0] == "get_available_seats") {
            if (req.size() <= 2) {
                return status_to_string(STATUS::WRONG_COMMAND);
            }
            auto seat_names = seat_booking.get_available_seats(req[1], req[2]);
            if (seat_names.size() > 0) {
                spdlog::info("return OK: {}", status_to_string(STATUS::OK));
                return status_to_string(STATUS::OK) + " " + join_vector_strings(seat_names);
            }
            else {
                spdlog::info("return SEAT_NOT_FOUND: {}", status_to_string(STATUS::SEAT_NOT_FOUND));
                return status_to_string(STATUS::SEAT_NOT_FOUND);
            }
        }
        else {
            spdlog::info("return WRONG_COMMAND: {}", status_to_string(STATUS::WRONG_COMMAND));
            return status_to_string(STATUS::WRONG_COMMAND);
        }
    }

};


BookingServer::BookingServer(const string& filepath, int port, float simulate_delay_ms)
: p(new Pimpl(filepath, port, simulate_delay_ms))
{
    spdlog::info("start booking server");
}

BookingServer::~BookingServer() {
    delete p;
}

void BookingServer::run_loop() {
    while (true) {
        zmq::message_t  message;
        try {
            auto ret = p->socket.recv(message, zmq::recv_flags::none);
            if (ret.has_value() && (EAGAIN == ret.value())) {
                spdlog::warn("error while processing message");
                break;
            }
            spdlog::info("Received: {}", message.to_string());

            string reply = p->process_request(message.to_string());

            // simulate work
            std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(p->simulate_delay_ms));

            // send the reply to the client
            spdlog::info("Send reply: {}", reply);
            p->socket.send(zmq::buffer(reply), zmq::send_flags::none);
        }
        catch(zmq::error_t) {
            spdlog::info("shutting down");
            break;
        }
    }
    p->socket.close();
}

void BookingServer::run_loop_in_thread() {
    p->loop_thread = thread(&BookingServer::run_loop, this);
}

void BookingServer::shutdown() {
    spdlog::info("Shutdown server");
    p->ctx.shutdown();
    p->loop_thread.join();
}
