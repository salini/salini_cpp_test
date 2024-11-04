#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <string>

#include "booking/Booking.h"
#include "booking_server/BookingServer.h"
#include "booking_server/BookingClient.h"

using namespace std;
using namespace booking;
using namespace booking_server;


TEST_CASE("Check booking") {
    string filepath = string(TEST_DIR) + "/test_movie_sessions.json";
    Booking my_booking(filepath);

    // Get data in booking
    auto& movies = my_booking.get_movies();
    REQUIRE( movies.size() == 3 );

    auto& theaters = my_booking.get_theaters("Movie1");
    REQUIRE( theaters.size() == 2 );

    auto& theaters2_ng = my_booking.get_theaters("WrongMovie");
    REQUIRE( theaters2_ng.size() == 0 );

    auto seats_name = my_booking.get_seat_names("Movie1", "Theater1", false);
    REQUIRE( seats_name.size() == 6 );

    auto seat_names_ng0 = my_booking.get_seat_names("WrongMovie", "WrongTheater", false);
    REQUIRE( seat_names_ng0.size() == 0 );

    vector<string> seat_names_ng1 = my_booking.get_seat_names("Movie1", "WrongTheater", false);
    REQUIRE( seat_names_ng1.size() == 0 );

    // check wrong inputs
    STATUS status = my_booking.book_seat("WrongMovie", "Theater1", "a1");
    REQUIRE( status == STATUS::MOVIE_NOT_FOUND );

    status = my_booking.book_seat("Movie1", "WrongTheater", "a1");
    REQUIRE( status == STATUS::THEATER_NOT_FOUND );

    status = my_booking.book_seat("Movie1", "Theater3", "a1"); // movie 1 not displayed in theater3
    REQUIRE( status == STATUS::THEATER_NOT_FOUND );

    status = my_booking.book_seat("Movie1", "Theater1", "WrongSeat");
    REQUIRE( status == STATUS::SEAT_NOT_FOUND );

    // perform booking and check status
    status = my_booking.book_seat("Movie1", "Theater1", "a1");
    REQUIRE( status == STATUS::OK );

    status = my_booking.book_seat("Movie1", "Theater1", "a1");
    REQUIRE( status == STATUS::ALREADY_BOOKED );

    status = my_booking.book_seats("Movie1", "Theater1", {"a2", "a3", "b1"});
    REQUIRE( status == STATUS::OK );

    status = my_booking.book_seats("Movie1", "Theater1", {"b1", "b2", "b3"}); // partially booked
    REQUIRE( status == STATUS::ALREADY_BOOKED );

    status = my_booking.book_seats("Movie1", "Theater2", {"b1", "b2", "b3"}); // Ok, other theater
    REQUIRE( status == STATUS::OK );
}


TEST_CASE("Check ZMQ connection") {
    string filepath = string(TEST_DIR) + "/test_movie_sessions.json";
    int port = 8888;
    BookingServer server(filepath, port, 0.0);

    server.run_loop_in_thread();

    BookingClient client("localhost", port);

    STATUS status;

    status = client.send_request("get_movies");
    if (status == STATUS::OK) {
        auto movies = client.get_last_reply();
        REQUIRE( movies.size() == 3 );
    }

    status = client.send_request("get_theaters Movie1");
    if (status == STATUS::OK) {
        auto theaters = client.get_last_reply();
        REQUIRE( theaters.size() == 2 );
    }

    status = client.send_request("get_seat_names Movie1 Theater1");
    if (status == STATUS::OK) {
        auto theaters = client.get_last_reply();
        REQUIRE( theaters.size() == 6 );
    }

    status = client.send_request("book_seats Movie1 Theater1 a1");
    REQUIRE( status == STATUS::OK );

    status = client.send_request("book_seats Movie1 Theater1 a1");
    REQUIRE( status == STATUS::ALREADY_BOOKED );

    status = client.send_request("get_available_seats Movie1 Theater1");
    if (status == STATUS::OK) {
        auto seats = client.get_last_reply();
        REQUIRE( seats.size() == 5 );
    }

    status = client.send_request("book_seats Movie1 Theater1 a2 a3 b1");
    REQUIRE( status == STATUS::OK );

    status = client.send_request("book_seats Movie1 Theater1 b1 b2");
    REQUIRE( status == STATUS::ALREADY_BOOKED );

    status = client.send_request("book_seats WrongMovie Theater1 b1 b2");
    REQUIRE( status == STATUS::MOVIE_NOT_FOUND );

    status = client.send_request("book_seats Movie1 WrongTheater b1 b2");
    REQUIRE( status == STATUS::THEATER_NOT_FOUND );

    status = client.send_request("book_seats Movie1 Theater1 b4");
    REQUIRE( status == STATUS::SEAT_NOT_FOUND );

    server.shutdown();
}

TEST_CASE("Check simultaneous requests") {
    string filepath = string(TEST_DIR) + "/test_movie_sessions.json";
    int port = 8888;
    BookingServer server(filepath, port, 1000.0);
    
    server.run_loop_in_thread();

    BookingClient client1("localhost", port);
    BookingClient client2("localhost", port);

    STATUS status1 = client1.send_request("book_seats Movie1 Theater1 a1");
    STATUS status2 = client2.send_request("book_seats Movie1 Theater1 a1");

    // check who is receiving first, and compare with second
    if (status1 == STATUS::OK) {
        REQUIRE(status2 == STATUS::ALREADY_BOOKED);
    }
    else if (status2 == STATUS::OK) {
        REQUIRE(status1 == STATUS::ALREADY_BOOKED);
    }

    server.shutdown();
}