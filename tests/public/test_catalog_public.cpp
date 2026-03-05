#include <gtest/gtest.h>
#include "Catalog.h"

// Helper — builds a catalog with two books
static Catalog makeCatalog() {
    Catalog c;
    c.addBook(Book("9780000000001", "Alpha Book",  "Author A", 2000, 2, "Fiction"));
    c.addBook(Book("9780000000002", "Beta Book",   "Author B", 2005, 0, "Science"));
    return c;
}

// ── Issue #15 — Catalog::findByISBN() returns a book that does NOT match ─────
TEST(Issue15_Public, FindByISBNReturnsCorrectBook) {
    Catalog c = makeCatalog();
    auto result = c.findByISBN("9780000000001");
    ASSERT_TRUE(result.has_value()) << "findByISBN() must return a value for an existing ISBN";
    EXPECT_EQ((*result)->getISBN(), "9780000000001")
        << "findByISBN() must return the book whose ISBN matches the query";
}

TEST(Issue15_Public, FindByISBNReturnsNulloptForMissing) {
    Catalog c = makeCatalog();
    auto result = c.findByISBN("0000000000000");
    EXPECT_FALSE(result.has_value())
        << "findByISBN() must return nullopt when no book has the given ISBN";
}

// ── Issue #16 — Catalog::searchByTitle() is case-sensitive ───────────────────
TEST(Issue16_Public, SearchByTitleIsCaseInsensitive) {
    Catalog c = makeCatalog();
    auto results = c.searchByTitle("alpha book");   // lower-case query
    EXPECT_FALSE(results.empty())
        << "searchByTitle() must find books regardless of query case";
}

// ── Issue #17 — Catalog::addBook() allows duplicate ISBNs ────────────────────
TEST(Issue17_Public, AddBookRejectsDuplicateISBN) {
    Catalog c;
    c.addBook(Book("9780000000001", "Alpha Book", "Author A", 2000, 2));
    c.addBook(Book("9780000000001", "Duplicate",  "Author X", 2010, 1));
    EXPECT_EQ(c.size(), 1)
        << "addBook() must not insert a book when its ISBN already exists in the catalog";
}

// ── Issue #18 — Catalog::removeBook() erases the element AFTER the match ─────
TEST(Issue18_Public, RemoveBookDeletesTheCorrectEntry) {
    Catalog c = makeCatalog();
    bool ok = c.removeBook("9780000000001");
    EXPECT_TRUE(ok) << "removeBook() must return true for an existing ISBN";
    EXPECT_EQ(c.size(), 1);
    // The remaining book must be the one that was NOT removed
    EXPECT_FALSE(c.findByISBN("9780000000001").has_value())
        << "The removed book must no longer be in the catalog";
    EXPECT_TRUE(c.findByISBN("9780000000002").has_value())
        << "The other book must still be in the catalog";
}

// ── Issue #19 — Catalog::sortByTitle() sorts descending instead of ascending ─
TEST(Issue19_Public, SortByTitleProducesAscendingOrder) {
    Catalog c;
    c.addBook(Book("9780000000003", "Zeta",  "Auth", 2000, 1));
    c.addBook(Book("9780000000001", "Alpha", "Auth", 2000, 1));
    c.addBook(Book("9780000000002", "Mango", "Auth", 2000, 1));
    c.sortByTitle();
    auto books = c.getAllBooks();
    ASSERT_EQ(books.size(), 3u);
    EXPECT_EQ(books[0]->getTitle(), "Alpha")
        << "After sortByTitle(), the first element must be the alphabetically smallest title";
    EXPECT_EQ(books[2]->getTitle(), "Zeta");
}

// ── Issue #20 — Catalog::getAvailableBooks() includes books with 0 copies ────
TEST(Issue20_Public, GetAvailableBooksExcludesZeroCopies) {
    Catalog c = makeCatalog();  // second book has 0 copies
    auto avail = c.getAvailableBooks();
    for (auto* b : avail) {
        EXPECT_GT(b->getAvailableCopies(), 0)
            << "getAvailableBooks() must only return books with copies_available > 0";
    }
}
