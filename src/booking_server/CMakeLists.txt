
add_library(booking_server
    BookingServer.h
    BookingServer.cpp
    BookingClient.h
    BookingClient.cpp
    string_operations.h
    string_operations.cpp
)

## add the include directory to our compile directives
target_include_directories(booking_server
    PUBLIC
    ${ZeroMQ_INCLUDE_DIR}
)
## at the 0mq library to our link directive
target_link_libraries(booking_server
    PUBLIC
    ${ZeroMQ_LIBRARY}
    PRIVATE
    booking
)