
#ifndef PAYMENT_RENEWAL_H
#define PAYMENT_RENEWAL_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Stores payment information of each gym member
struct MemberPayment {
    int memberID;
    string memberName;
    string membershipPlan;
    double monthlyFee;
    string nextPaymentDate;
    bool directDebitEnabled;
    bool debitApproved;
};

// Payment Renewal Engine Class
class PaymentRenewal {

private:
    vector<MemberPayment> members;

    // Adds one month to a date (YYYY-MM-DD)
    string addOneMonth(string date);

public:
    // Add a new member
    void addMember(MemberPayment member);

    // Display all members
    void displayMembers();

    // Process monthly renewals
    void processRenewals(string currentDate);

    // Display overdue payment notices
    void showOverdueMembers(string currentDate);

    // Find a member using ID
    int findMember(int id);
};

#endif
