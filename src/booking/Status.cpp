#include "Status.h"

using namespace booking;
using namespace std;

string booking::status_to_string(STATUS s) {
    if (s == STATUS::OK) {
        return "OK";
    }
    else if (s == STATUS::MOVIE_NOT_FOUND) {
        return "MOVIE_NOT_FOUND";
    }
    else if (s == STATUS::THEATER_NOT_FOUND) {
        return "THEATER_NOT_FOUND";
    }
    else if (s == STATUS::SEAT_NOT_FOUND) {
        return "SEAT_NOT_FOUND";
    }
    else if (s == STATUS::ALREADY_BOOKED) {
        return "ALREADY_BOOKED";
    }
    else if (s == STATUS::UNKNOWN_ERROR) {
        return "UNKNOWN_ERROR";
    }
    else if (s == STATUS::WRONG_COMMAND) {
        return "WRONG_COMMAND";
    }
    else {
        return "WRONG_STATUS";
    }
}

STATUS booking::string_to_status(const string& s) {
    if (s == "OK") {
        return STATUS::OK;
    }
    else if (s == "MOVIE_NOT_FOUND") {
        return STATUS::MOVIE_NOT_FOUND;
    }
    else if (s == "THEATER_NOT_FOUND") {
        return STATUS::THEATER_NOT_FOUND;
    }
    else if (s == "SEAT_NOT_FOUND") {
        return STATUS::SEAT_NOT_FOUND;
    }
    else if (s == "ALREADY_BOOKED") {
        return STATUS::ALREADY_BOOKED;
    }
    else if (s == "UNKNOWN_ERROR") {
        return STATUS::UNKNOWN_ERROR;
    }
    else if (s == "WRONG_COMMAND") {
        return STATUS::WRONG_COMMAND;
    }
    else {
        return STATUS::UNKNOWN_ERROR;
    }
}