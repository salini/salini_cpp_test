#ifndef __BOOKING_SESSION_H__
#define __BOOKING_SESSION_H__

#include "Status.h"

// #include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <map>

// using json = nlohmann::json;
using namespace std;




namespace booking
{

/**
 * Represent a 
 */
class Session {
public:
    Session();

    Session(map<string, int>& seats, const string& theater = "", const string& movie = "");
    Session(Session& s);

    bool seats_exist(const vector<string>& seat_names);
    bool seats_are_available(const vector<string>& seat_names);
    vector<string> get_seat_names(bool available_only);

    bool book_seats(const vector<string>& seat_names);

protected:
    string theater;
    string movie;
    map<string, int> seats;
};

} // namespace booking

#endif
