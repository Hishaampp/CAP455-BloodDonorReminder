// test_validators.cpp
// Automatic checks for validation, dates, eligibility (CA1)
// and constructors + binary file functions (CA2).
//
// Build: g++ -std=c++17 -Wall tests/test_validators.cpp src/Donor.cpp src/Utils.cpp src/FileHandler.cpp -Isrc -o run_tests
// Run  : ./run_tests

#include <iostream>
#include <cstdio>
#include "Donor.h"
#include "Utils.h"
#include "FileHandler.h"
using namespace std;

int passed = 0;
int failed = 0;

void check(bool ok, string message) {
    if (ok) {
        passed++;
        cout << "PASS  " << message << "\n";
    } else {
        failed++;
        cout << "FAIL  " << message << "\n";
    }
}

Date makeDate(int d, int m, int y) {
    Date date = {d, m, y};
    return date;
}

int main() {
    cout << "--- CA1: validation ---\n";
    check(isValidId("D1001"), "D1001 is a valid ID");
    check(!isValidId("1001"), "1001 is rejected (no D)");
    check(!isValidId("D10"), "D10 is rejected (too short)");
    check(!isValidId("D10A1"), "D10A1 is rejected (letter inside)");
    check(isValidName("R. K. Sharma"), "name with dots and spaces is accepted");
    check(!isValidName("A"), "one letter name is rejected");
    check(!isValidName("R@hul"), "name with @ is rejected");
    check(isValidCity("New Delhi"), "city with a space is accepted");
    check(!isValidCity("Ph4gwara"), "city with a digit is rejected");
    check(isValidPhone("9876543210"), "phone starting with 9 is accepted");
    check(isValidPhone("6000000002"), "phone starting with 6 is accepted (boundary)");
    check(!isValidPhone("5876543210"), "phone starting with 5 is rejected");
    check(!isValidPhone("98765"), "5 digit phone is rejected");
    check(isValidBloodGroup("AB-"), "AB- is a valid blood group");
    check(!isValidBloodGroup("C+"), "C+ is rejected");

    cout << "\n--- CA1: dates ---\n";
    Date d;
    check(stringToDate("29-02-2024", d), "29-02-2024 is valid (leap year)");
    check(!stringToDate("29-02-2026", d), "29-02-2026 is rejected (not leap year)");
    check(!stringToDate("31-04-2026", d), "31-04-2026 is rejected (April has 30 days)");
    check(!stringToDate("2026-08-01", d), "YYYY-MM-DD format is rejected");
    check(dateToString(daysToDate(dateToDays(makeDate(1, 8, 2026)) + 90)) == "30-10-2026",
          "01-08-2026 + 90 days = 30-10-2026");
    check(dateToString(daysToDate(dateToDays(makeDate(31, 12, 2026)) + 1)) == "01-01-2027",
          "31-12-2026 + 1 day = 01-01-2027");

    cout << "\n--- CA1: eligibility (friend function) ---\n";
    Date today = makeDate(5, 11, 2026);
    Donor male("D1001", "Test Male", 30, 'M', 70, "B+", "9876543210", "Phagwara", true, makeDate(7, 8, 2026));
    Donor female("D1002", "Test Female", 25, 'F', 55, "O+", "9876543211", "Jalandhar", true, makeDate(9, 7, 2026));
    Donor newDonor("D1003", "Test New", 20, 'F', 50, "A+", "9876543212", "Amritsar", false, makeDate(0, 0, 0));
    check(daysLeft(male, today) == 0, "male donated 07-08-2026: eligible exactly today (90 days)");
    check(daysLeft(female, today) == 1, "female donated 09-07-2026: due in 1 day (120 days)");
    check(daysLeft(newDonor, today) == 0, "first time donor is eligible now");
    check(Donor::getStatus(7) == "Due in 7 days", "7 days left is inside the reminder window");
    check(Donor::getStatus(8) == "Wait 8 days", "8 days left is outside the reminder window");

    cout << "\n--- CA2: constructors and destructor ---\n";
    Donor empty;
    check(empty.getId() == "" && empty.isFirstTime(), "default constructor makes an empty donor");
    check(male.getName() == "Test Male" && male.getBloodGroup() == "B+", "parameterised constructor stores the values");
    Donor copy(male);
    check(copy.getId() == "D1001" && daysLeft(copy, today) == 0, "copy constructor copies all details");
    copy.setPhone("9000000000");
    check(male.getPhone() == "9876543210", "changing the copy does not change the original");
    int before = Donor::getObjectCount();
    {
        Donor temp(male);   // object made inside this block
        check(Donor::getObjectCount() == before + 1, "object count goes up when an object is made");
    }                       // temp is destroyed here, destructor runs
    check(Donor::getObjectCount() == before, "destructor runs when the object goes out of scope");

    cout << "\n--- CA2: binary file ---\n";
    string testFile = "tests/test_donors.dat";
    remove(testFile.c_str());   // start with no file
    check(countRecords(testFile) == 0, "no file means 0 records");
    check(saveDonor(testFile, male), "save donor 1");
    check(saveDonor(testFile, female), "save donor 2");
    check(saveDonor(testFile, newDonor), "save donor 3");
    check(countRecords(testFile) == 3, "file has 3 records (size / sizeof(DonorRecord))");
    check(findPositionInFile(testFile, "D1002") == 1, "seekg search finds D1002 at position 1");
    check(findPositionInFile(testFile, "D9999") == -1, "seekg search returns -1 for a missing ID");

    Donor fromFile;
    check(readDonorAt(testFile, 2, fromFile) && fromFile.getName() == "Test New", "read record 3 with seekg");

    fromFile.setCity("Ludhiana");
    check(updateDonorAt(testFile, 2, fromFile), "update record 3 with seekp");
    Donor again;
    readDonorAt(testFile, 2, again);
    check(again.getCity() == "Ludhiana", "updated city is in the file");
    check(countRecords(testFile) == 3, "update did not add a new record");

    Donor loaded[MAX_DONORS];
    int n = loadAllDonors(testFile, loaded, MAX_DONORS);
    check(n == 3 && loaded[0].getId() == "D1001" && loaded[2].getCity() == "Ludhiana",
          "load all donors back (like after a restart)");
    remove(testFile.c_str());

    cout << "\nTotal: " << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
