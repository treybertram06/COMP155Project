//
// Created by treyb on 2025-03-18.
//

#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

static int current_id = 0;

//source for time stuff: https://cplusplus.com/reference/ctime/tm/, https://www.geeksforgeeks.org/date-and-time-parsing-in-cpp/
std::string int_to_date(int date) {

    tm start_date = {};
    start_date.tm_year = 2025 - 1900; //date is relative to jan 1st 2025
    start_date.tm_mon = 0;
    start_date.tm_mday = 1;

    time_t start_time = mktime(&start_date); //convert start time to time_t (seconds)
    time_t future_time = start_time + (date * 86400); //add days, 86400 seconds in a day
    tm* future_date = localtime(&future_time); //convert back

    std::stringstream date_stream;
    date_stream << std::put_time(future_date, "%B %d, %Y");  //%B = month name, %d = day, %Y = year

    return date_stream.str();
}

int date_to_int(std::string date) {
    tm date_tm = {};
    std::istringstream date_stream(date);

    date_stream >> std::get_time(&date_tm, "%m/%d/%Y");

    if (date_stream.fail()) {
        throw std::invalid_argument("Invalid date format. Use 'MM/DD/YYYY'.");
    }

    tm start_date = {};
    start_date.tm_year = 2025 - 1900;
    start_date.tm_mon = 0;
    start_date.tm_mday = 1;

    time_t start_time = mktime(&start_date);
    time_t target_time = mktime(&date_tm);

    if (target_time == -1 || start_time == -1) {
        throw std::runtime_error("Error converting date to time.");
    }

    double seconds_diff = difftime(target_time, start_time);
    return static_cast<int>(seconds_diff / 86400); // Convert seconds to days
}

int generate_ID() {

    return current_id++;
}


#endif //HELPER_H
