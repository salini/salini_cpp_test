
add_executable(tests
    main_tests.cpp
)

target_link_libraries(tests
    PRIVATE
    booking
    booking_server
)
