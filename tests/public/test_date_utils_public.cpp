#include <gtest/gtest.h>
#include "DateUtils.h"

// ── Issue #42 — DateUtils::addDays() doesn't normalise month rollover ─────────
TEST(Issue42_Public, AddDaysRollsOverMonthBoundary) {
    // Jan 28 + 5 days = Feb 2
    std::string result = DateUtils::addDays("2026-01-28", 5);
    EXPECT_EQ(result, "2026-02-02")
        << "addDays() must correctly handle month rollover (e.g. Jan 28 + 5 = Feb 2)";
}

TEST(Issue42_Public, AddDaysRollsOverYearBoundary) {
    // Dec 30 + 3 days = Jan 2 next year
    std::string result = DateUtils::addDays("2025-12-30", 3);
    EXPECT_EQ(result, "2026-01-02")
        << "addDays() must correctly handle year rollover (e.g. Dec 30 + 3 = Jan 2)";
}

// ── Issue #43 — DateUtils::daysBetween() uses wrong subtraction direction ─────
TEST(Issue43_Public, DaysBetweenIsPositiveWhenDate2IsLater) {
    int diff = DateUtils::daysBetween("2026-01-01", "2026-01-11");
    EXPECT_EQ(diff, 10)
        << "daysBetween(date1, date2) must return a positive value when date2 is after date1";
}

TEST(Issue43_Public, DaysBetweenIsZeroForSameDate) {
    EXPECT_EQ(DateUtils::daysBetween("2026-03-05", "2026-03-05"), 0);
}

// ── Issue #44 — DateUtils::isLeapYear() only checks % 4 ──────────────────────
TEST(Issue44_Public, Year2000IsLeap) {
    EXPECT_TRUE(DateUtils::isLeapYear(2000));
}

TEST(Issue44_Public, Year1900IsNotLeap) {
    // 1900 is divisible by 4 but NOT a leap year (century rule)
    EXPECT_FALSE(DateUtils::isLeapYear(1900))
        << "isLeapYear() must return false for century years not divisible by 400";
}

TEST(Issue44_Public, Year2024IsLeap) {
    EXPECT_TRUE(DateUtils::isLeapYear(2024));
}

TEST(Issue44_Public, Year2100IsNotLeap) {
    EXPECT_FALSE(DateUtils::isLeapYear(2100));
}

// ── Issue #45 — DateUtils::formatDate() uses tm_mon without +1 ───────────────
TEST(Issue45_Public, FormatDateProducesCorrectMonth) {
    std::tm t{};
    t.tm_year = 126;   // 2026
    t.tm_mon  = 2;     // 0-indexed: March
    t.tm_mday = 5;
    std::string result = DateUtils::formatDate(t);
    EXPECT_EQ(result, "2026-03-05")
        << "formatDate() must use tm_mon + 1 (tm_mon is 0-indexed, 0 = January)";
}

// ── Issue #46 — DateUtils::parseDate() swaps month and day ───────────────────
TEST(Issue46_Public, ParseDateExtractsCorrectMonthAndDay) {
    std::tm t = DateUtils::parseDate("2026-03-05");
    // tm_mon is 0-indexed, so March = 2
    EXPECT_EQ(t.tm_mon,  2) << "parseDate() must assign the month field (pos 5–6) to tm_mon";
    EXPECT_EQ(t.tm_mday, 5) << "parseDate() must assign the day field (pos 8–9) to tm_mday";
}
