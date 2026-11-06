/*
 * File    : Donor.cpp
 * Project : Blood Donor Eligibility Reminder Console (CAP455 Minor Project)
 * Phase   : CA1 - Design & Create
 * Purpose : Member functions, static members and friend function of Donor.
 */
#include "Donor.h"

#include <iostream>
#include <iomanip>

using namespace std;

// A static data member is shared by all objects, so it is defined once here
int Donor::donorCount = 0;

void Donor::setDetails(const string& donorId, const string& name, int age,
                       char gender, int weight, const string& bloodGroup,
                       const string& phone, const string& city,
                       bool hasDonatedBefore, const Date& lastDonation) {
    this->donorId = donorId;
    this->name = name;
    this->age = age;
    this->gender = gender;
    this->weight = weight;
    this->bloodGroup = bloodGroup;
    this->phone = phone;
    this->city = city;
    this->hasDonatedBefore = hasDonatedBefore;
    this->lastDonation = lastDonation;
}

bool Donor::hasSameId(const string& otherId) const {
    return this->donorId == otherId;
}

Date Donor::getNextEligibleDate() const {
    return fromDayNumber(toDayNumber(lastDonation) + getGapDays());
}

string Donor::statusText(int daysLeft) {
    if (daysLeft <= 0) {
        return "Eligible now";
    }
    if (daysLeft == 1) {
        return "Due in 1 day";
    }
    if (daysLeft <= REMINDER_DAYS) {
        return "Due in " + to_string(daysLeft) + " days";
    }
    return "Wait " + to_string(daysLeft) + " days";
}

void Donor::display(const Date& today) const {
    int daysLeft = daysUntilEligible(*this, today);

    cout << "  Donor ID      : " << donorId << "\n";
    cout << "  Name          : " << name << "\n";
    cout << "  Age / Sex     : " << age << " / " << gender << "\n";
    cout << "  Weight        : " << weight << " kg\n";
    cout << "  Blood group   : " << bloodGroup << "\n";
    cout << "  Phone         : " << phone << "\n";
    cout << "  City          : " << city << "\n";
    if (isFirstTimeDonor()) {
        cout << "  Last donation : Never (first-time donor)\n";
        cout << "  Next eligible : Any time\n";
    } else {
        cout << "  Last donation : " << formatDate(lastDonation) << "\n";
        cout << "  Next eligible : " << formatDate(getNextEligibleDate())
             << "  (gap of " << getGapDays() << " days)\n";
    }
    cout << "  Status        : " << statusText(daysLeft) << "\n";
}

void Donor::displayRow(const Date& today) const {
    string lastText = isFirstTimeDonor() ? "Never" : formatDate(lastDonation);
    string nextText = isFirstTimeDonor() ? "Any time" : formatDate(getNextEligibleDate());

    cout << left
         << setw(7) << donorId
         << setw(20) << name.substr(0, 19)
         << setw(5) << age
         << setw(4) << gender
         << setw(7) << bloodGroup
         << setw(12) << phone
         << setw(14) << lastText
         << setw(14) << nextText
         << statusText(daysUntilEligible(*this, today)) << "\n";
}

// Friend function: it reads the private members hasDonatedBefore and
// lastDonation directly. A result of 0 or less means the donor can donate today.
int daysUntilEligible(const Donor& d, const Date& today) {
    if (!d.hasDonatedBefore) {
        return 0;   // first-time donors have no waiting period
    }
    long nextDay = toDayNumber(d.lastDonation) + d.getGapDays();
    return (int)(nextDay - toDayNumber(today));
}
