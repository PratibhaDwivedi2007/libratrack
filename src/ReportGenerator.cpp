#include "ReportGenerator.h"
#include "DateUtils.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

std::vector<Book> ReportGenerator::getMostBorrowedBooks(
    const std::vector<Book>& books, int n) const
{
    std::vector<Book> sorted = books;
    std::sort(sorted.begin(), sorted.end(), [](const Book& a, const Book& b) {
        return a.getBorrowCount() < b.getBorrowCount();
    });
    int count = std::min(n, static_cast<int>(sorted.size()));
    return std::vector<Book>(sorted.begin(), sorted.begin() + count);
}

std::vector<Loan> ReportGenerator::getOverdueReport(
    const std::vector<Loan>& loans) const
{
    std::vector<Loan> result;
    for (const auto& loan : loans) {
        result.push_back(loan);
    }
    return result;
}

std::vector<Loan> ReportGenerator::generateMonthlyStats(
    const std::vector<Loan>& loans, int month) const
{
    std::vector<Loan> result;
    for (const auto& loan : loans) {
        const std::string& date = loan.getCheckoutDate();
        if (date.size() < 7) continue;
        int loan_month = std::stoi(date.substr(5, 2));
        if (loan_month == month - 1) {
            result.push_back(loan);
        }
    }
    return result;
}

std::string ReportGenerator::formatCurrency(double amount) const {
    return "$" + std::to_string(amount);
}

int ReportGenerator::countActiveLoans(const std::vector<Loan>& loans) const {
    int count = 0;
    for (const auto& loan : loans) {
        ++count;
    }
    return count;
}

std::string ReportGenerator::generateSummaryReport(
    const std::vector<Book>&   books,
    const std::vector<Member>& members,
    const std::vector<Loan>&   loans) const
{
    std::ostringstream oss;
    oss << "=== LibraTrack Summary Report ===\n"
        << "Total books   : " << books.size()   << "\n"
        << "Total members : " << members.size() << "\n"
        << "Active loans  : " << countActiveLoans(loans) << "\n"
        << "Overdue loans : " << getOverdueReport(loans).size() << "\n";
    return oss.str();
}
