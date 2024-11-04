# salini_cpp_test

This software is an exercice to implement a booking backend service in C++ for booking tickets.

## structure

This package contains the following folders:

* ext: external dependencies, header-only libraries
* python: some python script files, to create some test files and communicate with server
* src: the main source code, containing the source for the booking libraries and executables
* test: contains the test collections


## build

This has been tested on WSL, with Ubuntu 24.04

### install WSL (optional, if running on windows)

reference: https://learn.microsoft.com/en-us/windows/wsl/install

on windows, open a terminal and run:
```
wsl --install
```

follow this instructions to create you account. When it is done, run WSL by running in a terminal:
```
wsl
```

This will open a terminal in Ubuntu.
Note that you can access the file on Windows, directly from Ubuntu.
For instance, if a folder is located in `C:/path/to/folder` on Widows, it will be located in `/mnt/c/path/to/folder/ on Ubuntu.

### install requirements

it is needed install build-essential (c++ compiler) and cmake (the build tool)
```
sudo apt-get update
sudo apt-get install build-essential cmake
```

if the development is done with VSCode, it may be interesting to install ninja:
```
sudo apt-get install ninja-build
```

it is then needed to install some dependencies, ZeroMQ, for interprocess communication:
```
sudo apt-get install pkg-config libzmq3-dev
```

Note that other dependencies are included in the `ext` folder.
They are header-only libraries and are included in this package for simplicity:

* parse arguments: https://github.com/p-ranav/argparse
* to read json files: https://github.com/nlohmann/json
* to log information: https://github.com/gabime/spdlog
* for unit tests: https://github.com/log4cplus/Catch

### build package

when all dependencies are installed, it is possible to build the software.
To do so, in the root folder, open a terminal and build the software in a `build` folder:

```
mkdir build
cd build
cmake ..
make
```

## run test

To check if all unit tests pass, from the root folder, run the commands:
```
cd build/test
./tests
```

it is expected to get the following line at the end of the tests:
```
All tests passed (27 assertions in 3 test cases)
```

## run booking server

to run the booking server, from the root folder, run the commands:
```
cd build/src
./start_booking_server
```

to get more information on the possible arguments, please run:
```
./start_booking_server -h
```

it is expected to get the following line, showing that the server is ready to accept request from clients:
```
[info] start booking server
```

Note that the default "database" has been generated in python using the script `python/create_db.py`.
This default "database" is saved in `test/test_movie_sessions.json`.
Of course, it is possible to write another database, and to use it with the argument `--filepath`.

Note that the booking server accept only one request at a time, it should not be possible to do any overbooking.
This is tested in the TEST_CASE: "Check simultaneous requests"

## run booking client

Now that the server is ready, any program can communicate with the server by using ZeroMQ.
Here, a simple client has been developed to test and communicate with the server.
To run the booking client, open another terminal and, from the root folder, run the commands:
```
cd build/src
./start_booking_client
```

By default, the client will send an empty command, that cannot be processed by the server.
It is expected to receive the reply:
```
[info] client receive reply: WRONG_COMMAND
```

to get more information on the possible arguments, please run:
```
./start_booking_client -h
```

There are 3 arguments:

* `host` and `port`: they are used to set the ZeroMQ socket communication with the server
* `request`: it is the request to get information on the possible sessions and to book seats


### request to book seats

To send request to the booking server, one should use write the client as follows:
```
./start_booking_client --request <request_ arguments>
```

To get information, `<request_arguments>` can be the following:

* `./start_booking_client --request get_movies`: print the movies in the database
* `./start_booking_client --request get_theaters <movie_name>`: print the theaters that display the movie
	* return MOVIE_NOT_FOUND if movie is not in the database
* `./start_booking_client --request get_seat_names <movie_name> <theater_name>`: print all the seats in the theater for this movie
	* return THEATER_NOT_FOUND if theater does not display the movie
* `./start_booking_client --request get_available_seats <movie_name> <theater_name>`: print available seats in the theater for this movie
	* return THEATER_NOT_FOUND if theater does not display the movie

To book seats, `<request_arguments>` can be the following:

* `./start_booking_client --request book_seats <movie_name> <theater_name> <seat0> <seat1> <seat2> ...`: book the number of seats in the sequence
	* if all is ok, return OK
	* if the seat is already booked, return ALREADY_BOOKED
	* if the seat has the wrong name, return SEAT_NOT_FOUND


## run booking client in python

Because the communication is interprocess, it is possible to send this request from another program that the one generated in C++.
For instance, a small python script has been written to emulate the booking client `python/main_client.py`

it is necessary to install pyzmq to communicate with the server:
```
pip install pyzmq
```

Then one can run it:
```
python main_client.py
```

Note that this script does not need to run in Ubuntu. It can run Windows while the server is running on WSL/Ubuntu.


## run GUI booking client 

To check if it was possible to communicate with the booking server with a GUI, a GUI booking client has been created in python `python/main_client_gui.py`

it is necessary to install pyside6 to get the GUI:
```
pip install PySide6
```

Then one can run it:
```
python main_client_gui.py
```


## Notes

* For now, the bookings are available while the server is running, but the modifications are not saved in the json "database". Meaning that when the server is restarted, the booking states are reset to the value in the json "database".


## Reflection

### What aspect of this exercise did you find the most interesting?

During this exercice, the interesting part was to develop the algorithms and the unit tests to validate the concepts gradually.
It was also interesting to get more and more ideas on how to interact the the booking backend, inside the same process, outside the process, with another program/language.

### What did you find most cumbersome?

I would say that the setup the of the Booking server gave a bit more trouble than other parts in the program.
Even if there is a unit test to handle multiple requests simultaneously, it requires more time to have a good stress test on network issues (large delay, disconnection, etc.)
