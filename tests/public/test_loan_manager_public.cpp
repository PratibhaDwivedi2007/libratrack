#include <gtest/gtest.h>
#include "LoanManager.h"
#include "Catalog.h"
#include "Member.h"
#include "DateUtils.h"

// Helper: a catalog with one available book and a vector with one active member
static void makeEnv(Catalog& cat, std::vector<Member>& members) {
    cat.addBook(Book("9780000000001", "Test Book", "Author A", 2020, 2));
    members.emplace_back("M001", "Alice", "Smith", "a@test.com",
                         Member::MemberType::STUDENT, "2099-12-31");
}

// ── Issue #32 — checkoutBook() never calls book.decrementCopies() ─────────────
TEST(Issue32_Public, CheckoutDecrementsAvailableCopies) {
    Catalog cat; std::vector<Member> members;
    makeEnv(cat, members);
    LoanManager lm(cat, members);

    int before = (*cat.findByISBN("9780000000001"))->getAvailableCopies();
    lm.checkoutBook("9780000000001", "M001", "2026-03-01");
    int after  = (*cat.findByISBN("9780000000001"))->getAvailableCopies();

    EXPECT_EQ(after, before - 1)
        << "checkoutBook() must call decrementCopies() on the book";
}

// ── Issue #33 — returnBook() never calls book.incrementCopies() ───────────────
TEST(Issue33_Public, ReturnBookIncrementsAvailableCopies) {
    Catalog cat; std::vector<Member> members;
    makeEnv(cat, members);
    LoanManager lm(cat, members);

    std::string lid = lm.checkoutBook("9780000000001", "M001", "2026-03-01");
    int copies_after_checkout = (*cat.findByISBN("9780000000001"))->getAvailableCopies();
    lm.returnBook(lid, "2026-03-10");
    int copies_after_return   = (*cat.findByISBN("9780000000001"))->getAvailableCopies();

    EXPECT_EQ(copies_after_return, copies_after_checkout + 1)
        << "returnBook() must call incrementCopies() on the book";
}

// ── Issue #34 — renewLoan() extends by 7 days instead of 14 ──────────────────
TEST(Issue34_Public, RenewLoanExtendsByFourteenDays) {
    Catalog cat; std::vector<Member> members;
    makeEnv(cat, members);
    LoanManager lm(cat, members);

    std::string lid = lm.checkoutBook("9780000000001", "M001", "2026-03-01");
    Loan* loan      = lm.findLoan(lid);
    ASSERT_NE(loan, nullptr);
    std::string original_due = loan->getDueDate();  // 2026-03-15

    lm.renewLoan(lid);

    // After renewal a new loan record exists with the extended period
    // The renewed loan should have a due date 14 days from the original due date
    std::string renewed_due = DateUtils::addDays(original_due, 14);
    Loan* renewed = lm.findLoan(lid + "R");
    ASSERT_NE(renewed, nullptr) << "renewLoan() must create a renewed loan record";
    EXPECT_EQ(renewed->getDueDate(), renewed_due)
        << "renewLoan() must extend the due date by 14 days, not 7";
}

// ── Issue #35 — getActiveLoans() returns returned loans too ──────────────────
TEST(Issue35_Public, GetActiveLoansExcludesReturnedLoans) {
    Catalog cat; std::vector<Member> members;
    makeEnv(cat, members);
    LoanManager lm(cat, members);

    std::string lid = lm.checkoutBook("9780000000001", "M001", "2026-03-01");
    lm.returnBook(lid, "2026-03-10");

    auto active = lm.getActiveLoans();
    for (const auto& l : active) {
        EXPECT_FALSE(l.isReturned())
            << "getActiveLoans() must not include loans that have been returned";
    }
}

// ── Issue #36 — checkoutBook() ignores member.canBorrow() limit ───────────────
TEST(Issue36_Public, CheckoutRejectedWhenMemberAtLoanLimit) {
    Catalog cat; std::vector<Member> members;
    cat.addBook(Book("isbn1","B1","A",2000,10));
    cat.addBook(Book("isbn2","B2","A",2000,10));
    cat.addBook(Book("isbn3","B3","A",2000,10));
    cat.addBook(Book("isbn4","B4","A",2000,10));
    cat.addBook(Book("isbn5","B5","A",2000,10));
    cat.addBook(Book("isbn6","B6","A",2000,10));
    members.emplace_back("M001", "Alice", "Smith", "a@test.com",
                         Member::MemberType::STUDENT, "2099-12-31");
    LoanManager lm(cat, members);

    // Checkout up to the limit
    for (int i = 1; i <= Member::MAX_LOANS; ++i)
        lm.checkoutBook("isbn" + std::to_string(i), "M001", "2026-03-01");

    // The 6th checkout must fail
    std::string extra = lm.checkoutBook("isbn6", "M001", "2026-03-01");
    EXPECT_TRUE(extra.empty())
        << "checkoutBook() must return empty string when member has reached MAX_LOANS";
}

// ── Issue #37 — returnBook() never records a fine via FineCalculator ──────────
TEST(Issue37_Public, ReturnBookCalculatesAndRecordsFine) {
    Catalog cat; std::vector<Member> members;
    makeEnv(cat, members);
    LoanManager lm(cat, members);

    // Checkout on a date far enough in the past to be overdue
    std::string lid = lm.checkoutBook("9780000000001", "M001", "2025-01-01");
    lm.returnBook(lid, "2026-03-01");  // returned very late

    Loan* loan = lm.findLoan(lid);
    ASSERT_NE(loan, nullptr);
    EXPECT_GT(loan->getFineAmount(), 0.0)
        << "returnBook() must calculate and store the fine when a loan is returned overdue";
}

// ── Issue #59 — checkoutBook() never calls member.addLoan() ──────────────────
TEST(Issue59_Public, CheckoutUpdatesActiveLoanCountOnMember) {
    Catalog cat; std::vector<Member> members;
    makeEnv(cat, members);
    LoanManager lm(cat, members);

    EXPECT_EQ(members[0].getActiveLoanCount(), 0);
    lm.checkoutBook("9780000000001", "M001", "2026-03-01");
    EXPECT_EQ(members[0].getActiveLoanCount(), 1)
        << "checkoutBook() must call member.addLoan() so that active_loan_count_ is updated";
}
