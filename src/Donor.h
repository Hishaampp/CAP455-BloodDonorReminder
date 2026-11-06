/*
 * File    : Donor.h
 * Project : Blood Donor Eligibility Reminder Console (CAP455 Minor Project)
 * Phase   : CA1 - Design & Create
 * Purpose : Donor entity class. One Donor object holds one donor's record.
 *
 * Eligibility rules follow the NBTC "Guidelines for Blood Donor Selection &
 * Blood Donor Referral" (2017): age 18-65 (first-time donors up to 60),
 * weight at least 45 kg, and a gap of 90 days (male) or 120 days (female)
 * between two whole blood donations.
 */
#ifndef DONOR_H
#define DONOR_H

#include <string>
#include "Utils.h"

const int MAX_DONORS = 100;   // size of the donor array (in-memory storage for CA1)

class Donor {
private:
    std::string donorId;       // unique key, e.g. D1001
    std::string name;
    int age;
    char gender;               // 'M' or 'F'
    int weight;                // in kg
    std::string bloodGroup;    // A+, A-, B+, B-, AB+, AB-, O+, O-
    std::string phone;         // 10-digit mobile number
    std::string city;
    bool hasDonatedBefore;
    Date lastDonation;         // used only when hasDonatedBefore is true

    static int donorCount;     // how many donors have been registered

public:
    // Rule values shared by every donor, so they are static constants
    static const int MIN_AGE = 18;
    static const int MAX_AGE = 65;
    static const int MAX_FIRST_TIME_AGE = 60;
    static const int MIN_WEIGHT = 45;
    static const int MAX_WEIGHT = 200;
    static const int MALE_GAP_DAYS = 90;
    static const int FEMALE_GAP_DAYS = 120;
    static const int REMINDER_DAYS = 7;   // "due soon" window for reminders

    // Stores all details in this object. Parameter names match the data
    // members, so "this->" is used to tell them apart.
    void setDetails(const std::string& donorId, const std::string& name, int age,
                    char gender, int weight, const std::string& bloodGroup,
                    const std::string& phone, const std::string& city,
                    bool hasDonatedBefore, const Date& lastDonation);

    // Getters defined inside the class body are inline automatically
    std::string getId() const { return donorId; }
    std::string getName() const { return name; }
    std::string getBloodGroup() const { return bloodGroup; }

    // Declared here, defined below the class with the inline keyword
    inline bool isFirstTimeDonor() const;
    inline int getGapDays() const;

    bool hasSameId(const std::string& otherId) const;
    Date getNextEligibleDate() const;
    void display(const Date& today) const;      // full details of one donor
    void displayRow(const Date& today) const;   // one line of the donor table

    // Static members work on the class as a whole, not on one object
    static int getDonorCount() { return donorCount; }
    static void increaseDonorCount() { donorCount++; }
    static std::string statusText(int daysLeft);

    // Friend function: not a member, but allowed to read private data
    friend int daysUntilEligible(const Donor& d, const Date& today);
};

inline bool Donor::isFirstTimeDonor() const {
    return !hasDonatedBefore;
}

inline int Donor::getGapDays() const {
    return (gender == 'M') ? MALE_GAP_DAYS : FEMALE_GAP_DAYS;
}

#endif
