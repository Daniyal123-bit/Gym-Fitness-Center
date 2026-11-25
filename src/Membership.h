#ifndef MEMBERSHIP_H
#define MEMBERSHIP_H

#include <string>
using namespace std;
class MemberProfile {
private:
    int memberID;
    string name;
    string phoneNumber;
    string email;

public:
    MemberProfile();
    MemberProfile(int id, string memberName, string phone, string memberEmail);

    int getMemberID();
    string getName();
    string getPhoneNumber();
    string getEmail();

    void setPhoneNumber(string phone);
    void setEmail(string memberEmail);
};
class MembershipPlan {
private:
    string planType;
    double fee;
    int durationMonths;
    string startDate;
    string expiryDate;

public:
    MembershipPlan();
    MembershipPlan(string type, string start);

    string getPlanType();
    double getFee();
    int getDurationMonths();
    string getStartDate();
    string getExpiryDate();

    void setPlan(string type, string start);
    void extendExpiry(int days);
};
class GymMembership {
private:
    MemberProfile profile;
    MembershipPlan plan;
    bool freezeRequested;
    bool membershipFrozen;

public:
    GymMembership(int id, string memberName, string phone, string email,
                  string planType, string startDate);

    int getMemberID();
    string getMemberName();
    string getExpiryDate();

    void updatePhoneNumber(string phone);
    void updateEmail(string email);
    void changePlan(string planType, string startDate);

    void requestFreeze();
    void approveFreeze(int days);
    void rejectFreeze();

    void displayMemberDetails();
};

#endif
