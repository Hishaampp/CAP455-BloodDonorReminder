/*
 * File    : Utils.cpp
 * Project : Blood Donor Eligibility Reminder Console (CAP455 Minor Project)
 * Phase   : CA1 - Design & Create
 * Purpose : Implementation of the helpers declared in Utils.h.
 */
#include "Utils.h"

#include <iostream>
#include <cctype>
#include <cstdlib>
#include <ctime>

using namespace std;

// The eight blood groups accepted by the system
const int BLOOD_GROUP_COUNT = 8;
const string BLOOD_GROUPS[BLOOD_GROUP_COUNT] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};

// ===================== Date helpers =====================

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(int month, int year) {
    if (month == 2) {
        return isLeapYear(year) ? 29 : 28;
    }
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    }
    return 31;
}

// Converts "DD-MM-YYYY" into a Date. Returns false if the text is not a real date.
bool parseDate(const string& text, Date& result) {
    if (text.length() != 10 || text[2] != '-' || text[5] != '-') {
        return false;
    }
    string dd = text.substr(0, 2);
    string mm = text.substr(3, 2);
    string yyyy = text.substr(6, 4);
    if (!isAllDigits(dd) || !isAllDigits(mm) || !isAllDigits(yyyy)) {
        return false;
    }

    int day = atoi(dd.c_str());
    int month = atoi(mm.c_str());
    int year = atoi(yyyy.c_str());

    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > daysInMonth(month, year)) return false;

    result.day = day;
    result.month = month;
    result.year = year;
    return true;
}

// Counts days from 01-01-1900 (which is day 1). Two dates can then be
// subtracted to get the number of days between them.
long toDayNumber(const Date& d) {
    long days = 0;
    for (int y = 1900; y < d.year; y++) {
        days += isLeapYear(y) ? 366 : 365;
    }
    for (int m = 1; m < d.month; m++) {
        days += daysInMonth(m, d.year);
    }
    days += d.day;
    return days;
}

Date fromDayNumber(long dayNumber) {
    Date d;
    d.year = 1900;
    while (dayNumber > (isLeapYear(d.year) ? 366 : 365)) {
        dayNumber -= isLeapYear(d.year) ? 366 : 365;
        d.year++;
    }
    d.month = 1;
    while (dayNumber > daysInMonth(d.month, d.year)) {
        dayNumber -= daysInMonth(d.month, d.year);
        d.month++;
    }
    d.day = (int)dayNumber;
    return d;
}

string formatDate(const Date& d) {
    string text;
    if (d.day < 10) text += "0";
    text += to_string(d.day) + "-";
    if (d.month < 10) text += "0";
    text += to_string(d.month) + "-";
    text += to_string(d.year);
    return text;
}

Date getToday() {
    time_t now = time(0);
    tm* local = localtime(&now);
    Date today;
    today.day = local->tm_mday;
    today.month = local->tm_mon + 1;      // tm_mon counts from 0
    today.year = local->tm_year + 1900;   // tm_year counts from 1900
    return today;
}

// ===================== String helpers =====================

string trim(const string& text) {
    size_t start = text.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }
    size_t end = text.find_last_not_of(" \t\r\n");
    return text.substr(start, end - start + 1);
}

string toUpperCase(string text) {
    for (size_t i = 0; i < text.length(); i++) {
        text[i] = (char)toupper((unsigned char)text[i]);
    }
    return text;
}

bool isAllDigits(const string& text) {
    if (text.empty()) {
        return false;
    }
    for (size_t i = 0; i < text.length(); i++) {
        if (!isdigit((unsigned char)text[i])) {
            return false;
        }
    }
    return true;
}

// ===================== Validators =====================

bool isValidDonorId(const string& id) {
    return id.length() == 5 && id[0] == 'D' && isAllDigits(id.substr(1));
}

bool isValidName(const string& name) {
    if (name.length() < 2 || name.length() > 30) return false;
    if (!isalpha((unsigned char)name[0])) return false;   // must start with a letter
    for (size_t i = 0; i < name.length(); i++) {
        char c = name[i];
        if (!isalpha((unsigned char)c) && c != ' ' && c != '.') {
            return false;
        }
    }
    return true;
}

bool isValidCity(const string& city) {
    if (city.length() < 2 || city.length() > 25) return false;
    if (!isalpha((unsigned char)city[0])) return false;
    for (size_t i = 0; i < city.length(); i++) {
        if (!isalpha((unsigned char)city[i]) && city[i] != ' ') {
            return false;
        }
    }
    return true;
}

bool isValidPhone(const string& phone) {
    // Indian mobile numbers: 10 digits starting with 6, 7, 8 or 9
    return phone.length() == 10 && isAllDigits(phone) && phone[0] >= '6';
}

bool isValidBloodGroup(const string& group) {
    for (int i = 0; i < BLOOD_GROUP_COUNT; i++) {
        if (group == BLOOD_GROUPS[i]) {
            return true;
        }
    }
    return false;
}

// ===================== Safe console input =====================

// Reads one full line. getline is used everywhere (instead of cin >>) so that
// leftover newlines never skip the next prompt.
string readLine(const string& prompt) {
    cout << prompt;
    string line;
    if (!getline(cin, line)) {
        cout << "\nInput ended. Exiting program.\n";
        exit(0);
    }
    return trim(line);
}

// Keeps asking until the user types a whole number within [minValue, maxValue].
int readInt(const string& prompt, int minValue, int maxValue) {
    while (true) {
        string text = readLine(prompt);
        if (isAllDigits(text) && text.length() <= 6) {
            int value = atoi(text.c_str());
            if (value >= minValue && value <= maxValue) {
                return value;
            }
        }
        cout << "  Invalid input. Enter a whole number from " << minValue
             << " to " << maxValue << ".\n";
    }
}

// Keeps asking until the user types one of the allowed letters (case-insensitive).
char readChoice(const string& prompt, const string& allowed) {
    while (true) {
        string text = toUpperCase(readLine(prompt));
        if (text.length() == 1 && allowed.find(text[0]) != string::npos) {
            return text[0];
        }
        cout << "  Invalid choice. Enter one of: ";
        for (size_t i = 0; i < allowed.length(); i++) {
            cout << allowed[i] << (i + 1 < allowed.length() ? "/" : "\n");
        }
    }
}
