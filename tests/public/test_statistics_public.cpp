#include <gtest/gtest.h>
#include "Statistics.h"
#include "Loan.h"
#include "Book.h"
#include "Member.h"

// ── Issue #55 — calculateAverageLoanDuration() includes active loans ──────────
TEST(Issue55_Public, AverageLoanDurationOnlyCountsReturnedLoans) {
    // Returned loan: 10 days
    Loan returned("L001", "isbn1", "M001", "2026-01-01");
    returned.markReturned("2026-01-11");  // 10 days

    // Active loan: checked out today — duration is 0 or very small
    Loan active("L002", "isbn2", "M001", "2026-03-04");
    // Not returned

    Statistics stats;
    double avg = stats.calculateAverageLoanDuration({returned, active});

    // If active loan is included in the denominator with duration ~1 day,
    // avg would be ~5.5. The correct answer (only returned loan) is 10.
    EXPECT_DOUBLE_EQ(avg, 10.0)
        << "calculateAverageLoanDuration() must only average over returned loans";
}

// ── Issue #56 — getPopularityScore() adds instead of divides ─────────────────
TEST(Issue56_Public, PopularityScoreIsRatioNotSum) {
    Book b("isbn1", "Popular Book", "Author", 2020, 1);
    for (int i = 0; i < 10; ++i) b.incrementBorrowCount();  // borrow_count = 10

    Statistics stats;
    double score = stats.getPopularityScore(b, 100);

    // Correct: 10 / 100 = 0.1
    // Buggy:   10 + 100 = 110.0
    EXPECT_DOUBLE_EQ(score, 0.1)
        << "getPopularityScore() must return borrow_count / total_members (a ratio)";
}

// ── Issue #57 — getMostActiveMonth() uses wrong index variable ────────────────
TEST(Issue57_Public, GetMostActiveMonthReturnsCorrectMonth) {
    // 5 loans in March (month 3), 1 in January
    std::vector<Loan> loans;
    loans.emplace_back("L001", "i", "M", "2026-01-15");
    loans.emplace_back("L002", "i", "M", "2026-03-01");
    loans.emplace_back("L003", "i", "M", "2026-03-05");
    loans.emplace_back("L004", "i", "M", "2026-03-10");
    loans.emplace_back("L005", "i", "M", "2026-03-15");
    loans.emplace_back("L006", "i", "M", "2026-03-20");

    Statistics stats;
    EXPECT_EQ(stats.getMostActiveMonth(loans), 3)
        << "getMostActiveMonth() must return 3 (March) — the month with the most checkouts";
}

// ── Issue #58 — generateTrend() never resets per-period counter ───────────────
TEST(Issue58_Public, GenerateTrendCountsEachPeriodIndependently) {
    std::vector<Loan> loans;
    // 2 loans in January
    loans.emplace_back("L001", "i", "M", "2026-01-10");
    loans.emplace_back("L002", "i", "M", "2026-01-20");
    // 1 loan in February
    loans.emplace_back("L003", "i", "M", "2026-02-05");

    Statistics stats;
    auto trend = stats.generateTrend(loans);

    ASSERT_EQ(trend.count("2026-01"), 1u);
    ASSERT_EQ(trend.count("2026-02"), 1u);
    EXPECT_EQ(trend.at("2026-01"), 2)
        << "generateTrend() must count 2 checkouts in January independently";
    EXPECT_EQ(trend.at("2026-02"), 1)
        << "generateTrend() must count 1 checkout in February; the January count must not carry over";
}
