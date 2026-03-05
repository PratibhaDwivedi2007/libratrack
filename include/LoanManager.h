#pragma once
#include "Catalog.h"
#include "Member.h"
#include "Loan.h"
#include "FineCalculator.h"
#include <vector>
#include <string>

class LoanManager {
public:
    LoanManager(Catalog& catalog, std::vector<Member>& members);

    // Returns the generated loan ID on success, empty string on failure
    std::string checkoutBook(const std::string& isbn,
                             const std::string& member_id,
                             const std::string& date);

    bool returnBook(const std::string& loan_id,
                    const std::string& return_date);

    bool renewLoan(const std::string& loan_id);

    std::vector<Loan>  getActiveLoans()                  const;
    std::vector<Loan>& getAllLoans()                     { return loans_; }
    const std::vector<Loan>& getAllLoans()               const { return loans_; }
    Loan*              findLoan(const std::string& loan_id);

private:
    Catalog&             catalog_;
    std::vector<Member>& members_;
    std::vector<Loan>    loans_;
    FineCalculator       fine_calc_;
    int                  next_loan_id_{1};

    Member* findMember(const std::string& member_id);
};
