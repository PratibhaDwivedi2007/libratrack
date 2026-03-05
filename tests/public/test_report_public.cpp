#include <gtest/gtest.h>
#include "ReportGenerator.h"
#include "Loan.h"
#include "Book.h"

// ── Issue #27 — getMostBorrowedBooks() returns least-borrowed books ───────────
TEST(Issue27_Public, GetMostBorrowedBooksReturnsTopByCount) {
    Book b1("isbn1", "Low",  "Auth", 2000, 1); // borrow_count == 0
    Book b2("isbn2", "High", "Auth", 2001, 1);
    for (int i = 0; i < 5; ++i) b2.incrementBorrowCount();

    ReportGenerator rg;
    auto top = rg.getMostBorrowedBooks({b1, b2}, 1);
    ASSERT_EQ(top.size(), 1u);
    EXPECT_EQ(top[0].getTitle(), "High")
        << "getMostBorrowedBooks() must return the book with the HIGHEST borrow count first";
}

// ── Issue #28 — getOverdueReport() returns all loans, not just overdue ones ───
TEST(Issue28_Public, GetOverdueReportOnlyContainsOverdueLoans) {
    // A returned loan is never overdue
    Loan returned("L001", "isbn1", "M001", "2026-01-01");
    returned.markReturned("2026-01-10");

    // A loan checked out in the far future cannot be overdue
    Loan fresh("L002", "isbn2", "M002", "2099-01-01");

    ReportGenerator rg;
    auto report = rg.getOverdueReport({returned, fresh});
    EXPECT_TRUE(report.empty())
        << "getOverdueReport() must only include loans where isOverdue() == true";
}

// ── Issue #29 — generateMonthlyStats() matches month-1 instead of month ──────
TEST(Issue29_Public, GenerateMonthlyStatsMatchesCorrectMonth) {
    Loan march_loan("L001", "isbn1", "M001", "2026-03-10");
    Loan april_loan("L002", "isbn2", "M001", "2026-04-01");

    ReportGenerator rg;
    auto march_results = rg.generateMonthlyStats({march_loan, april_loan}, 3);
    ASSERT_EQ(march_results.size(), 1u)
        << "generateMonthlyStats() with month=3 must return exactly the March loan";
    EXPECT_EQ(march_results[0].getLoanID(), "L001");
}

// ── Issue #30 — formatCurrency() lacks fixed-precision formatting ─────────────
TEST(Issue30_Public, FormatCurrencyShowsTwoDecimalPlaces) {
    ReportGenerator rg;
    EXPECT_EQ(rg.formatCurrency(5.0),  "$5.00")
        << "formatCurrency(5.0) must produce \"$5.00\", not \"$5\" or \"$5.000000\"";
    EXPECT_EQ(rg.formatCurrency(1.5),  "$1.50");
    EXPECT_EQ(rg.formatCurrency(10.25),"$10.25");
}

// ── Issue #31 — countActiveLoans() counts returned loans too ──────────────────
TEST(Issue31_Public, CountActiveLoansExcludesReturnedLoans) {
    Loan active("L001", "isbn1", "M001", "2026-01-01");
    Loan returned("L002", "isbn2", "M001", "2026-01-01");
    returned.markReturned("2026-01-10");

    ReportGenerator rg;
    EXPECT_EQ(rg.countActiveLoans({active, returned}), 1)
        << "countActiveLoans() must not count loans that have already been returned";
}
