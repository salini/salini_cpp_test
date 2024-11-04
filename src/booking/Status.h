#ifndef __BOOKING_STATUS_H__
#define __BOOKING_STATUS_H__

#include <string>

using namespace std;


namespace booking {

/**
 * represent the status of a booking request
*/
enum STATUS {
  OK,
  MOVIE_NOT_FOUND,
  THEATER_NOT_FOUND,
  SEAT_NOT_FOUND,
  ALREADY_BOOKED,
  UNKNOWN_ERROR,
  WRONG_COMMAND
}; 

/**
 * convert STATUS enum into a string
 * 
 * @param status Status to convert
 * @return converted `status`
 */
string status_to_string(STATUS status);

/**
 * convert string into a STATUS enum
 * 
 * @param str the string representing the STATUS enum
 * @return the corresponding STATUS enum
 */
STATUS string_to_status(const string& str);

} // namespace booking

#endif