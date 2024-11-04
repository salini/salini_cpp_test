#ifndef __BOOKING_SERVER_BOOKING_SERVER_H__
#define __BOOKING_SERVER_BOOKING_SERVER_H__

#include <string>

using namespace std;


namespace booking_server
{
    
/**
 * Booking server to handle booking requests from other processes
 * 
 * This server creates a Booking instance internally,
 * and listens to a port defined in constructor 
 * to handle requests from clients
 * 
 * the server accepts the following requests:
 * `get_movies`
 * `get_theaters            movie_name`
 * `get_seat_names          movie_name      theater_name`
 * `get_available_seats     movie_name      theater_name`
 * `book_seats              movie_name      theater_name    seat_name0 seat_name1 seat_name2 ...`
 */
class BookingServer {
public:
    /**
     * Booking server constructor
     * 
     * @param filepath the json "database" to initialize the internal Booking instance
     * @param port the port used to communicate with the server
     * @param simulate_delay_ms simulate some processing time when processing requests (for testing purposes)
     */
    BookingServer(const string& filepath, int port=8888, float simulate_delay_ms=0);

    /** Booking server destructor */
    virtual ~BookingServer();

    /**
     * run booking server in the main thread
     */
    void run_loop();

    /**
     * run booking server in another thread
     * 
     * This is convenient for testing purposes, if one wants to spawn the booking server and the client(s) in the same process.
     */
    void run_loop_in_thread();

    /**
     * Shutdown booking server properly, especially if it run on a separate thread
     */
    void shutdown();

private:
    /**
     * Internal implementation to set communication with ZMQ 
     */
    struct Pimpl;
    Pimpl* p;
};

} // namespace booking_server

#endif

