#include "Session.h"

#include <spdlog/spdlog.h>

using namespace std;

using namespace booking;

Session::Session()
    : theater("")
    , movie("")
{

}


Session::Session(map<string, int>& _seats, const string& _theater, const string& _movie) {
    theater = _theater;
    movie = _movie;
    seats.clear();
    seats = _seats;
} 

Session::Session(Session& s) {
    theater = s.theater;
    seats.clear();
    seats = s.seats;
}


bool Session::seats_exist(const vector<string>& seat_names) {
    for (auto& s: seat_names) {
        if (seats.find(s) == seats.end()) {
            return false;
        }
    }
    return true;
}

bool Session::seats_are_available(const vector<string>& seat_names) {
    for (auto& s: seat_names) {
        if (seats.find(s) == seats.end()) {
            return false;
        }
        if (seats[s] != 0) { // it is already booked
            return false;
        }
    }
    return true;
}


vector<string> Session::get_seat_names(bool available_only) {
    vector<string> seats_name;
    for (auto& s: seats) {
        if (!available_only || s.second == 0) {
            seats_name.push_back(s.first);
        }
    }
    return seats_name;
}

bool Session::book_seats(const vector<string>& seat_names) {
    if (seats_are_available(seat_names)) {
        for (auto& s: seat_names) {
            seats[s] = 1;
        }
        return true;
    }
    else {
        return false;
    }
}