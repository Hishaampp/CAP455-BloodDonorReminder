/*
 * File    : test_validators.cpp
 * Project : Blood Donor Eligibility Reminder Console (CAP455 Minor Project)
 * Phase   : CA1 - Design & Create
 * Purpose : Automatic checks for the validators, date maths and eligibility
 *           rule. Prints PASS/FAIL for each check and a final total.
 *
 * Build : g++ -std=c++17 -Wall tests/test_validators.cpp src/Donor.cpp src/Utils.cpp -Isrc -o run_tests
 * Run   : ./run_tests
 */
#include <iostream>
#include <string>

#include "Donor.h"
#include "Utils.h"

using namespace std;

int passed = 0;
int failed = 0;

void check(bool condition, const string& description) {
    if (condition) {
        passed++;
        cout << "PASS  " << description << "\n";
    } else {
        failed++;
        cout << "FAIL  " << description << "\n";
    }
}

Date makeDate(int day, int month, int year) {
    Date d = {day, month, year};
    return d;
}

int main() {
    // ----- Donor ID -----
    check(isValidDonorId("D1001"), "D1001 is a valid Donor ID");
    check(!isValidDonorId("1001"), "1001 is rejected (no D)");
    check(!isValidDonorId("D10"), "D10 is rejected (too short)");
    check(!isValidDonorId("D10A1"), "D10A1 is rejected (letter in number)");

    // ----- Name and city -----
    check(isValidName("R. K. Sharma"), "Name with dots and spaces is accepted");
    check(!isValidName("A"), "One-letter name is rejected");
    check(!isValidName("R@hul"), "Name with symbol is rejected");
    check(isValidCity("New Delhi"), "City with a space is accepted");
    check(!isValidCity("Ph4gwara"), "City with a digit is rejected");

    // ----- Phone -----
    check(isValidPhone("9876543210"), "10-digit number starting with 9 is accepted");
    check(isValidPhone("6000000002"), "Number starting with 6 is accepted (boundary)");
    check(!isValidPhone("5876543210"), "Number starting with 5 is rejected");
    check(!isValidPhone("98765"), "5-digit number is rejected");

    // ----- Blood group -----
    check(isValidBloodGroup("AB-"), "AB- is a valid blood group");
    check(!isValidBloodGroup("C+"), "C+ is rejected");
    check(!isValidBloodGroup("ab+"), "Lowercase ab+ is rejected (input is uppercased first)");

    // ----- Dates -----
    Date d;
    check(parseDate("29-02-2024", d), "29-02-2024 is valid (leap year)");
    check(!parseDate("29-02-2026", d), "29-02-2026 is rejected (not a leap year)");
    check(!parseDate("31-04-2026", d), "31-04-2026 is rejected (April has 30 days)");
    check(!parseDate("2026-08-01", d), "YYYY-MM-DD format is rejected");
    check(formatDate(fromDayNumber(toDayNumber(makeDate(1, 8, 2026)) + 90)) == "30-10-2026",
          "01-08-2026 + 90 days = 30-10-2026");
    check(formatDate(fromDayNumber(toDayNumber(makeDate(31, 12, 2026)) + 1)) == "01-01-2027",
          "31-12-2026 + 1 day = 01-01-2027 (year change)");

    // ----- Eligibility (friend function) -----
    Date today = makeDate(5, 11, 2026);
    Donor male;
    male.setDetails("D1001", "Test Male", 30, 'M', 70, "B+", "9876543210", "Phagwara",
                    true, makeDate(7, 8, 2026));
    check(daysUntilEligible(male, today) == 0, "Male, donated 07-08-2026: eligible exactly today");

    Donor female;
    female.setDetails("D1002", "Test Female", 25, 'F', 55, "O+", "9876543211", "Jalandhar",
                      true, makeDate(9, 7, 2026));
    check(daysUntilEligible(female, today) == 1, "Female, donated 09-07-2026: due in 1 day (120-day gap)");

    Donor firstTime;
    firstTime.setDetails("D1003", "Test New", 20, 'F', 50, "A+", "9876543212", "Amritsar",
                         false, makeDate(0, 0, 0));
    check(daysUntilEligible(firstTime, today) == 0, "First-time donor is eligible now");

    check(Donor::statusText(7) == "Due in 7 days", "7 days left is inside the reminder window");
    check(Donor::statusText(8) == "Wait 8 days", "8 days left is outside the reminder window");

    cout << "\nTotal: " << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
