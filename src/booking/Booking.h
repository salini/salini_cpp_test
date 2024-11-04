#ifndef __BOOKING_BOOKING_H__
#define __BOOKING_BOOKING_H__

#include <string>
#include <vector>

#include "Status.h"
#include "Session.h"

using namespace std;

namespace booking
{

/**
 * The Booking backend
 * 
 * Can get information and book seats for all the available sessions
 */
class Booking {
public:

    /** default contructor */
    Booking();

    /**
     * Initialize the booking backend from a "database" file, a json file
     * this json file reprensents the initial state of all the possible sessions,
     * and it shoud be defined as follow:
     *      {movie_names: {theater_names: {seat_names: occupancy}}}
     * where occupancy is a value in: {0: free, 1: booked}
     * 
     * @param filepath the path to this "database" json file
     */
    Booking(const string& filepath);

    /** destructor */
    virtual ~Booking();

    /**
     * @return the list of all movie names
     */
    const vector<string>& get_movies();

    /**
     * get all theater names that display a particular movie
     * 
     * @param movie the movie that is display in return theaters
     * @return the list of all theater that display the corresponding movie, empty if movie is not found
     */
    const vector<string>& get_theaters(const string& movie);

    /**
     * get all the seat names for a session (movie, theater)
     * 
     * @param movie the movie name to consider
     * @param theater the theater name to consider
     * @param available_only wheter to return all seats, or just the available ones (free, not booked) 
     * @return the corresponding seat names of the session(movie, theater)
     */
    vector<string> get_seat_names(const string& movie, const string& theater, bool available_only);

    /**
     * get all the seat that are available (free, not booked) for a session (movie, theater)
     * 
     * same as `get_seat_names` function, where `available_only` is set to true
     * @param movie the movie name to consider
     * @param theater the theater name to consider
     * @return the corresponding seat that are available for the session (movie, theater)
     */
    vector<string> get_available_seats(const string& movie, const string& theater);

    /**
     * request to book seat for a session (movie, theater)
     * 
     * @param movie the movie name to consider
     * @param theater the theater name to consider
     * @param seat seat name one wants to book
     * @return wheter the booking was performed correctly
     * * OK: seat has been booked properly
     * * MOVIE_NOT_FOUND: cannot find the requested movie in the "database"
     * * THEATER_NOT_FOUND: cannot find the requested theater for the corresponding movie
     * * SEAT_NOT_FOUND: cannot find seat name in the requested session (movie, theater)
     * * ALREADY_BOOKED: seat was not available in the requested session (movie, theater)
     */
    STATUS book_seat(const string& movie, const string& theater, const string& seat);

    /**
     * request to book seat for a session (movie, theater)
     * 
     * @param movie the movie name to consider
     * @param theater the theater name to consider
     * @param seats the list of seat names one wants to book
     * @return wheter the booking was performed correctly
     * * OK: seats have been booked properly
     * * MOVIE_NOT_FOUND: cannot find the requested movie in the "database"
     * * THEATER_NOT_FOUND: cannot find the requested theater for the corresponding movie
     * * SEAT_NOT_FOUND: cannot find seat name in the requested session (movie, theater)
     * * ALREADY_BOOKED: one or more seats were not available in the requested session (movie, theater)
     */
    STATUS book_seats(const string& movie, const string& theater, const vector<string>& seats);


protected:
    vector<string> movies;
    map<string, vector<string>> theaters;
    map<string, map<string, Session*>> movies_sessions;

    vector<string> _not_found;
};

} // namespace booking


#endif