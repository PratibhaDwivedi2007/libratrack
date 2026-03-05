#pragma once
#include "Loan.h"
#include "Member.h"
#include <vector>

class FineCalculator {
public:
    static constexpr int    GRACE_PERIOD_DAYS = 2;
    static constexpr double MAX_FINE          = 20.0;

    double getDailyRate()   const;
    double calculateFine(const Loan& loan)  const;
    double getTotalFines(const std::vector<Loan>& loans) const;
    double applyDiscount(double fine, const Member& member) const;
    double getMaxFine(const std::vector<Loan>& loans)      const;
    bool   hasPendingFines(const std::vector<Loan>& loans) const;
};
