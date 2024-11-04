#ifndef __BOOKING_SERVER_BOOKING_CLIENT_H__
#define __BOOKING_SERVER_BOOKING_CLIENT_H__

#include "booking/Status.h"
#include <vector>
#include <string>

using namespace booking;
using namespace std;

namespace booking_server {

/**
 * Booking client to get information and book seats on the possible sessions
 */
class BookingClient {
public:
    /**
     * Booking client constructor
     * 
     * @param host the name of the host machine that run the booking server
     * @param port the port on the host machine to communicate with the booking server
     */
    BookingClient(const string& host, int port);

    /** Booking client destructor */
    virtual ~BookingClient();

    /**
     * Send a request to the booking server
     * 
     * 
     * the server accepts the following requests:
     * `get_movies`
     * `get_theaters            movie_name`
     * `get_seat_names          movie_name      theater_name`
     * `get_available_seats     movie_name      theater_name`
     * `book_seats              movie_name      theater_name    seat_name0 seat_name1 seat_name2 ...`
     * 
     * @param request the "request sentence" to send to the server
     * @return the status of the request. Check output from `booking::Booking::book_seats`
     */
    STATUS send_request(const string& request);
    vector<string> get_last_reply();

private:
    /**
     * Internal implementation to set communication with ZMQ 
     */
    struct Pimpl;
    Pimpl* p;
};

} // namespace booking_server

#endif