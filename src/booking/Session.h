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
 * Represent a session
 * 
 * It contains: the seats names and their occupancy, the theater name, the movie name
 */
class Session {
public:
    Session();

    /**
     * Session constructor
     * 
     * @param seats the map {seat_name: occupancy} for this session. 0 means free, 1 means booked (can be extended to other states in the future)
     * @param theater theater name of this session
     * @param movie movie name of this session
     */
    Session(map<string, int>& seats, const string& theater = "", const string& movie = "");

    /** Session copy constructor */
    Session(Session& s);

    /**
     * Check if all the seats exist for this session
     * 
     * @param seat_names list of seats to check, using their names, e.g. {"a1", "a2", "b1"}
     * @return whether all seats exist
     */
    bool seats_exist(const vector<string>& seat_names);

    /**
     * Check if all the seats are available (free, not booked) for this session
     * 
     * @param seat_names list of seats to check, using their names, e.g. {"a1", "a2", "b1"}
     * @return whether all seats are available
     */
    bool seats_are_available(const vector<string>& seat_names);

    /**
     * Get all the names of the seat for this session
     * 
     * @param available_only if true, consider only the seats that are free
     * @return all seat names, or only the available ones if available_only is true
     */
    vector<string> get_seat_names(bool available_only);

    /**
     * book some seats for this sessions
     * 
     * @param seat_names the list of seats to book
     * @return true if all seats were available, and booked properly, false otherwise
     */
    bool book_seats(const vector<string>& seat_names);

protected:
    string theater;
    string movie;
    map<string, int> seats;
};

} // namespace booking

#endif
