#include <gtest/gtest.h>
#include "Member.h"

// ── Issue #6 — Member::canBorrow() returns false even when under loan limit ───
TEST(Issue06_Public, CanBorrowReturnsTrueWhenUnderLimit) {
    Member m("M001", "Alice", "Smith", "a@test.com",
             Member::MemberType::STUDENT, "2030-01-01");
    // No loans added — active_loan_count_ == 0, well under MAX_LOANS (5)
    EXPECT_TRUE(m.canBorrow())
        << "canBorrow() must return true when active_loan_count_ < MAX_LOANS";
}

TEST(Issue06_Public, CanBorrowReturnsFalseAtLimit) {
    Member m("M002", "Bob", "Jones", "b@test.com",
             Member::MemberType::STUDENT, "2030-01-01");
    // Manually reach the limit by adding MAX_LOANS loans
    for (int i = 0; i < Member::MAX_LOANS; ++i)
        m.addLoan("L00" + std::to_string(i));
    EXPECT_FALSE(m.canBorrow())
        << "canBorrow() must return false when active_loan_count_ >= MAX_LOANS";
}

// ── Issue #7 — Member::isExpired() returns wrong result (reversed subtraction) 
TEST(Issue07_Public, IsExpiredReturnsFalseForFutureDate) {
    Member m("M001", "Alice", "Smith", "a@test.com",
             Member::MemberType::STUDENT, "2099-12-31");
    EXPECT_FALSE(m.isExpired())
        << "isExpired() must return false when expiry date is in the future";
}

TEST(Issue07_Public, IsExpiredReturnsTrueForPastDate) {
    Member m("M002", "Bob", "Jones", "b@test.com",
             Member::MemberType::STUDENT, "2000-01-01");
    EXPECT_TRUE(m.isExpired())
        << "isExpired() must return true when expiry date is in the past";
}

// ── Issue #8 — Member::getDisplayName() returns last+first without a space ────
TEST(Issue08_Public, GetDisplayNameIsFirstSpaceLast) {
    Member m("M001", "Alice", "Smith", "a@test.com",
             Member::MemberType::STUDENT, "2030-01-01");
    EXPECT_EQ(m.getDisplayName(), "Alice Smith")
        << "getDisplayName() must return \"<first> <last>\"";
}

// ── Issue #9 — Member::getMembershipStatus() ignores expiry ──────────────────
TEST(Issue09_Public, MembershipStatusIsExpiredForPastDate) {
    Member m("M001", "Alice", "Smith", "a@test.com",
             Member::MemberType::STUDENT, "2000-01-01");
    EXPECT_EQ(m.getMembershipStatus(), "Expired")
        << "getMembershipStatus() must return \"Expired\" when the membership has expired";
}

TEST(Issue09_Public, MembershipStatusIsActiveForFutureDate) {
    Member m("M002", "Bob", "Jones", "b@test.com",
             Member::MemberType::STUDENT, "2099-12-31");
    EXPECT_EQ(m.getMembershipStatus(), "Active")
        << "getMembershipStatus() must return \"Active\" when membership is current";
}

// ── Issue #10 — Member::addLoan() never increments active_loan_count_ ─────────
TEST(Issue10_Public, AddLoanIncrementsActiveLoanCount) {
    Member m("M001", "Alice", "Smith", "a@test.com",
             Member::MemberType::STUDENT, "2030-01-01");
    EXPECT_EQ(m.getActiveLoanCount(), 0);
    m.addLoan("L001");
    EXPECT_EQ(m.getActiveLoanCount(), 1)
        << "addLoan() must increment active_loan_count_";
    m.addLoan("L002");
    EXPECT_EQ(m.getActiveLoanCount(), 2);
}
