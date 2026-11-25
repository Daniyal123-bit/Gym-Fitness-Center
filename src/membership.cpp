#include "Membership.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
using namespace std;

static bool readDate(string dateText, int &day, int &month, int &year) {
    char first, second;
    stringstream input(dateText);

    if (!(input >> day >> first >> month >> second >> year)) {
        return false;
    }

    if (first != '-' || second != '-' || month < 1 || month > 12 ||
        day < 1 || day > 31) {
        return false;
    }

    return true;
}

static bool isLeapYear(int year) {
    return (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0));
}

static int daysInMonth(int month, int year) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month == 2 && isLeapYear(year)) {
        return 29;
    }

    return days[month - 1];
}

static string makeDate(int day, int month, int year) {
    stringstream output;
    output << setfill('0') << setw(2) << day << "-"
           << setw(2) << month << "-" << year;
    return output.str();
}

static string addMonths(string dateText, int months) {
    int day, month, year;

    if (!readDate(dateText, day, month, year)) {
        return dateText;
    }

    int totalMonths = (year * 12) + (month - 1) + months;
    year = totalMonths / 12;
    month = (totalMonths % 12) + 1;

    int lastDay = daysInMonth(month, year);
    if (day > lastDay) {
        day = lastDay;
    }

    return makeDate(day, month, year);
}

static string addDays(string dateText, int days) {
    int day, month, year;

    if (!readDate(dateText, day, month, year)) {
        return dateText;
    }

    for (int i = 0; i < days; i++) {
        day++;

        if (day > daysInMonth(month, year)) {
            day = 1;
            month++;

            if (month > 12) {
                month = 1;
                year++;
            }
        }
    }

    return makeDate(day, month, year);
}

MemberProfile::MemberProfile() {
    memberID = 0;
    name = "";
    phoneNumber = "";
    email = "";
}

MemberProfile::MemberProfile(int id, string memberName, string phone, string memberEmail) {
    memberID = id;
    name = memberName;
    phoneNumber = phone;
    email = memberEmail;
}

int MemberProfile::getMemberID() {
    return memberID;
}

string MemberProfile::getName() {
    return name;
}

string MemberProfile::getPhoneNumber() {
    return phoneNumber;
}

string MemberProfile::getEmail() {
    return email;
}

void MemberProfile::setPhoneNumber(string phone) {
    phoneNumber = phone;
}

void MemberProfile::setEmail(string memberEmail) {
    email = memberEmail;
}
MembershipPlan::MembershipPlan() {
    planType = "Monthly";
    fee = 3000;
    durationMonths = 1;
    startDate = "01-01-2026";
    expiryDate = addMonths(startDate, durationMonths);
}

MembershipPlan::MembershipPlan(string type, string start) {
    setPlan(type, start);
}

string MembershipPlan::getPlanType() {
    return planType;
}

double MembershipPlan::getFee() {
    return fee;
}

int MembershipPlan::getDurationMonths() {
    return durationMonths;
}

string MembershipPlan::getStartDate() {
    return startDate;
}

string MembershipPlan::getExpiryDate() {
    return expiryDate;
}

void MembershipPlan::setPlan(string type, string start) {
    startDate = start;

    if (type == "Monthly") {
        planType = "Monthly";
        fee = 3000;
        durationMonths = 1;
    } else if (type == "Annual") {
        planType = "Annual";
        fee = 25000;
        durationMonths = 12;
    } else if (type == "VIP") {
        planType = "VIP";
        fee = 40000;
        durationMonths = 12;
    } else {
        planType = "Monthly";
        fee = 3000;
        durationMonths = 1;
        cout << "Invalid plan. Monthly plan assigned by default." << endl;
    }

    expiryDate = addMonths(startDate, durationMonths);
}

void MembershipPlan::extendExpiry(int days) {
    expiryDate = addDays(expiryDate, days);
}
GymMembership::GymMembership(int id, string memberName, string phone, string email,
                             string planType, string startDate) {
    profile = MemberProfile(id, memberName, phone, email);
    plan = MembershipPlan(planType, startDate);
    freezeRequested = false;
    membershipFrozen = false;
}

int GymMembership::getMemberID() {
    return profile.getMemberID();
}

string GymMembership::getMemberName() {
    return profile.getName();
}

string GymMembership::getExpiryDate() {
    return plan.getExpiryDate();
}

void GymMembership::updatePhoneNumber(string phone) {
    profile.setPhoneNumber(phone);
    cout << profile.getName() << "'s phone number updated." << endl;
}

void GymMembership::updateEmail(string email) {
    profile.setEmail(email);
    cout << profile.getName() << "'s email updated." << endl;
}

void GymMembership::changePlan(string planType, string startDate) {
    plan.setPlan(planType, startDate);
    freezeRequested = false;
    membershipFrozen = false;
    cout << profile.getName() << "'s membership plan changed to "
         << plan.getPlanType() << "." << endl;
}

void GymMembership::requestFreeze() {
    if (membershipFrozen) {
        cout << "Membership is already frozen." << endl;
    } else if (freezeRequested) {
        cout << "A freeze request is already pending." << endl;
    } else {
        freezeRequested = true;
        cout << "Freeze request submitted for " << profile.getName() << "." << endl;
    }
}

void GymMembership::approveFreeze(int days) {
    if (!freezeRequested) {
        cout << "There is no pending freeze request." << endl;
        return;
    }

    if (days <= 0) {
        cout << "Freeze days must be greater than zero." << endl;
        return;
    }

    plan.extendExpiry(days);
    freezeRequested = false;
    membershipFrozen = true;

    cout << "Freeze request approved for " << days << " days." << endl;
    cout << "New expiry date: " << plan.getExpiryDate() << endl;
}

void GymMembership::rejectFreeze() {
    if (!freezeRequested) {
        cout << "There is no pending freeze request." << endl;
        return;
    }

    freezeRequested = false;
    cout << "Freeze request rejected for " << profile.getName() << "." << endl;
}

void GymMembership::displayMemberDetails() {
    cout << "----------------------------------------" << endl;
    cout << "Member ID:       " << profile.getMemberID() << endl;
    cout << "Name:            " << profile.getName() << endl;
    cout << "Phone Number:    " << profile.getPhoneNumber() << endl;
    cout << "Email:           " << profile.getEmail() << endl;
    cout << "Membership Plan: " << plan.getPlanType() << endl;
    cout << "Plan Fee:        Rs. " << plan.getFee() << endl;
    cout << "Duration:        " << plan.getDurationMonths() << " month(s)" << endl;
    cout << "Start Date:      " << plan.getStartDate() << endl;
    cout << "Expiry Date:     " << plan.getExpiryDate() << endl;
    cout << "Freeze Request:  " << (freezeRequested ? "Pending" : "No") << endl;
    cout << "Membership:      " << (membershipFrozen ? "Frozen" : "Active") << endl;
    cout << "----------------------------------------" << endl;
}
