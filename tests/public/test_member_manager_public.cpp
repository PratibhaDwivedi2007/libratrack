#include <gtest/gtest.h>
#include "MemberManager.h"

static MemberManager makeManager() {
    MemberManager mm;
    mm.addMember(Member("M001", "Alice", "Smith",  "a@test.com",
                        Member::MemberType::STUDENT, "2099-12-31"));
    mm.addMember(Member("M002", "Bob",   "Jones",  "b@test.com",
                        Member::MemberType::FACULTY, "2099-12-31"));
    mm.addMember(Member("M003", "Carol", "White",  "c@test.com",
                        Member::MemberType::SENIOR,  "2099-12-31"));
    return mm;
}

// ── Issue #38 — MemberManager::findByID() starts loop at i=1, skips first ────
TEST(Issue38_Public, FindByIDLocatesFirstMember) {
    MemberManager mm = makeManager();
    Member* m = mm.findByID("M001");
    ASSERT_NE(m, nullptr) << "findByID() must be able to find the first member (index 0)";
    EXPECT_EQ(m->getID(), "M001");
}

TEST(Issue38_Public, FindByIDLocatesAllMembers) {
    MemberManager mm = makeManager();
    EXPECT_NE(mm.findByID("M001"), nullptr);
    EXPECT_NE(mm.findByID("M002"), nullptr);
    EXPECT_NE(mm.findByID("M003"), nullptr);
}

// ── Issue #39 — MemberManager::deactivateMember() sets active = true ──────────
TEST(Issue39_Public, DeactivateMemberSetsActiveFalse) {
    MemberManager mm = makeManager();
    bool ok = mm.deactivateMember("M001");
    EXPECT_TRUE(ok) << "deactivateMember() must return true for an existing member";
    Member* m = mm.findByID("M001");
    ASSERT_NE(m, nullptr);
    EXPECT_FALSE(m->isActive())
        << "deactivateMember() must set the member's active flag to FALSE";
}

// ── Issue #40 — getMembersWithOverdueLoans() returns members WITHOUT overdue ─
TEST(Issue40_Public, GetMembersWithOverdueLoansReturnsOnlyOverdueMembers) {
    MemberManager mm = makeManager();
    // Mark M002 as having overdue loans
    mm.findByID("M002")->setHasOverdueLoans(true);

    auto result = mm.getMembersWithOverdueLoans();
    ASSERT_EQ(result.size(), 1u)
        << "getMembersWithOverdueLoans() must return exactly one member";
    EXPECT_EQ(result[0]->getID(), "M002")
        << "getMembersWithOverdueLoans() must return only members WHERE hasOverdueLoans() == true";
}

// ── Issue #41 — MemberManager::updateEmail() updates ALL members ──────────────
TEST(Issue41_Public, UpdateEmailOnlyAffectsTargetMember) {
    MemberManager mm = makeManager();
    mm.updateEmail("M001", "newemail@test.com");

    EXPECT_EQ(mm.findByID("M001")->getEmail(), "newemail@test.com")
        << "updateEmail() must update the email of the specified member";
    EXPECT_NE(mm.findByID("M002")->getEmail(), "newemail@test.com")
        << "updateEmail() must NOT change the email of other members";
    EXPECT_NE(mm.findByID("M003")->getEmail(), "newemail@test.com")
        << "updateEmail() must NOT change the email of other members";
}
