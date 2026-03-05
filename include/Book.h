#pragma once
#include <string>

class Book {
public:
    Book(const std::string& isbn,
         const std::string& title,
         const std::string& author,
         int publication_year,
         int total_copies,
         const std::string& genre = "");

    // Getters
    const std::string& getISBN()            const { return isbn_; }
    const std::string& getTitle()           const { return title_; }
    const std::string& getAuthor()          const { return author_; }
    int                getPublicationYear() const { return publication_year_; }
    int                getTotalCopies()     const { return total_copies_; }
    int                getAvailableCopies() const { return copies_available_; }
    const std::string& getGenre()           const { return genre_; }
    int                getBorrowCount()     const { return borrow_count_; }

    // Setters
    void setPublicationYear(int year);

    // Operations
    bool        isAvailable()  const;
    bool        validateISBN() const;
    void        decrementCopies();
    void        incrementCopies();
    std::string getFullTitle()  const;
    void        incrementBorrowCount()  { ++borrow_count_; }

private:
    std::string isbn_;
    std::string title_;
    std::string author_;
    int         publication_year_;
    int         total_copies_;
    int         copies_available_;
    std::string genre_;
    int         borrow_count_{0};
};
