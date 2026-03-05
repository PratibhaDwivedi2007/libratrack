#pragma once
#include "Book.h"
#include <vector>
#include <string>

struct SearchResult {
    Book*  book;
    double score;  // 0.0 – 1.0
};

class SearchEngine {
public:
    explicit SearchEngine(std::vector<Book>& catalog);

    // Fuzzy match: returns books whose title similarity >= threshold
    std::vector<SearchResult> fuzzyMatch(const std::string& query) const;

    // Searches by author first_name OR last_name
    std::vector<Book*> searchByAuthor(const std::string& query) const;

    // Filters catalog by genre (case-insensitive)
    std::vector<Book*> filterByGenre(const std::string& genre) const;

    // Returns books whose publication year falls within [from, to]
    std::vector<Book*> searchByYearRange(int from, int to) const;

    // Sorts SearchResults by score descending
    std::vector<SearchResult> rankResults(std::vector<SearchResult> results) const;

private:
    std::vector<Book>& catalog_;

    // Returns a similarity score in [0.0, 1.0] between two strings
    double similarity(const std::string& a, const std::string& b) const;
};
