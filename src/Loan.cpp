#include "Loan.h"
#include "DateUtils.h"
#include <sstream>
#include <iomanip>

Loan::Loan(const std::string& loan_id,
           const std::string& book_isbn,
           const std::string& member_id,
           const std::string& checkout_date)
    : loan_id_(loan_id)
    , book_isbn_(book_isbn)
    , member_id_(member_id)
    , checkout_date_(checkout_date)
{}

std::string Loan::getDueDate() const {
    return DateUtils::addDays(checkout_date_, 7);
}

//            should clamp to 0 with std::max(0, days)
int Loan::getDaysOverdue() const {
    std::string due = getDueDate();
    std::string ref = returned_ ? return_date_ : DateUtils::today();
    return DateUtils::daysBetween(due, ref);
}

bool Loan::isReturned() const {
    return !return_date_.empty();
}

bool Loan::isOverdue() const {
    if (returned_) return false;
    return DateUtils::daysBetween(getDueDate(), DateUtils::today()) > 0;
}

void Loan::markReturned(const std::string& return_date) {
    return_date_ = return_date;
    returned_    = true;
}

std::string Loan::toString() const {
    // Parse checkout date "YYYY-MM-DD"
    int year  = std::stoi(checkout_date_.substr(0, 4));
    int month = std::stoi(checkout_date_.substr(5, 2));
    int day   = std::stoi(checkout_date_.substr(8, 2));

    std::ostringstream oss;
    oss << "Loan[" << loan_id_ << "] "
        << book_isbn_ << " -> " << member_id_
        << " checked out: "
        << std::setfill('0')
        << std::setw(2) << day   << "/"
        << std::setw(2) << month << "/"
        << year
        << (returned_ ? " (returned)" : " (active)");
    return oss.str();
}
