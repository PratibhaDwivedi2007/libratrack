#pragma once
#include <string>
#include <ctime>

class DateUtils {
public:
    // Adds `days` to `date` (format: "YYYY-MM-DD") and returns the new date string
    static std::string addDays(const std::string& date, int days);

    // Returns the number of calendar days from date1 to date2 (date2 - date1)
    static int daysBetween(const std::string& date1, const std::string& date2);

    // Returns true if the given year is a leap year
    static bool isLeapYear(int year);

    // Formats a std::tm as "YYYY-MM-DD"
    static std::string formatDate(const std::tm& tm);

    // Parses "YYYY-MM-DD" into a std::tm
    static std::tm parseDate(const std::string& date);

    // Returns today's date as "YYYY-MM-DD"
    static std::string today();
};
