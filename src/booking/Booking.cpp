#include "Booking.h"

#include <fstream>
#include <iostream>
#include <algorithm>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

using json = nlohmann::json;
using namespace std;

using namespace booking;

Booking::Booking(const string& filepath) {

    movies.clear();
    theaters.clear();
    movies_sessions.clear();
    _not_found.clear();

    spdlog::info("load reseravations from: {}", filepath);

    std::ifstream f(filepath);
    json data = json::parse(f);

    
    for (auto& el : data.items()) {
        spdlog::debug("Movie: {}", el.key());
        
        map<string, Session*> sessions;

        for (auto& th_seats : el.value().items()) {
            string theater = th_seats.key(); 
            spdlog::debug("* theater: {}", theater);
            map<string, int> seats;

            json o = th_seats.value();
            for (auto& seat : o.items()) {
                spdlog::debug("* * seat: {}: {}", seat.key(), seat.value().get<int>());
                seats[seat.key()] = seat.value();
            }
            Session* session = new Session(seats, theater, el.key());
            sessions[theater] = session;
        }
        movies_sessions[el.key()] = sessions;
    }

    for (auto& m: movies_sessions) {
        auto& movie_name = m.first;
        movies.push_back(movie_name);

        vector<string> _theaters;
        for (auto& t: movies_sessions[movie_name]) {
            _theaters.push_back(t.first);
        }
        theaters[movie_name] = _theaters;
    }
}

Booking::~Booking() {
    for (auto& mv_session : movies_sessions) {
        for (auto& th_session : mv_session.second) {
            if (th_session.second != NULL) {
                delete th_session.second;
            }
        }
    }
}


const vector<string>& Booking::get_movies() {
    return movies;
}

const vector<string>& Booking::get_theaters(const string& movie) {
    if (theaters.find(movie) != theaters.end()) {
        return theaters[movie];
    }
    else {
        spdlog::warn("Cannot find movie: {}", movie);
        return _not_found;
    }
    
}

vector<string> Booking::get_seat_names(const string& movie, const string& theater, bool available_only) {
    auto& _theaters = get_theaters(movie);
    if (find(_theaters.begin(), _theaters.end(), theater) != _theaters.end()) {
        return movies_sessions[movie][theater]->get_seat_names(available_only);
    }
    else {
        spdlog::warn("Cannot find theater: {} for movie: {}", theater, movie);
        return _not_found;
    }
}

vector<string> Booking::get_available_seats(const string& movie, const string& theater) {
    return get_seat_names(movie, theater, true);
}


STATUS Booking::book_seat(const string& movie, const string& theater, const string& seat) {
    // treat one seat as vector of seats
    vector<string> seats = {seat};
    return book_seats(movie, theater, {seat});

}

STATUS Booking::book_seats(const string& movie, const string& theater, const vector<string>& seats) {
    if (find(movies.begin(), movies.end(), movie) == movies.end()) {
        return STATUS::MOVIE_NOT_FOUND;
    }
    const vector<string>& _theaters = theaters[movie];
    if (find(_theaters.begin(), _theaters.end(), theater) == _theaters.end()) {
        return STATUS::THEATER_NOT_FOUND;
    }

    if (!movies_sessions[movie][theater]->seats_exist(seats)) {
        return STATUS::SEAT_NOT_FOUND;
    }
    if (!movies_sessions[movie][theater]->seats_are_available(seats)) {
        return STATUS::ALREADY_BOOKED;
    }

    bool res = movies_sessions[movie][theater]->book_seats(seats);
    if (res) {
        return STATUS::OK;
    }
    else {
        return STATUS::UNKNOWN_ERROR;
    }
}
