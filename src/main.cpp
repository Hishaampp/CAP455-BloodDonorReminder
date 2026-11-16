// main.cpp
// Blood Donor Eligibility Reminder Console - CAP455 Minor Project
// Hisham P P, 12604325
//
// CA1: add donor, show all, search, reminders, find by blood group (array of objects)
// CA2: donors are saved in data/donors.dat (binary file), search with seekg,
//      update with seekp, data is loaded again when the program starts
//
// Build: g++ -std=c++17 -Wall src/main.cpp src/Donor.cpp src/Utils.cpp src/FileHandler.cpp -o donor_app
// Run  : ./donor_app              (uses today's date)
//        ./donor_app 05-11-2026   (test mode: uses this date as today)

#include <iostream>
#include <iomanip>
#include <string>
#include "Donor.h"
#include "Utils.h"
#include "FileHandler.h"
using namespace std;

void printLine(char ch, int length) {
    for (int i = 0; i < length; i++)
        cout << ch;
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
         << "Status\n";
    printLine('-', 99);
}

// Search the array using a pointer. Returns the address of the donor or nullptr.
Donor *findDonor(Donor *list, int count, string id) {
    for (Donor *p = list; p < list + count; p++) {
        if (p->getId() == id)
            return p;
    }
    return nullptr;
}

// ================= input for each field =================

string inputNewId(Donor donors[], int count) {
    while (true) {
        string id = toUpper(inputText("Donor ID (e.g. D1001)       : "));
        if (id == "0")
            return id;   // 0 = cancel
        if (!isValidId(id))
            cout << "  Wrong ID. Use D and 4 digits, e.g. D1001.\n";
        else if (findDonor(donors, count, id) != nullptr)
            cout << "  Donor ID " << id << " already exists. Enter a different ID.\n";
        else
            return id;
    }
}

string inputName() {
    while (true) {
        string name = inputText("Full name                   : ");
        if (isValidName(name))
            return name;
        cout << "  Wrong name. Use 2-30 letters (spaces and dots allowed).\n";
    }
}

string inputBloodGroup() {
    while (true) {
        string group = toUpper(inputText("Blood group (e.g. B+)       : "));
        if (isValidBloodGroup(group))
            return group;
        cout << "  Wrong blood group. Enter one of A+ A- B+ B- AB+ AB- O+ O-.\n";
    }
}

string inputPhone() {
    while (true) {
        string phone = inputText("Mobile number (10 digits)   : ");
        if (isValidPhone(phone))
            return phone;
        cout << "  Wrong number. Enter 10 digits starting with 6, 7, 8 or 9.\n";
    }
}

string inputCity() {
    while (true) {
        string city = inputText("City                        : ");
        if (isValidCity(city))
            return city;
        cout << "  Wrong city. Use 2-25 letters and spaces.\n";
    }
}

Date inputDonationDate(Date today) {
    while (true) {
        string text = inputText("Last donation (DD-MM-YYYY)  : ");
        Date d;
        if (!stringToDate(text, d))
            cout << "  Wrong date. Use DD-MM-YYYY with a real date.\n";
        else if (dateToDays(d) > dateToDays(today))
            cout << "  Donation date cannot be in the future.\n";
        else
            return d;
    }
}

// ================= menu options =================

// 1. Add a new donor (Create)
void addDonor(Donor donors[], Date today) {
    int count = Donor::getDonorCount();
    if (count >= MAX_DONORS) {
        cout << "\n  Donor list is full (" << MAX_DONORS << " donors).\n";
        return;
    }

    cout << "\n--- Add New Donor ---   (enter 0 as Donor ID to cancel)\n";
    string id = inputNewId(donors, count);
    if (id == "0") {
        cout << "  Cancelled. No donor added.\n";
        return;
    }

    string name = inputName();
    char gender = inputChoice("Sex (M/F)                   : ", "MF");
    int age = inputNumber("Age in years (18-65)        : ", Donor::MIN_AGE, Donor::MAX_AGE);

    char answer = inputChoice("Donated blood before? (Y/N) : ", "YN");
    bool donatedBefore = (answer == 'Y');
    Date lastDonation = {0, 0, 0};

    if (donatedBefore) {
        lastDonation = inputDonationDate(today);
    } else if (age > Donor::FIRST_TIME_MAX_AGE) {
        cout << "  Not registered: first time donors must be "
             << Donor::FIRST_TIME_MAX_AGE << " years or younger.\n";
        return;
    }

    int weight = inputNumber("Weight in kg (45-200)       : ", Donor::MIN_WEIGHT, Donor::MAX_WEIGHT);
    string group = inputBloodGroup();
    string phone = inputPhone();
    string city = inputCity();

    // parameterised constructor
    Donor newDonor(id, name, age, gender, weight, group, phone, city, donatedBefore, lastDonation);

    // save to the file first, so we never have a donor only in memory
    if (!saveDonor(DATA_FILE, newDonor)) {
        cout << "  Error: could not open " << DATA_FILE << ". Is the data folder there?\n";
        return;
    }

    donors[count] = newDonor;   // also keep it in the array
    Donor::addDonorCount();

    cout << "\n  Donor " << id << " registered and saved to file.\n";
    cout << "  Status: " << Donor::getStatus(daysLeft(newDonor, today)) << "\n";
}

// 2. Show all donors
void showAllDonors(Donor donors[], Date today) {
    int count = Donor::getDonorCount();
    cout << "\n--- All Donors (" << count << ") ---\n";
    if (count == 0) {
        cout << "  No donors registered yet.\n";
        return;
    }
    printTableHeader();
    for (Donor *p = donors; p < donors + count; p++)
        p->showRow(today);
}

// 3. Search by ID - reads straight from the file using seekg
void searchById(Date today) {
    cout << "\n--- Search Donor by ID ---\n";
    if (Donor::getDonorCount() == 0) {
        cout << "  No donors registered yet.\n";
        return;
    }
    string id = toUpper(inputText("Enter Donor ID : "));
    int position = findPositionInFile(DATA_FILE, id);
    if (position == -1) {
        cout << "  No donor found with ID " << id << ".\n";
        return;
    }

    Donor d;
    readDonorAt(DATA_FILE, position, d);
    cout << "  Found at record number " << position + 1 << " in the file.\n\n";
    d.showDetails(today);
}

// 4. Reminders: who can donate today and who can donate in the next 7 days
void showReminders(Donor donors[], Date today) {
    int count = Donor::getDonorCount();
    cout << "\n--- Eligibility Reminders for " << dateToString(today) << " ---\n";
    if (count == 0) {
        cout << "  No donors registered yet.\n";
        return;
    }

    int eligible = 0;
    for (int i = 0; i < count; i++) {
        if (daysLeft(donors[i], today) <= 0)
            eligible++;
    }
    cout << "\nEligible to donate today: " << eligible << "\n";
    if (eligible > 0) {
        printTableHeader();
        for (int i = 0; i < count; i++) {
            if (daysLeft(donors[i], today) <= 0)
                donors[i].showRow(today);
        }
    }

    int dueSoon = 0;
    for (int i = 0; i < count; i++) {
        int left = daysLeft(donors[i], today);
        if (left >= 1 && left <= Donor::REMINDER_DAYS)
            dueSoon++;
    }
    cout << "\nBecoming eligible in the next " << Donor::REMINDER_DAYS << " days: " << dueSoon << "\n";
    if (dueSoon > 0) {
        printTableHeader();
        for (int i = 0; i < count; i++) {
            int left = daysLeft(donors[i], today);
            if (left >= 1 && left <= Donor::REMINDER_DAYS)
                donors[i].showRow(today);
        }
    }
}

// 5. Find donors by blood group
void searchByBloodGroup(Donor donors[], Date today) {
    int count = Donor::getDonorCount();
    cout << "\n--- Find Donors by Blood Group ---\n";
    if (count == 0) {
        cout << "  No donors registered yet.\n";
        return;
    }
    string group = inputBloodGroup();

    int found = 0;
    int canDonate = 0;
    for (int i = 0; i < count; i++) {
        if (donors[i].getBloodGroup() == group) {
            if (found == 0)
                printTableHeader();
            donors[i].showRow(today);
            found++;
            if (daysLeft(donors[i], today) <= 0)
                canDonate++;
        }
    }
    if (found == 0)
        cout << "  No donors with blood group " << group << ".\n";
    else
        cout << "\n  " << canDonate << " of " << found << " " << group << " donor(s) can donate today.\n";
}

// 6. Update a donor (CA2) - finds the record with seekg and overwrites it with seekp
void updateDonor(Donor donors[], Date today) {
    cout << "\n--- Update Donor ---\n";
    if (Donor::getDonorCount() == 0) {
        cout << "  No donors registered yet.\n";
        return;
    }
    string id = toUpper(inputText("Enter Donor ID : "));
    int position = findPositionInFile(DATA_FILE, id);
    if (position == -1) {
        cout << "  No donor found with ID " << id << ".\n";
        return;
    }

    Donor d;
    readDonorAt(DATA_FILE, position, d);
    d.showDetails(today);

    Donor before(d);   // copy constructor: keep the old details for the message

    cout << "\n  What do you want to update?\n";
    cout << "  1. Phone number\n";
    cout << "  2. City\n";
    cout << "  3. Record a new donation (last donation date)\n";
    cout << "  0. Cancel\n";
    int option = inputNumber("Enter option: ", 0, 3);

    if (option == 0) {
        cout << "  Cancelled. Nothing changed.\n";
        return;
    } else if (option == 1) {
        d.setPhone(inputPhone());
    } else if (option == 2) {
        d.setCity(inputCity());
    } else {
        d.setLastDonation(inputDonationDate(today));
    }

    char sure = inputChoice("Save this change? (Y/N)     : ", "YN");
    if (sure == 'N') {
        cout << "  Cancelled. Nothing changed.\n";
        return;
    }

    if (!updateDonorAt(DATA_FILE, position, d)) {
        cout << "  Error: could not write to " << DATA_FILE << ".\n";
        return;
    }
    donors[position] = d;   // keep the array same as the file

    cout << "\n  Donor " << id << " updated in the file.\n";
    if (option == 1)
        cout << "  Phone: " << before.getPhone() << " -> " << d.getPhone() << "\n";
    else if (option == 2)
        cout << "  City: " << before.getCity() << " -> " << d.getCity() << "\n";
    else
        cout << "  New status: " << Donor::getStatus(daysLeft(d, today)) << "\n";
}

void showMenu(Date today) {
    cout << "\n";
    printLine('=', 60);
    cout << "        BLOOD DONOR ELIGIBILITY REMINDER CONSOLE\n";
    printLine('=', 60);
    cout << "  Today: " << dateToString(today)
         << "            Donors registered: " << Donor::getDonorCount()
         << "/" << MAX_DONORS << "\n";
    printLine('-', 60);
    cout << "  1. Add new donor\n";
    cout << "  2. Show all donors\n";
    cout << "  3. Search donor by ID\n";
    cout << "  4. Eligibility reminders (today + next " << Donor::REMINDER_DAYS << " days)\n";
    cout << "  5. Find donors by blood group\n";
    cout << "  6. Update donor\n";
    cout << "  0. Exit\n";
    printLine('-', 60);
}

int main(int argc, char *argv[]) {
    Donor donors[MAX_DONORS];   // array of objects (default constructor runs for each)
    Date today = getTodayDate();

    // test mode: give a date so test runs show the same result on any day
    if (argc > 1) {
        Date testDate;
        if (stringToDate(argv[1], testDate)) {
            today = testDate;
            cout << "(Test mode: today's date is set to " << dateToString(today) << ")\n";
        } else {
            cout << "(Wrong date given, using today's date. Use DD-MM-YYYY.)\n";
        }
    }

    // CA2: load saved donors from the file
    int loaded = loadAllDonors(DATA_FILE, donors, MAX_DONORS);
    for (int i = 0; i < loaded; i++)
        Donor::addDonorCount();
    cout << "Loaded " << loaded << " donor(s) from " << DATA_FILE << "\n";

    int choice;
    do {
        showMenu(today);
        choice = inputNumber("Enter choice: ", 0, 6);
        switch (choice) {
            case 1: addDonor(donors, today); break;
            case 2: showAllDonors(donors, today); break;
            case 3: searchById(today); break;
            case 4: showReminders(donors, today); break;
            case 5: searchByBloodGroup(donors, today); break;
            case 6: updateDonor(donors, today); break;
            case 0: cout << "\nData is saved in " << DATA_FILE << ". Bye!\n"; break;
        }
    } while (choice != 0);

    return 0;
}
