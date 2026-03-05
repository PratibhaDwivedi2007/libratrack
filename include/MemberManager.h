#pragma once
#include "Member.h"
#include <vector>
#include <string>
#include <optional>

class MemberManager {
public:
    void addMember(const Member& member);
    bool removeMember(const std::string& id);

    Member*              findByID(const std::string& id);
    std::vector<Member*> getActiveMembers()  const;
    std::vector<Member*> getMembersWithOverdueLoans();
    bool                 deactivateMember(const std::string& id);
    bool                 updateEmail(const std::string& id,
                                     const std::string& email);

    std::vector<Member>&       getAll()       { return members_; }
    const std::vector<Member>& getAll() const { return members_; }
    int size() const { return static_cast<int>(members_.size()); }

private:
    std::vector<Member> members_;
};
