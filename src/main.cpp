/*
 * File    : main.cpp
 * Project : Blood Donor Eligibility Reminder Console (CAP455 Minor Project)
 * Phase   : CA1 - Design & Create
 * Purpose : Menu-driven console. CA1 covers in-memory Create and Read:
 *           add donor, show all, search by ID, reminders, search by group.
 *
 * Build : g++ -std=c++17 -Wall src/main.cpp src/Donor.cpp src/Utils.cpp -o donor_app
 * Run   : ./donor_app              (uses today's date)
 *         ./donor_app 05-11-2026   (test mode: pretends today is the given date)
 */
#include <iostream>
#include <iomanip>
#include <string>

#include "Donor.h"
#include "Utils.h"

using namespace std;

// ===================== Display helpers =====================

void printLine(char symbol, int length) {
    for (int i = 0; i < length; i++) {
        cout << symbol;
    }
    cout << "\n";
}

void printTableHeader() {
    cout << left
         << setw(7) << "ID"
         << setw(20) << "Name"
         << setw(5) << "Age"
         << setw(4) << "Sex"
         << setw(7) << "Group"
         << setw(12) << "Phone"
         << setw(14) << "Last donated"
         << setw(14) << "Next eligible"
         << "Status" << "\n";
    printLine('-', 99);
}

// ===================== Search using pointers =====================

// Walks through the array with a pointer and returns the address of the
// matching donor, or nullptr if the ID is not found.
Donor* findDonorById(Donor* list, int count, const string& id) {
    for (Donor* p = list; p < list + count; p++) {
        if (p->hasSameId(id)) {
            return p;
        }
    }
    return nullptr;
}

// ===================== Validated input for each field =====================

// Returns a new, unused Donor ID, or "0" if the user cancels.
string readNewDonorId(Donor* list, int count) {
    while (true) {
        string id = toUpperCase(readLine("Donor ID (e.g. D1001)       : "));
        if (id == "0") {
            return id;
        }
        if (!isValidDonorId(id)) {
            cout << "  Invalid ID. Use the letter D followed by 4 digits, e.g. D1001.\n";
        } else if (findDonorById(list, count, id) != nullptr) {
            cout << "  Donor ID " << id << " already exists. Enter a different ID.\n";
        } else {
            return id;
        }
    }
}

string readName() {
    while (true) {
        string name = readLine("Full name                   : ");
        if (isValidName(name)) {
            return name;
        }
        cout << "  Invalid name. Use 2-30 letters (spaces and dots allowed).\n";
    }
}

string readBloodGroup() {
    while (true) {
        string group = toUpperCase(readLine("Blood group (e.g. B+)       : "));
        if (isValidBloodGroup(group)) {
            return group;
        }
        cout << "  Invalid blood group. Enter one of A+ A- B+ B- AB+ AB- O+ O-.\n";
    }
}

string readPhone() {
    while (true) {
        string phone = readLine("Mobile number (10 digits)   : ");
        if (isValidPhone(phone)) {
            return phone;
        }
        cout << "  Invalid number. Enter 10 digits starting with 6, 7, 8 or 9.\n";
    }
}

string readCity() {
    while (true) {
        string city = readLine("City                        : ");
        if (isValidCity(city)) {
            return city;
        }
        cout << "  Invalid city. Use 2-25 letters and spaces.\n";
    }
}

Date readLastDonationDate(const Date& today) {
    while (true) {
        string text = readLine("Last donation (DD-MM-YYYY)  : ");
        Date d;
        if (!parseDate(text, d)) {
            cout << "  Invalid date. Use DD-MM-YYYY with a real calendar date.\n";
        } else if (toDayNumber(d) > toDayNumber(today)) {
            cout << "  Last donation date cannot be in the future.\n";
        } else {
            return d;
        }
    }
}

// ===================== Menu operations =====================

void addDonor(Donor donors[], const Date& today) {
    int count = Donor::getDonorCount();
    if (count >= MAX_DONORS) {
        cout << "\n  Donor list is full (" << MAX_DONORS << " donors). Cannot add more.\n";
        return;
    }

    cout << "\n--- Add New Donor ---   (enter 0 as Donor ID to cancel)\n";
    string id = readNewDonorId(donors, count);
    if (id == "0") {
        cout << "  Cancelled. No donor added.\n";
        return;
    }

    string name = readName();
    char gender = readChoice("Sex (M/F)                   : ", "MF");
    int age = readInt("Age in years (18-65)        : ", Donor::MIN_AGE, Donor::MAX_AGE);

    char donatedBefore = readChoice("Donated blood before? (Y/N) : ", "YN");
    bool hasDonated = (donatedBefore == 'Y');
    Date lastDonation = {0, 0, 0};

    if (hasDonated) {
        lastDonation = readLastDonationDate(today);
    } else if (age > Donor::MAX_FIRST_TIME_AGE) {
        // NBTC 2017: a first-time donor must not be older than 60
        cout << "  Not registered: first-time donors must be "
             << Donor::MAX_FIRST_TIME_AGE << " years or younger.\n";
        return;
    }

    int weight = readInt("Weight in kg (45-200)       : ", Donor::MIN_WEIGHT, Donor::MAX_WEIGHT);
    string group = readBloodGroup();
    string phone = readPhone();
    string city = readCity();

    // Next free slot in the array of objects, reached through a pointer
    Donor* slot = &donors[count];
    slot->setDetails(id, name, age, gender, weight, group, phone, city, hasDonated, lastDonation);
    Donor::increaseDonorCount();

    cout << "\n  Donor " << id << " registered successfully.\n";
    cout << "  Status: " << Donor::statusText(daysUntilEligible(*slot, today)) << "\n";
}

void showAllDonors(Donor donors[], const Date& today) {
    int count = Donor::getDonorCount();
    cout << "\n--- All Donors (" << count << ") ---\n";
    if (count == 0) {
        cout << "  No donors registered yet.\n";
        return;
    }
    printTableHeader();
    for (Donor* p = donors; p < donors + count; p++) {
        p->displayRow(today);
    }
}

void searchById(Donor donors[], const Date& today) {
    cout << "\n--- Search Donor by ID ---\n";
    if (Donor::getDonorCount() == 0) {
        cout << "  No donors registered yet.\n";
        return;
    }
    string id = toUpperCase(readLine("Enter Donor ID : "));
    Donor* found = findDonorById(donors, Donor::getDonorCount(), id);
    if (found == nullptr) {
        cout << "  No donor found with ID " << id << ".\n";
        return;
    }
    cout << "\n";
    found->display(today);
}

void showReminders(Donor donors[], const Date& today) {
    int count = Donor::getDonorCount();
    cout << "\n--- Eligibility Reminders for " << formatDate(today) << " ---\n";
    if (count == 0) {
        cout << "  No donors registered yet.\n";
        return;
    }

    // First pass: donors who can donate today
    int eligibleCount = 0;
    for (Donor* p = donors; p < donors + count; p++) {
        if (daysUntilEligible(*p, today) <= 0) {
            eligibleCount++;
        }
    }
    cout << "\nEligible to donate today: " << eligibleCount << "\n";
    if (eligibleCount > 0) {
        printTableHeader();
        for (Donor* p = donors; p < donors + count; p++) {
            if (daysUntilEligible(*p, today) <= 0) {
                p->displayRow(today);
            }
        }
    }

    // Second pass: donors who become eligible within the reminder window
    int dueCount = 0;
    for (Donor* p = donors; p < donors + count; p++) {
        int daysLeft = daysUntilEligible(*p, today);
        if (daysLeft >= 1 && daysLeft <= Donor::REMINDER_DAYS) {
            dueCount++;
        }
    }
    cout << "\nBecoming eligible in the next " << Donor::REMINDER_DAYS << " days: " << dueCount << "\n";
    if (dueCount > 0) {
        printTableHeader();
        for (Donor* p = donors; p < donors + count; p++) {
            int daysLeft = daysUntilEligible(*p, today);
            if (daysLeft >= 1 && daysLeft <= Donor::REMINDER_DAYS) {
                p->displayRow(today);
            }
        }
    }
}

void searchByBloodGroup(Donor donors[], const Date& today) {
    int count = Donor::getDonorCount();
    cout << "\n--- Find Donors by Blood Group ---\n";
    if (count == 0) {
        cout << "  No donors registered yet.\n";
        return;
    }
    string group = readBloodGroup();

    int matchCount = 0;
    int eligibleNow = 0;
    for (Donor* p = donors; p < donors + count; p++) {
        if (p->getBloodGroup() == group) {
            if (matchCount == 0) {
                printTableHeader();
            }
            p->displayRow(today);
            matchCount++;
            if (daysUntilEligible(*p, today) <= 0) {
                eligibleNow++;
            }
        }
    }
    if (matchCount == 0) {
        cout << "  No donors with blood group " << group << ".\n";
    } else {
        cout << "\n  " << eligibleNow << " of " << matchCount << " " << group
             << " donor(s) can donate today.\n";
    }
}

void showMenu(const Date& today) {
    cout << "\n";
    printLine('=', 60);
    cout << "        BLOOD DONOR ELIGIBILITY REMINDER CONSOLE\n";
    printLine('=', 60);
    cout << "  Today: " << formatDate(today)
         << "            Donors registered: " << Donor::getDonorCount()
         << "/" << MAX_DONORS << "\n";
    printLine('-', 60);
    cout << "  1. Add new donor\n";
    cout << "  2. Show all donors\n";
    cout << "  3. Search donor by ID\n";
    cout << "  4. Eligibility reminders (today + next " << Donor::REMINDER_DAYS << " days)\n";
    cout << "  5. Find donors by blood group\n";
    cout << "  0. Exit\n";
    printLine('-', 60);
}

// ===================== main =====================

int main(int argc, char* argv[]) {
    Donor donors[MAX_DONORS];   // array of objects: in-memory storage for CA1
    Date today = getToday();

    // Optional test date so that test runs give the same results on any day
    if (argc > 1) {
        Date testDate;
        if (parseDate(argv[1], testDate)) {
            today = testDate;
            cout << "(Test mode: today's date is set to " << formatDate(today) << ")\n";
        } else {
            cout << "(Ignoring invalid date argument. Use DD-MM-YYYY.)\n";
        }
    }

    int choice;
    do {
        showMenu(today);
        choice = readInt("Enter choice: ", 0, 5);
        switch (choice) {
            case 1: addDonor(donors, today); break;
            case 2: showAllDonors(donors, today); break;
            case 3: searchById(donors, today); break;
            case 4: showReminders(donors, today); break;
            case 5: searchByBloodGroup(donors, today); break;
            case 0: cout << "\nThank you. Exiting program.\n"; break;
        }
    } while (choice != 0);

    return 0;
}
