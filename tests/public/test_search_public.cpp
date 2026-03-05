#include <gtest/gtest.h>
#include "SearchEngine.h"
#include "Book.h"
#include <vector>

static std::vector<Book> makeBooks() {
    return {
        Book("isbn1", "Clean Code",       "Robert Martin", 2008, 1, "Programming"),
        Book("isbn2", "Design Patterns",  "Gang of Four",  1994, 1, "Programming"),
        Book("isbn3", "The Pragmatic Programmer", "David Thomas", 1999, 1, "programming"),
        Book("isbn4", "Refactoring",      "Martin Fowler", 2018, 1, "Programming"),
    };
}

// ── Issue #47 — SearchEngine::fuzzyMatch() threshold 0.9 is too strict ────────
TEST(Issue47_Public, FuzzyMatchFindsNearMatchesAtSeventyPercent) {
    auto books = makeBooks();
    SearchEngine se(books);
    // "Clean Cod" is ~89% similar to "Clean Code" — must be found at 0.7 threshold
    auto results = se.fuzzyMatch("Clean Cod");
    EXPECT_FALSE(results.empty())
        << "fuzzyMatch() must find books with similarity >= 0.7; threshold 0.9 is too strict";
}

// ── Issue #48 — SearchEngine::searchByAuthor() ignores first name ─────────────
TEST(Issue48_Public, SearchByAuthorMatchesFirstName) {
    auto books = makeBooks();
    SearchEngine se(books);
    auto results = se.searchByAuthor("Robert");  // first name
    EXPECT_FALSE(results.empty())
        << "searchByAuthor() must search both first and last name, not only the last name";
}

TEST(Issue48_Public, SearchByAuthorMatchesLastName) {
    auto books = makeBooks();
    SearchEngine se(books);
    auto results = se.searchByAuthor("Martin");  // last name
    // Both "Robert Martin" and "Martin Fowler" share this token
    EXPECT_GE(results.size(), 1u);
}

// ── Issue #49 — SearchEngine::filterByGenre() is case sensitive ───────────────
TEST(Issue49_Public, FilterByGenreIsCaseInsensitive) {
    auto books = makeBooks();
    SearchEngine se(books);
    // Catalog has "Programming" and "programming" — both should match
    auto results = se.filterByGenre("programming");
    EXPECT_EQ(results.size(), 4u)
        << "filterByGenre() must be case-insensitive (\"programming\" must match \"Programming\")";
}

// ── Issue #50 — searchByYearRange() excludes boundary years ──────────────────
TEST(Issue50_Public, SearchByYearRangeIncludesBoundaryYears) {
    auto books = makeBooks();
    SearchEngine se(books);
    // 2008 and 1994 are boundary values in this data set
    auto results = se.searchByYearRange(1994, 2018);
    // All 4 books fall within [1994, 2018] inclusive
    EXPECT_EQ(results.size(), 4u)
        << "searchByYearRange() must include books published exactly on `from` and `to` years";
}

// ── Issue #51 — SearchEngine::rankResults() sorts ascending ──────────────────
TEST(Issue51_Public, RankResultsReturnsBestMatchFirst) {
    auto books = makeBooks();
    SearchEngine se(books);

    // Build results with known scores
    SearchResult r1{&books[0], 0.6};
    SearchResult r2{&books[1], 0.9};
    SearchResult r3{&books[2], 0.75};

    auto ranked = se.rankResults({r1, r2, r3});
    ASSERT_EQ(ranked.size(), 3u);
    EXPECT_DOUBLE_EQ(ranked[0].score, 0.9)
        << "rankResults() must place the highest-scoring result first (descending order)";
    EXPECT_DOUBLE_EQ(ranked[2].score, 0.6);
}
