#pragma once
#include <string>

class Loan {
public:
    Loan(const std::string& loan_id,
         const std::string& book_isbn,
         const std::string& member_id,
         const std::string& checkout_date);  // "YYYY-MM-DD"

    // Getters
    const std::string& getLoanID()       const { return loan_id_; }
    const std::string& getBookISBN()     const { return book_isbn_; }
    const std::string& getMemberID()     const { return member_id_; }
    const std::string& getCheckoutDate() const { return checkout_date_; }
    const std::string& getReturnDate()   const { return return_date_; }
    double             getFineAmount()   const { return fine_amount_; }
    bool               getReturned()     const { return returned_; }

    // Operations
    std::string getDueDate()       const;
    int         getDaysOverdue()   const;
    bool        isReturned()       const;
    bool        isOverdue()        const;
    std::string toString()         const;

    void markReturned(const std::string& return_date);
    void setFineAmount(double amount) { fine_amount_ = amount; }

private:
    std::string loan_id_;
    std::string book_isbn_;
    std::string member_id_;
    std::string checkout_date_;
    std::string return_date_;
    double      fine_amount_{0.0};
    bool        returned_{false};
};
