#include <gtest/gtest.h>
#include "NotificationService.h"
#include "Member.h"
#include "Loan.h"
#include "DateUtils.h"

// ── Issue #52 — sendOverdueNotices() sends to ALL members regardless ──────────
TEST(Issue52_Public, SendOverdueNoticesOnlySendsToMembersWithOverdueLoans) {
    Member m1("M001", "Alice", "Smith", "a@test.com",
              Member::MemberType::STUDENT, "2099-12-31");
    Member m2("M002", "Bob",   "Jones", "b@test.com",
              Member::MemberType::STUDENT, "2099-12-31");
    m1.setHasOverdueLoans(true);
    // m2 has no overdue loans

    NotificationService ns;
    auto notices = ns.sendOverdueNotices({m1, m2}, {});

    ASSERT_EQ(notices.size(), 1u)
        << "sendOverdueNotices() must only generate a notice for members with overdue loans";
    EXPECT_EQ(notices[0].member_id, "M001");
}

// ── Issue #53 — formatMessage() uses member ID instead of display name ────────
TEST(Issue53_Public, FormatMessageUsesDisplayName) {
    Member m("M001", "Alice", "Smith", "a@test.com",
             Member::MemberType::STUDENT, "2099-12-31");

    NotificationService ns;
    std::string msg = ns.formatMessage(m, "Please return your books.");

    EXPECT_NE(msg.find("Alice Smith"), std::string::npos)
        << "formatMessage() must address the member by display name (\"Alice Smith\"), not by ID";
    EXPECT_EQ(msg.find("M001"), std::string::npos)
        << "formatMessage() must not use the raw member ID in the greeting";
}

// ── Issue #54 — scheduleReminders() fires at 1 day instead of days_before ────
TEST(Issue54_Public, ScheduleRemindersRespectsDaysBeforeParameter) {
    // Build a loan whose due date is exactly 3 days from today
    std::string due_in_3 = DateUtils::addDays(DateUtils::today(), 3);
    // Reverse-engineer checkout date: due = checkout + 14
    std::string checkout = DateUtils::addDays(due_in_3, -14);

    Loan loan("L001", "isbn1", "M001", checkout);

    Member m("M001", "Alice", "Smith", "a@test.com",
             Member::MemberType::STUDENT, "2099-12-31");

    NotificationService ns;
    auto ids = ns.scheduleReminders({loan}, {m}, 3);

    EXPECT_FALSE(ids.empty())
        << "scheduleReminders() with days_before=3 must fire for a loan due in 3 days";
    EXPECT_EQ(ids[0], "M001");
}

TEST(Issue54_Public, ScheduleRemindersDoesNotFireForWrongInterval) {
    // Loan due in 5 days — should NOT trigger with days_before=3
    std::string due_in_5 = DateUtils::addDays(DateUtils::today(), 5);
    std::string checkout = DateUtils::addDays(due_in_5, -14);

    Loan loan("L001", "isbn1", "M001", checkout);
    Member m("M001", "Alice", "Smith", "a@test.com",
             Member::MemberType::STUDENT, "2099-12-31");

    NotificationService ns;
    auto ids = ns.scheduleReminders({loan}, {m}, 3);

    EXPECT_TRUE(ids.empty())
        << "scheduleReminders() with days_before=3 must NOT fire for a loan due in 5 days";
}
