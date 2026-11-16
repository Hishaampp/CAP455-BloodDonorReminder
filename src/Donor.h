// Donor.h
// The Donor class. One Donor object = one blood donor.
//
// Rules (NBTC 2017 guidelines):
//   age 18 to 65 (first time donor max 60), weight at least 45 kg,
//   gap between donations: 90 days for male, 120 days for female.

#ifndef DONOR_H
#define DONOR_H

#include <string>
#include "Utils.h"
using namespace std;

const int MAX_DONORS = 100;

// CA2: a fixed size record that is written to the binary file.
// I can't write a string straight to a file because a string only keeps
// a pointer to its text. So I copy everything into char arrays first.
struct DonorRecord {
    char id[6];
    char name[31];
    int age;
    char gender;
    int weight;
    char bloodGroup[4];
    char phone[11];
    char city[26];
    bool donatedBefore;
    Date lastDonation;
};

class Donor {
private:
    string id;            // unique key, like D1001
    string name;
    int age;
    char gender;          // M or F
    int weight;           // kg
    string bloodGroup;
    string phone;
    string city;
    bool donatedBefore;
    Date lastDonation;    // only used if donatedBefore is true

    static int donorCount;    // how many donors are registered
    static int objectCount;   // how many Donor objects exist right now

public:
    // rule values, same for every donor so they are static
    static const int MIN_AGE = 18;
    static const int MAX_AGE = 65;
    static const int FIRST_TIME_MAX_AGE = 60;
    static const int MIN_WEIGHT = 45;
    static const int MAX_WEIGHT = 200;
    static const int MALE_GAP = 90;
    static const int FEMALE_GAP = 120;
    static const int REMINDER_DAYS = 7;

    // ---- CA2: constructors and destructor ----
    Donor();                                    // default constructor
    Donor(string id, string name, int age, char gender, int weight,
          string bloodGroup, string phone, string city,
          bool donatedBefore, Date lastDonation);   // parameterised constructor
    Donor(const Donor &other);                  // copy constructor
    ~Donor();                                   // destructor

    // getters (inline because they are written inside the class)
    string getId() const { return id; }
    string getName() const { return name; }
    string getBloodGroup() const { return bloodGroup; }
    string getPhone() const { return phone; }
    string getCity() const { return city; }

    // setters used by the update option
    void setPhone(string phone) { this->phone = phone; }
    void setCity(string city) { this->city = city; }
    void setLastDonation(Date d);

    inline bool isFirstTime() const;
    inline int getGapDays() const;

    Date getNextEligibleDate() const;
    void showDetails(Date today) const;   // full details of one donor
    void showRow(Date today) const;       // one line in the table

    // CA2: change between Donor object and file record
    DonorRecord toRecord() const;
    void fromRecord(const DonorRecord &r);

    // static functions
    static int getDonorCount() { return donorCount; }
    static void addDonorCount() { donorCount++; }
    static int getObjectCount() { return objectCount; }
    static string getStatus(int daysLeft);

    // friend function: not a member but can read private data
    friend int daysLeft(const Donor &d, Date today);
};

inline bool Donor::isFirstTime() const {
    return !donatedBefore;
}

inline int Donor::getGapDays() const {
    if (gender == 'M')
        return MALE_GAP;
    return FEMALE_GAP;
}

#endif
