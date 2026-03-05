#include <gtest/gtest.h>
#include "Loan.h"
#include "DateUtils.h"

// ── Issue #11 — Loan::getDaysOverdue() can return negative when not overdue ───
TEST(Issue11_Public, GetDaysOverdueIsNeverNegative) {
    // Loan checked out today — it cannot be overdue yet
    std::string today = DateUtils::today();
    Loan loan("L001", "9780000000001", "M001", today);
    EXPECT_GE(loan.getDaysOverdue(), 0)
        << "getDaysOverdue() must return 0 (not negative) when the loan is not yet overdue";
}

// ── Issue #12 — Loan::isReturned() checks return_date string instead of flag ─
TEST(Issue12_Public, IsReturnedFalseBeforeReturn) {
    Loan loan("L001", "9780000000001", "M001", "2026-01-01");
    EXPECT_FALSE(loan.isReturned())
        << "isReturned() must return false for a loan that has not been returned";
}

TEST(Issue12_Public, IsReturnedTrueAfterMarkReturned) {
    Loan loan("L001", "9780000000001", "M001", "2026-01-01");
    loan.markReturned("2026-01-10");
    EXPECT_TRUE(loan.isReturned())
        << "isReturned() must return true after markReturned() is called";
}

// ── Issue #13 — Loan::getDueDate() adds 7 days instead of 14 ─────────────────
TEST(Issue13_Public, DueDateIsFourteenDaysAfterCheckout) {
    Loan loan("L001", "9780000000001", "M001", "2026-03-01");
    std::string due = loan.getDueDate();
    EXPECT_EQ(due, "2026-03-15")
        << "getDueDate() must return checkout_date + 14 days (standard loan period)";
}

// ── Issue #14 — Loan::toString() prints day/month in wrong order ──────────────
TEST(Issue14_Public, ToStringUsesMMDDYYYYOrder) {
    // Checkout on 2026-03-05 — month=03, day=05
    Loan loan("L001", "9780000000001", "M001", "2026-03-05");
    std::string s = loan.toString();
    // The date portion must appear as 03/05/2026 (MM/DD/YYYY), not 05/03/2026
    EXPECT_NE(s.find("03/05/2026"), std::string::npos)
        << "toString() must format the checkout date as MM/DD/YYYY (found: " + s + ")";
}
