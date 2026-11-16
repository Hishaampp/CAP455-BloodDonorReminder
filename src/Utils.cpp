// Utils.cpp
// Code for the helper functions declared in Utils.h

#include "Utils.h"
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <ctime>

// ================= date functions =================

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(int month, int year) {
    if (month == 2) {
        if (isLeapYear(year))
            return 29;
        return 28;
    }
    if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;
    return 31;
}

// text must look like DD-MM-YYYY and be a real date
bool stringToDate(string text, Date &d) {
    if (text.length() != 10 || text[2] != '-' || text[5] != '-')
        return false;

    string dd = text.substr(0, 2);
    string mm = text.substr(3, 2);
    string yy = text.substr(6, 4);
    if (!isNumber(dd) || !isNumber(mm) || !isNumber(yy))
        return false;

    int day = atoi(dd.c_str());
    int month = atoi(mm.c_str());
    int year = atoi(yy.c_str());

    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > daysInMonth(month, year)) return false;

    d.day = day;
    d.month = month;
    d.year = year;
    return true;
}

// Count days from 01-01-1900. Now two dates can simply be subtracted.
long dateToDays(Date d) {
    long total = 0;
    for (int y = 1900; y < d.year; y++) {
        if (isLeapYear(y))
            total += 366;
        else
            total += 365;
    }
    for (int m = 1; m < d.month; m++) {
        total += daysInMonth(m, d.year);
    }
    total += d.day;
    return total;
}

Date daysToDate(long days) {
    Date d;
    d.year = 1900;
    while (true) {
        int daysInYear = isLeapYear(d.year) ? 366 : 365;
        if (days <= daysInYear)
            break;
        days -= daysInYear;
        d.year++;
    }
    d.month = 1;
    while (days > daysInMonth(d.month, d.year)) {
        days -= daysInMonth(d.month, d.year);
        d.month++;
    }
    d.day = (int)days;
    return d;
}

string dateToString(Date d) {
    string text = "";
    if (d.day < 10) text += "0";
    text += to_string(d.day) + "-";
    if (d.month < 10) text += "0";
    text += to_string(d.month) + "-";
    text += to_string(d.year);
    return text;
}

Date getTodayDate() {
    time_t now = time(0);
    tm *t = localtime(&now);
    Date today;
    today.day = t->tm_mday;
    today.month = t->tm_mon + 1;      // tm_mon starts from 0
    today.year = t->tm_year + 1900;   // tm_year starts from 1900
    return today;
}

// ================= string functions =================

string toUpper(string text) {
    for (int i = 0; i < (int)text.length(); i++) {
        text[i] = toupper(text[i]);
    }
    return text;
}

bool isNumber(string text) {
    if (text.length() == 0)
        return false;
    for (int i = 0; i < (int)text.length(); i++) {
        if (!isdigit(text[i]))
            return false;
    }
    return true;
}

// ================= validation =================

bool isValidId(string id) {
    return id.length() == 5 && id[0] == 'D' && isNumber(id.substr(1));
}

bool isValidName(string name) {
    if (name.length() < 2 || name.length() > 30)
        return false;
    if (!isalpha(name[0]))
        return false;
    for (int i = 0; i < (int)name.length(); i++) {
        if (!isalpha(name[i]) && name[i] != ' ' && name[i] != '.')
            return false;
    }
    return true;
}

bool isValidCity(string city) {
    if (city.length() < 2 || city.length() > 25)
        return false;
    if (!isalpha(city[0]))
        return false;
    for (int i = 0; i < (int)city.length(); i++) {
        if (!isalpha(city[i]) && city[i] != ' ')
            return false;
    }
    return true;
}

bool isValidPhone(string phone) {
    return phone.length() == 10 && isNumber(phone) && phone[0] >= '6';
}

bool isValidBloodGroup(string group) {
    string groups[8] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
    for (int i = 0; i < 8; i++) {
        if (group == groups[i])
            return true;
    }
    return false;
}

// ================= input =================

// I use getline everywhere (not cin >>) so a leftover Enter key
// never skips the next question.
string inputText(string message) {
    cout << message;
    string line;
    if (!getline(cin, line)) {
        cout << "\nNo more input. Closing program.\n";
        exit(0);
    }
    // remove spaces at the start and end
    int start = 0;
    int end = (int)line.length() - 1;
    while (start <= end && isspace(line[start])) start++;
    while (end >= start && isspace(line[end])) end--;
    return line.substr(start, end - start + 1);
}

int inputNumber(string message, int min, int max) {
    while (true) {
        string text = inputText(message);
        if (isNumber(text) && text.length() <= 6) {
            int value = atoi(text.c_str());
            if (value >= min && value <= max)
                return value;
        }
        cout << "  Wrong input. Enter a number from " << min << " to " << max << ".\n";
    }
}

char inputChoice(string message, string allowed) {
    while (true) {
        string text = toUpper(inputText(message));
        if (text.length() == 1 && allowed.find(text[0]) != string::npos)
            return text[0];
        cout << "  Wrong choice. Enter one of: " << allowed << "\n";
    }
}
