/*
 * File    : Utils.h
 * Project : Blood Donor Eligibility Reminder Console (CAP455 Minor Project)
 * Phase   : CA1 - Design & Create
 * Purpose : Date helpers, string helpers, input validators and safe input
 *           functions used by the rest of the program.
 */
#ifndef UTILS_H
#define UTILS_H

#include <string>

// A simple calendar date (DD-MM-YYYY)
struct Date {
    int day;
    int month;
    int year;
};

// ---------- Date helpers ----------
bool isLeapYear(int year);
int daysInMonth(int month, int year);
bool parseDate(const std::string& text, Date& result);   // "DD-MM-YYYY" -> Date
long toDayNumber(const Date& d);                          // days counted from 01-01-1900
Date fromDayNumber(long dayNumber);                       // reverse of toDayNumber
std::string formatDate(const Date& d);                    // Date -> "DD-MM-YYYY"
Date getToday();                                          // today's date from the system clock

// ---------- String helpers ----------
std::string trim(const std::string& text);
std::string toUpperCase(std::string text);
bool isAllDigits(const std::string& text);

// ---------- Validators (return true if the value is acceptable) ----------
bool isValidDonorId(const std::string& id);        // D followed by 4 digits, e.g. D1001
bool isValidName(const std::string& name);         // 2-30 letters, spaces or dots
bool isValidCity(const std::string& city);         // 2-25 letters or spaces
bool isValidPhone(const std::string& phone);       // 10 digits, starts with 6-9
bool isValidBloodGroup(const std::string& group);  // A+, A-, B+, B-, AB+, AB-, O+, O-

// ---------- Safe console input ----------
std::string readLine(const std::string& prompt);
int readInt(const std::string& prompt, int minValue, int maxValue);
char readChoice(const std::string& prompt, const std::string& allowed);  // e.g. allowed = "MF"

#endif
