
add_subdirectory(booking)
add_subdirectory(booking_server)


add_executable(start_booking_server
start_booking_server.cpp
)

target_link_libraries(start_booking_server
    booking_server
)



add_executable(start_booking_client
start_booking_client.cpp
)

target_link_libraries(start_booking_client
    booking_server
)