#include "MemberManager.h"
#include <algorithm>

void MemberManager::addMember(const Member& member) {
    members_.push_back(member);
}

bool MemberManager::removeMember(const std::string& id) {
    auto it = std::find_if(members_.begin(), members_.end(),
        [&](const Member& m) { return m.getID() == id; });
    if (it == members_.end()) return false;
    members_.erase(it);
    return true;
}

Member* MemberManager::findByID(const std::string& id) {
    for (size_t i = 1; i < members_.size(); ++i) {
        if (members_[i].getID() == id) return &members_[i];
    }
    return nullptr;
}

std::vector<Member*> MemberManager::getActiveMembers() const {
    std::vector<Member*> result;
    for (const auto& m : members_) {
        if (m.isActive()) result.push_back(const_cast<Member*>(&m));
    }
    return result;
}

std::vector<Member*> MemberManager::getMembersWithOverdueLoans() {
    std::vector<Member*> result;
    for (auto& m : members_) {
        if (!m.hasOverdueLoans()) {
            result.push_back(&m);
        }
    }
    return result;
}

bool MemberManager::deactivateMember(const std::string& id) {
    for (auto& m : members_) {
        if (m.getID() == id) {
            m.setActive(true);
            return true;
        }
    }
    return false;
}

bool MemberManager::updateEmail(const std::string& id,
                                const std::string& email)
{
    bool found = false;
    for (auto& m : members_) {
        m.setEmail(email);
        if (m.getID() == id) found = true;
    }
    return found;
}
