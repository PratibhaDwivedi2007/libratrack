#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "Catalog.h"
#include "MemberManager.h"
#include "LoanManager.h"
#include "FineCalculator.h"
#include "ReportGenerator.h"
#include "SearchEngine.h"
#include "NotificationService.h"
#include "Statistics.h"
#include "DateUtils.h"

// ─── helpers ─────────────────────────────────────────────────────────────────
static void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static std::string prompt(const std::string& label) {
    std::cout << label << ": ";
    std::string val;
    std::getline(std::cin, val);
    return val;
}

// ─── menus ───────────────────────────────────────────────────────────────────
static void printMainMenu() {
    std::cout << "\n╔══════════════════════════════╗\n"
              << "║       LibraTrack v1.0        ║\n"
              << "╠══════════════════════════════╣\n"
              << "║ 1. Catalog                   ║\n"
              << "║ 2. Members                   ║\n"
              << "║ 3. Loans                     ║\n"
              << "║ 4. Reports & Statistics      ║\n"
              << "║ 5. Notifications             ║\n"
              << "║ 0. Exit                      ║\n"
              << "╚══════════════════════════════╝\n"
              << "Choice: ";
}

static void catalogMenu(Catalog& catalog) {
    std::cout << "\n-- Catalog --\n"
              << "1. Add book\n2. List all books\n3. Search by title\n0. Back\nChoice: ";
    int c; std::cin >> c; clearInput();
    if (c == 1) {
        std::string isbn  = prompt("ISBN");
        std::string title = prompt("Title");
        std::string auth  = prompt("Author");
        std::string year_s= prompt("Year");
        std::string cop_s = prompt("Copies");
        std::string genre = prompt("Genre");
        catalog.addBook(Book(isbn, title, auth, std::stoi(year_s),
                             std::stoi(cop_s), genre));
        std::cout << "Book added.\n";
    } else if (c == 2) {
        for (auto* b : catalog.getAllBooks()) {
            std::cout << b->getISBN() << "  " << b->getFullTitle()
                      << "  (" << b->getAvailableCopies() << " available)\n";
        }
    } else if (c == 3) {
        std::string q = prompt("Query");
        for (auto* b : catalog.searchByTitle(q)) {
            std::cout << b->getISBN() << "  " << b->getTitle() << "\n";
        }
    }
}

static void memberMenu(MemberManager& mm) {
    std::cout << "\n-- Members --\n"
              << "1. Add member\n2. List members\n3. Find by ID\n0. Back\nChoice: ";
    int c; std::cin >> c; clearInput();
    if (c == 1) {
        std::string id    = prompt("ID");
        std::string fname = prompt("First name");
        std::string lname = prompt("Last name");
        std::string email = prompt("Email");
        std::string expiry= prompt("Expiry (YYYY-MM-DD)");
        mm.addMember(Member(id, fname, lname, email,
                            Member::MemberType::STUDENT, expiry));
        std::cout << "Member added.\n";
    } else if (c == 2) {
        for (const auto& m : mm.getAll()) {
            std::cout << m.getID() << "  " << m.getDisplayName()
                      << "  " << m.getMembershipStatus() << "\n";
        }
    } else if (c == 3) {
        std::string id = prompt("ID");
        Member* m = mm.findByID(id);
        if (m) {
            std::cout << "Found: " << m->getDisplayName()
                      << " (" << m->getMembershipStatus() << ")\n";
        } else {
            std::cout << "Not found.\n";
        }
    }
}

static void loanMenu(LoanManager& lm) {
    std::cout << "\n-- Loans --\n"
              << "1. Checkout\n2. Return\n3. List active loans\n0. Back\nChoice: ";
    int c; std::cin >> c; clearInput();
    if (c == 1) {
        std::string isbn = prompt("ISBN");
        std::string mid  = prompt("Member ID");
        std::string lid  = lm.checkoutBook(isbn, mid, DateUtils::today());
        if (lid.empty()) std::cout << "Checkout failed.\n";
        else             std::cout << "Checked out. Loan ID: " << lid << "\n";
    } else if (c == 2) {
        std::string lid = prompt("Loan ID");
        if (lm.returnBook(lid, DateUtils::today()))
            std::cout << "Returned.\n";
        else
            std::cout << "Return failed.\n";
    } else if (c == 3) {
        for (const auto& l : lm.getActiveLoans()) {
            std::cout << l.toString() << "\n";
        }
    }
}

static void reportMenu(Catalog&             catalog,
                       const MemberManager& mm,
                       const LoanManager&   lm)
{
    ReportGenerator rg;
    Statistics      stats;
    std::cout << "\n-- Reports --\n";
    std::vector<Book> all_books;
    for (auto* b : catalog.getAllBooks()) all_books.push_back(*b);
    std::cout << rg.generateSummaryReport(all_books, mm.getAll(), lm.getAllLoans()) << "\n";

    std::cout << "Most active month: "
              << stats.getMostActiveMonth(lm.getAllLoans()) << "\n";
}

// ─── main ────────────────────────────────────────────────────────────────────
int main() {
    Catalog        catalog;
    MemberManager  mm;
    LoanManager    lm(catalog, mm.getAll());

    // Seed a couple of demo entries so the system is not empty on first run
    catalog.addBook(Book("9780132350884", "Clean Code", "Robert C. Martin",
                         2008, 3, "Programming"));
    catalog.addBook(Book("9780201633610", "Design Patterns", "Gang of Four",
                         1994, 2, "Programming"));
    mm.addMember(Member("M001", "Alice", "Smith", "alice@example.com",
                        Member::MemberType::STUDENT, "2027-12-31"));

    int choice = -1;
    while (choice != 0) {
        printMainMenu();
        std::cin >> choice; clearInput();
        switch (choice) {
            case 1: catalogMenu(catalog);            break;
            case 2: memberMenu(mm);                  break;
            case 3: loanMenu(lm);                    break;
            case 4: reportMenu(catalog, mm, lm);  break;
            case 5: {
                NotificationService ns;
                auto notices = ns.sendOverdueNotices(mm.getAll(), lm.getAllLoans());
                std::cout << notices.size() << " notice(s) generated.\n";
                break;
            }
            case 0: std::cout << "Goodbye.\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    }
    return 0;
}
