
import zmq

def main_client(request, host="localhost", port = 8888):
    context = zmq.Context()
    print("Connecting to server...")
    socket = context.socket(zmq.REQ)
    socket.connect(f"tcp://{host}:{port}")

    print("Sending request:", request)
    socket.send(request.encode())
    #  Get the reply.
    message = socket.recv().decode()
    reply = message.split()

    print("Received status", reply[0])
    print("Received message", reply[1:])


if __name__ == "__main__":
    main_client("get_movies")
    main_client("get_theaters Movie1")
    main_client("get_theaters NG")  # expect MOVIE_NOT_FOUND
    main_client("get_seat_names Movie1 Theater1")
    main_client("get_seat_names Movie1 NG")  # expect THEATER_NOT_FOUND
    main_client("get_available_seats Movie1 Theater1")
    main_client("book_seats Movie1 Theater1 NG")  # expected SEAT_NOT_FOUND
    main_client("book_seats Movie1 Theater1 a1 a2 b1")
    main_client("book_seats Movie1 Theater1 b1")  # expect ALREADY_BOOKED
