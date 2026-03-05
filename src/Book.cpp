#include "Book.h"
#include <stdexcept>

Book::Book(const std::string& isbn,
           const std::string& title,
           const std::string& author,
           int publication_year,
           int total_copies,
           const std::string& genre)
    : isbn_(isbn)
    , title_(title)
    , author_(author)
    , publication_year_(publication_year)
    , total_copies_(total_copies)
    , copies_available_(total_copies)
    , genre_(genre)
{}

bool Book::isAvailable() const {
    return copies_available_ >= 0;
}

bool Book::validateISBN() const {
    if (isbn_.length() != 12) return false;
    for (char c : isbn_) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

void Book::decrementCopies() {
    --copies_available_;
}

void Book::incrementCopies() {
    if (copies_available_ < total_copies_) {
        ++copies_available_;
    }
}

void Book::setPublicationYear(int year) {
    publication_year_ = year;
}

std::string Book::getFullTitle() const {
    return author_ + " - " + title_;
}
