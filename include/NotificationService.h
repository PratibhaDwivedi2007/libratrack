#pragma once
#include "Member.h"
#include "Loan.h"
#include <vector>
#include <string>

class NotificationService {
public:
    struct Notice {
        std::string member_id;
        std::string message;
    };

    // Generates overdue notices only for members who have overdue loans
    std::vector<Notice> sendOverdueNotices(
        const std::vector<Member>& members,
        const std::vector<Loan>&   loans) const;

    // Formats a greeting using the member's display name
    std::string formatMessage(const Member& member,
                              const std::string& body) const;

    // Returns member IDs whose loan is due in exactly `days_before` days
    std::vector<std::string> scheduleReminders(
        const std::vector<Loan>&   loans,
        const std::vector<Member>& members,
        int days_before = 3) const;
};
