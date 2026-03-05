#include <gtest/gtest.h>
#include "Catalog.h"
#include "LoanManager.h"
#include "Member.h"

// ── Issue #60 — Catalog::removeBook() leaves orphaned active loans ────────────
//
// When a book is removed from the catalog while it has active loans outstanding,
// those loan records should either be cancelled or flagged as invalid. Currently
// removeBook() only deletes the catalog entry and does nothing about loans.

TEST(Issue60_Public, RemoveBookFlagsOrCancelsActiveLoans) {
    Catalog cat;
    cat.addBook(Book("9780000000001", "Test Book", "Author A", 2020, 3));

    std::vector<Member> members;
    members.emplace_back("M001", "Alice", "Smith", "a@test.com",
                         Member::MemberType::STUDENT, "2099-12-31");

    LoanManager lm(cat, members);
    std::string lid = lm.checkoutBook("9780000000001", "M001", "2026-03-01");
    ASSERT_FALSE(lid.empty()) << "Precondition: checkout must succeed";

    // Now remove the book while the loan is still active
    cat.removeBook("9780000000001");

    // The loan for the removed book should be cancelled/flagged — not still active
    auto active = lm.getActiveLoans();
    for (const auto& loan : active) {
        EXPECT_NE(loan.getBookISBN(), "9780000000001")
            << "After removeBook(), active loans for that ISBN must be cancelled or flagged";
    }
}

TEST(Issue60_Public, RemoveBookDoesNotAffectUnrelatedLoans) {
    Catalog cat;
    cat.addBook(Book("9780000000001", "Book One", "Auth", 2020, 2));
    cat.addBook(Book("9780000000002", "Book Two", "Auth", 2020, 2));

    std::vector<Member> members;
    members.emplace_back("M001", "Alice", "Smith", "a@test.com",
                         Member::MemberType::STUDENT, "2099-12-31");
    members.emplace_back("M002", "Bob",   "Jones", "b@test.com",
                         Member::MemberType::STUDENT, "2099-12-31");

    LoanManager lm(cat, members);
    lm.checkoutBook("9780000000001", "M001", "2026-03-01");  // will be orphaned
    std::string lid2 = lm.checkoutBook("9780000000002", "M002", "2026-03-01");

    cat.removeBook("9780000000001");

    // Loan for book 2 must still be active
    auto active = lm.getActiveLoans();
    bool found = false;
    for (const auto& l : active) {
        if (l.getLoanID() == lid2) found = true;
    }
    EXPECT_TRUE(found)
        << "Removing one book must not accidentally cancel loans for other books";
}
