#pragma once
#include <string>
#include <vector>

class Loan;  // forward declaration

class Member {
public:
    static constexpr int MAX_LOANS = 5;

    enum class MemberType { STUDENT, FACULTY, SENIOR, GUEST };

    Member(const std::string& id,
           const std::string& first_name,
           const std::string& last_name,
           const std::string& email,
           MemberType type,
           const std::string& expiry_date);  // "YYYY-MM-DD"

    // Getters
    const std::string& getID()         const { return id_; }
    const std::string& getFirstName()  const { return first_name_; }
    const std::string& getLastName()   const { return last_name_; }
    const std::string& getEmail()      const { return email_; }
    MemberType         getType()       const { return type_; }
    const std::string& getExpiryDate() const { return expiry_date_; }
    int                getActiveLoanCount() const { return active_loan_count_; }
    bool               isActive()      const { return is_active_; }

    // Setters
    void setEmail(const std::string& email) { email_ = email; }
    void setActive(bool active)             { is_active_ = active; }

    // Operations
    bool        canBorrow()            const;
    bool        isExpired()            const;
    std::string getDisplayName()       const;
    std::string getMembershipStatus()  const;
    void        addLoan(const std::string& loan_id);
    void        removeLoan(const std::string& loan_id);
    bool        hasOverdueLoans()      const;
    void        setHasOverdueLoans(bool val) { has_overdue_loans_ = val; }
    const std::vector<std::string>& getLoanIDs() const { return loan_ids_; }

private:
    std::string              id_;
    std::string              first_name_;
    std::string              last_name_;
    std::string              email_;
    MemberType               type_;
    std::string              expiry_date_;
    int                      active_loan_count_{0};
    bool                     is_active_{true};
    bool                     has_overdue_loans_{false};
    std::vector<std::string> loan_ids_;
};
