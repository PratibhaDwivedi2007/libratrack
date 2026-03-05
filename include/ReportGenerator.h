#pragma once
#include "Loan.h"
#include "Book.h"
#include "Member.h"
#include <vector>
#include <string>

class ReportGenerator {
public:
    // Returns the top N most-borrowed books (descending borrow count)
    std::vector<Book> getMostBorrowedBooks(const std::vector<Book>& books, int n) const;

    // Returns only loans that are currently overdue
    std::vector<Loan> getOverdueReport(const std::vector<Loan>& loans) const;

    // Returns loans whose checkout month matches `month` (1-based)
    std::vector<Loan> generateMonthlyStats(const std::vector<Loan>& loans, int month) const;

    // Formats a double as a currency string, e.g. "$5.25"
    std::string formatCurrency(double amount) const;

    // Counts loans that are NOT yet returned
    int countActiveLoans(const std::vector<Loan>& loans) const;

    std::string generateSummaryReport(
        const std::vector<Book>&   books,
        const std::vector<Member>& members,
        const std::vector<Loan>&   loans) const;
};
