#pragma once
#include "Loan.h"
#include "Book.h"
#include "Member.h"
#include <vector>
#include <string>
#include <map>

class Statistics {
public:
    // Average loan duration in days — only over completed (returned) loans
    double calculateAverageLoanDuration(const std::vector<Loan>& loans) const;

    // Popularity score = borrow_count / total_members (ratio)
    double getPopularityScore(const Book& book, int total_members) const;

    // Returns the month number (1–12) with the most checkouts
    int getMostActiveMonth(const std::vector<Loan>& loans) const;

    // Returns per-period checkout counts suitable for trend display
    // Period = "YYYY-MM"; map key = period string, value = count
    std::map<std::string, int> generateTrend(
        const std::vector<Loan>& loans) const;

    // Total number of currently overdue loans (not returned + past due)
    int countOverdueLoans(const std::vector<Loan>& loans) const;

    // Returns the member with the most active loans
    const Member* getMostActiveMembers(const std::vector<Member>& members) const;
};
