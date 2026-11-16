// Donor.cpp
// Code for the Donor class functions

#include "Donor.h"
#include <iostream>
#include <iomanip>
#include <cstring>

// static data members are defined once outside the class
int Donor::donorCount = 0;
int Donor::objectCount = 0;

// ================= constructors and destructor =================

// default constructor: empty donor
Donor::Donor() {
    id = "";
    name = "";
    age = 0;
    gender = 'M';
    weight = 0;
    bloodGroup = "";
    phone = "";
    city = "";
    donatedBefore = false;
    lastDonation = {0, 0, 0};
    objectCount++;
}

// parameterised constructor
// parameter names are same as member names, so I use this-> to tell them apart
Donor::Donor(string id, string name, int age, char gender, int weight,
             string bloodGroup, string phone, string city,
             bool donatedBefore, Date lastDonation) {
    this->id = id;
    this->name = name;
    this->age = age;
    this->gender = gender;
    this->weight = weight;
    this->bloodGroup = bloodGroup;
    this->phone = phone;
    this->city = city;
    this->donatedBefore = donatedBefore;
    this->lastDonation = lastDonation;
    objectCount++;
}

// copy constructor: makes a new donor with the same details as other
Donor::Donor(const Donor &other) {
    id = other.id;
    name = other.name;
    age = other.age;
    gender = other.gender;
    weight = other.weight;
    bloodGroup = other.bloodGroup;
    phone = other.phone;
    city = other.city;
    donatedBefore = other.donatedBefore;
    lastDonation = other.lastDonation;
    objectCount++;
}

// destructor: runs when the object is destroyed
Donor::~Donor() {
    objectCount--;
}

// ================= other member functions =================

void Donor::setLastDonation(Date d) {
    donatedBefore = true;
    lastDonation = d;
}

Date Donor::getNextEligibleDate() const {
    return daysToDate(dateToDays(lastDonation) + getGapDays());
}

string Donor::getStatus(int daysLeft) {
    if (daysLeft <= 0)
        return "Eligible now";
    if (daysLeft == 1)
        return "Due in 1 day";
    if (daysLeft <= REMINDER_DAYS)
        return "Due in " + to_string(daysLeft) + " days";
    return "Wait " + to_string(daysLeft) + " days";
}

void Donor::showDetails(Date today) const {
    cout << "  Donor ID      : " << id << "\n";
    cout << "  Name          : " << name << "\n";
    cout << "  Age / Sex     : " << age << " / " << gender << "\n";
    cout << "  Weight        : " << weight << " kg\n";
    cout << "  Blood group   : " << bloodGroup << "\n";
    cout << "  Phone         : " << phone << "\n";
    cout << "  City          : " << city << "\n";
    if (isFirstTime()) {
        cout << "  Last donation : Never (first time donor)\n";
        cout << "  Next eligible : Any time\n";
    } else {
        cout << "  Last donation : " << dateToString(lastDonation) << "\n";
        cout << "  Next eligible : " << dateToString(getNextEligibleDate())
             << "  (gap of " << getGapDays() << " days)\n";
    }
    // *this = the current object, passed to the friend function
    cout << "  Status        : " << getStatus(daysLeft(*this, today)) << "\n";
}

void Donor::showRow(Date today) const {
    string last = "Never";
    string next = "Any time";
    if (!isFirstTime()) {
        last = dateToString(lastDonation);
        next = dateToString(getNextEligibleDate());
    }
    cout << left
         << setw(7) << id
         << setw(20) << name.substr(0, 19)
         << setw(5) << age
         << setw(4) << gender
         << setw(7) << bloodGroup
         << setw(12) << phone
         << setw(14) << last
         << setw(14) << next
         << getStatus(daysLeft(*this, today)) << "\n";
}

// ================= CA2: file record =================

DonorRecord Donor::toRecord() const {
    DonorRecord r;
    memset(&r, 0, sizeof(r));   // fill with zeros first
    // lengths are already checked when the donor was added, so they fit
    strcpy(r.id, id.c_str());
    strcpy(r.name, name.c_str());
    strcpy(r.bloodGroup, bloodGroup.c_str());
    strcpy(r.phone, phone.c_str());
    strcpy(r.city, city.c_str());
    r.age = age;
    r.gender = gender;
    r.weight = weight;
    r.donatedBefore = donatedBefore;
    r.lastDonation = lastDonation;
    return r;
}

void Donor::fromRecord(const DonorRecord &r) {
    id = r.id;            // char array -> string
    name = r.name;
    bloodGroup = r.bloodGroup;
    phone = r.phone;
    city = r.city;
    age = r.age;
    gender = r.gender;
    weight = r.weight;
    donatedBefore = r.donatedBefore;
    lastDonation = r.lastDonation;
}

// ================= friend function =================

// Returns how many days are left before the donor can donate again.
// 0 or less means the donor can donate today.
int daysLeft(const Donor &d, Date today) {
    if (!d.donatedBefore)
        return 0;   // first time donor, no waiting
    long nextDay = dateToDays(d.lastDonation) + d.getGapDays();
    return (int)(nextDay - dateToDays(today));
}
