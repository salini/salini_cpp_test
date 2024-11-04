#ifndef __BOOKING_SERVER_BOOKING_SERVER_H__
#define __BOOKING_SERVER_BOOKING_SERVER_H__

#include <string>

using namespace std;


namespace booking_server
{
    
class BookingServer {
public:
    BookingServer(const string& filepath, int port=8888, float simulate_delay_ms=0);
    virtual ~BookingServer();

    void run_loop();
    void run_loop_in_thread();
    void shutdown();

private:
    struct Pimpl;
    Pimpl* p;
};

} // namespace booking_server

#endif

