#ifndef __BOOKING_BOOKING_H__
#define __BOOKING_BOOKING_H__

#include <string>
#include <vector>

#include "Status.h"
#include "Session.h"

using namespace std;

namespace booking
{

class Booking {
public:
    Booking() {}
    Booking(const string& filepath);

    virtual ~Booking();

    const vector<string>& get_movies();
    const vector<string>& get_theaters(const string& movie);
    vector<string> get_seat_names(const string& movie, const string& theater, bool available_only);
    vector<string> get_available_seats(const string& movie, const string& theater);

    STATUS book_seat(const string& movie, const string& theater, const string& seat);
    STATUS book_seats(const string& movie, const string& theater, const vector<string>& seats);


protected:
    vector<string> movies;
    map<string, vector<string>> theaters;
    map<string, map<string, Session*>> movies_sessions;

    vector<string> _not_found;
};

} // namespace booking


#endif