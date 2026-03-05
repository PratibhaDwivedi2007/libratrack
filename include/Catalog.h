#pragma once
#include "Book.h"
#include <vector>
#include <string>
#include <optional>

class Catalog {
public:
    void addBook(const Book& book);
    bool removeBook(const std::string& isbn);
    bool removeBookByIndex(int index);

    std::optional<Book*> findByISBN(const std::string& isbn);
    std::vector<Book*>   searchByTitle(const std::string& query);
    std::vector<Book*>   searchByAuthor(const std::string& query);
    std::vector<Book*>   getAvailableBooks();
    std::vector<Book*>   getAllBooks();
    std::vector<Book*>   getBooksByGenre(const std::string& genre);

    void sortByTitle();
    void sortByAuthor();
    int  size() const { return static_cast<int>(books_.size()); }

private:
    std::vector<Book> books_;
};
