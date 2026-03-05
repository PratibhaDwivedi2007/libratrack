#include <gtest/gtest.h>
#include "Book.h"

// ── Issue #1 — Book::isAvailable() returns true when no copies remain ─────────
TEST(Issue01_Public, IsAvailableReturnsFalseWhenZeroCopies) {
    Book b("9780000000001", "Test Book", "Author A", 2020, 1);
    b.decrementCopies();          // copies_available_ is now 0
    EXPECT_FALSE(b.isAvailable()) << "isAvailable() must return false when copies_available_ == 0";
}

// ── Issue #2 — Book::validateISBN() accepts a 12-digit string as valid ────────
TEST(Issue02_Public, ValidateISBNRejectsLengthNotThirteen) {
    Book b("978000000000", "Test Book", "Author A", 2020, 1);  // 12 digits
    EXPECT_FALSE(b.validateISBN()) << "validateISBN() must return false for a 12-digit ISBN (ISBN-13 requires 13 digits)";
}

TEST(Issue02_Public, ValidateISBNAcceptsThirteenDigits) {
    Book b("9780000000001", "Test Book", "Author A", 2020, 1);  // 13 digits
    EXPECT_TRUE(b.validateISBN()) << "validateISBN() must return true for a valid 13-digit ISBN";
}

// ── Issue #3 — Book::decrementCopies() allows negative available count ────────
TEST(Issue03_Public, DecrementCopiesDoesNotGoBelowZero) {
    Book b("9780000000001", "Test Book", "Author A", 2020, 1);
    b.decrementCopies();  // goes to 0
    b.decrementCopies();  // must not go to -1
    EXPECT_GE(b.getAvailableCopies(), 0)
        << "decrementCopies() must not allow copies_available_ to go below 0";
}

// ── Issue #4 — Book::setPublicationYear() accepts zero and negative years ─────
TEST(Issue04_Public, SetPublicationYearRejectsZero) {
    Book b("9780000000001", "Test Book", "Author A", 2020, 1);
    b.setPublicationYear(0);
    EXPECT_NE(b.getPublicationYear(), 0)
        << "setPublicationYear() must reject year == 0";
}

TEST(Issue04_Public, SetPublicationYearRejectsNegative) {
    Book b("9780000000001", "Test Book", "Author A", 2020, 1);
    b.setPublicationYear(-500);
    EXPECT_GT(b.getPublicationYear(), 0)
        << "setPublicationYear() must reject negative years";
}

// ── Issue #5 — Book::getFullTitle() returns author before title ───────────────
TEST(Issue05_Public, GetFullTitleFormatIsTitleByAuthor) {
    Book b("9780000000001", "Clean Code", "Robert Martin", 2008, 1);
    std::string full = b.getFullTitle();
    EXPECT_EQ(full, "Clean Code by Robert Martin")
        << "getFullTitle() must return \"<title> by <author>\"";
}
