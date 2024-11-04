#ifndef __BOOKING_SERVER_BOOKING_CLIENT_H__
#define __BOOKING_SERVER_BOOKING_CLIENT_H__

#include "booking/Status.h"
#include <vector>
#include <string>

using namespace booking;
using namespace std;

namespace booking_server {

class BookingClient {
public:
    BookingClient(const string& host, int port);
    virtual ~BookingClient();

    STATUS send_request(const string& request);
    vector<string> get_last_reply();

private:
    struct Pimpl;
    Pimpl* p;
};

} // namespace booking_server

#endif