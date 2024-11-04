#include "BookingClient.h"
#include "string_operations.h"

#include <string>
#include <zmq.hpp>

#include <spdlog/spdlog.h>

using namespace std;
using namespace booking_server;

struct BookingClient::Pimpl {
    string host;
    int port;
    zmq::context_t ctx;
    zmq::socket_t socket; 

    vector<string> reply;

    Pimpl(const string& _host, int _port)
        : host(_host)
        , port(_port)
    {
        spdlog::debug("initialize client zmq");
        socket = zmq::socket_t(ctx, zmq::socket_type::req);

        string binding_address = string("tcp://") + host + ":" + to_string(port);
        socket.connect(binding_address.c_str());
    }

    ~Pimpl() {
    }
};

BookingClient::BookingClient(const string& host, int port)
    : p(new Pimpl(host, port))
{

}

BookingClient::~BookingClient() {
    delete p;
}

STATUS BookingClient::send_request(const string& request) {
    spdlog::info("client send request: {}", request);
    p->socket.send(zmq::buffer(request), zmq::send_flags::none);

    zmq::message_t  message;
    auto ret = p->socket.recv(message, zmq::recv_flags::none);
    if (ret.has_value() && (EAGAIN == ret.value())) {
        return STATUS::UNKNOWN_ERROR;
    }

    string reply = message.to_string();
    spdlog::info("client receive reply: {}", reply);

    vector<string> splitted = split_string(reply);
    p->reply = vector<string>(splitted.begin()+1, splitted.end());
    return string_to_status(splitted[0]);
}

vector<string> BookingClient::get_last_reply() {
    vector<string> reply = p->reply;
    p->reply.clear();
    return reply;
}