#include <gtest/gtest.h>
#include "FineCalculator.h"
#include "Loan.h"
#include "Member.h"

// ── Issue #21 — FineCalculator::getDailyRate() returns 0.50 instead of 0.25 ─
TEST(Issue21_Public, DailyRateIsPointTwoFive) {
    FineCalculator fc;
    EXPECT_DOUBLE_EQ(fc.getDailyRate(), 0.25)
        << "getDailyRate() must return 0.25";
}

// ── Issue #22 — calculateFine() subtracts grace days from the fine amount ────
// With 0.25/day rate, 4 overdue days minus 2-day grace = 2 billable days = $0.50
TEST(Issue22_Public, CalculateFineAppliesGracePeriodToOverdueDays) {
    // Create a loan overdue by 4 days
    Loan loan("L001", "9780000000001", "M001", "2026-01-01");
    loan.markReturned("2026-01-22");  // checkout 2026-01-01, due 2026-01-15, returned 2026-01-22 = 7 days overdue

    FineCalculator fc;
    double fine = fc.calculateFine(loan);
    // Expected: (7 - 2) * 0.25 = 1.25
    EXPECT_DOUBLE_EQ(fine, 1.25)
        << "calculateFine() must subtract GRACE_PERIOD_DAYS from overdue_days before multiplying by rate";
}

// ── Issue #23 — getTotalFines() loop condition i <= size causes UB ────────────
TEST(Issue23_Public, GetTotalFinesDoesNotCrash) {
    FineCalculator fc;
    Loan l1("L001", "isbn1", "M001", "2026-01-01");
    Loan l2("L002", "isbn2", "M001", "2026-01-01");
    l1.setFineAmount(1.0);
    l2.setFineAmount(2.0);
    std::vector<Loan> loans = {l1, l2};
    double total = fc.getTotalFines(loans);
    EXPECT_DOUBLE_EQ(total, 3.0)
        << "getTotalFines() must sum fine amounts and not access out-of-bounds elements";
}

// ── Issue #24 — applyDiscount() gives senior discount to STUDENT members ─────
TEST(Issue24_Public, DiscountAppliedToSeniorNotStudent) {
    FineCalculator fc;
    Member senior("M001", "Eve", "Elder", "e@test.com",
                  Member::MemberType::SENIOR, "2030-01-01");
    Member student("M002", "Sam", "Young", "s@test.com",
                   Member::MemberType::STUDENT, "2030-01-01");

    double fine = 4.0;
    double senior_fine  = fc.applyDiscount(fine, senior);
    double student_fine = fc.applyDiscount(fine, student);

    EXPECT_DOUBLE_EQ(senior_fine, 2.0)
        << "applyDiscount() must halve the fine for SENIOR members";
    EXPECT_DOUBLE_EQ(student_fine, 4.0)
        << "applyDiscount() must NOT apply the senior discount to STUDENT members";
}

// ── Issue #25 — getMaxFine() uses min_element instead of max_element ──────────
TEST(Issue25_Public, GetMaxFineReturnsHighestValue) {
    FineCalculator fc;
    Loan l1("L001", "isbn1", "M001", "2026-01-01"); l1.setFineAmount(1.0);
    Loan l2("L002", "isbn2", "M001", "2026-01-01"); l2.setFineAmount(5.0);
    Loan l3("L003", "isbn3", "M001", "2026-01-01"); l3.setFineAmount(3.0);
    std::vector<Loan> loans = {l1, l2, l3};
    EXPECT_DOUBLE_EQ(fc.getMaxFine(loans), 5.0)
        << "getMaxFine() must return the largest fine amount, not the smallest";
}

// ── Issue #26 — hasPendingFines() returns true when total == 0 (inverted) ────
TEST(Issue26_Public, HasPendingFinesReturnsFalseWhenNoFines) {
    FineCalculator fc;
    Loan l1("L001", "isbn1", "M001", "2026-01-01"); l1.setFineAmount(0.0);
    std::vector<Loan> loans = {l1};
    EXPECT_FALSE(fc.hasPendingFines(loans))
        << "hasPendingFines() must return false when no loan has a fine amount > 0";
}

TEST(Issue26_Public, HasPendingFinesReturnsTrueWhenFinesExist) {
    FineCalculator fc;
    Loan l1("L001", "isbn1", "M001", "2026-01-01"); l1.setFineAmount(2.5);
    std::vector<Loan> loans = {l1};
    EXPECT_TRUE(fc.hasPendingFines(loans))
        << "hasPendingFines() must return true when at least one loan has a fine > 0";
}
