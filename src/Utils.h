// Utils.h
// Small helper functions: dates, checking input, and reading input safely.

#ifndef UTILS_H
#define UTILS_H

#include <string>
using namespace std;

// A simple date (DD-MM-YYYY)
struct Date {
    int day;
    int month;
    int year;
};

// ---- date functions ----
bool isLeapYear(int year);
int daysInMonth(int month, int year);
bool stringToDate(string text, Date &d);   // "05-11-2026" -> Date, false if wrong
long dateToDays(Date d);                   // number of days from 01-01-1900
Date daysToDate(long days);                // opposite of dateToDays
string dateToString(Date d);               // Date -> "05-11-2026"
Date getTodayDate();                       // today's date from the computer

// ---- string functions ----
string toUpper(string text);
bool isNumber(string text);                // true if text has only digits

// ---- validation functions ----
bool isValidId(string id);                 // D + 4 digits, like D1001
bool isValidName(string name);             // letters, spaces and dots
bool isValidCity(string city);             // letters and spaces
bool isValidPhone(string phone);           // 10 digits, starts with 6-9
bool isValidBloodGroup(string group);      // A+ A- B+ B- AB+ AB- O+ O-

// ---- input functions (keep asking until the input is correct) ----
string inputText(string message);
int inputNumber(string message, int min, int max);
char inputChoice(string message, string allowed);   // allowed = "MF" or "YN"

#endif
