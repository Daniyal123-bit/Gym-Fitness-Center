
#include "../include/adam_payment.h"
#include <sstream>
#include <iomanip>

// Find member by ID
int PaymentRenewal::findMember(int id) {

    for (int i = 0; i < members.size(); i++) {

        if (members[i].memberID == id) {
            return i;
        }
    }

    return -1;
}

// Add new member
void PaymentRenewal::addMember(MemberPayment member) {

    if (findMember(member.memberID) != -1) {
        cout << "Member ID already exists!\n";
        return;
    }

    members.push_back(member);

    cout << "Member added successfully!\n";
}

// Display all members
void PaymentRenewal::displayMembers() {

    cout << "\n===== GYM MEMBERS PAYMENT RECORDS =====\n";

    for (const auto& member : members) {

        cout << "\nMember ID: " << member.memberID;
        cout << "\nName: " << member.memberName;
        cout << "\nPlan: " << member.membershipPlan;

        cout << "\nMonthly Fee: Rs. "
             << fixed << setprecision(2)
             << member.monthlyFee;

        cout << "\nNext Payment Date: "
             << member.nextPaymentDate;

        cout << "\nDirect Debit: "
             << (member.directDebitEnabled ? "Enabled" : "Disabled");

        cout << "\n";
    }
}

// Add one month to the payment date
string PaymentRenewal::addOneMonth(string date) {

    int year, month, day;

    char separator1, separator2;

    stringstream ss(date);

    ss >> year >> separator1 >> month
       >> separator2 >> day;

    month++;

    if (month > 12) {
        month = 1;
        year++;
    }

    int daysInMonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    // Leap year check
    if ((year % 400 == 0) ||
        (year % 4 == 0 && year % 100 != 0)) {
        daysInMonth[1] = 29;
    }

    if (day > daysInMonth[month - 1]) {
        day = daysInMonth[month - 1];
    }

    stringstream result;

    result << setfill('0')
           << setw(4) << year << "-"
           << setw(2) << month << "-"
           << setw(2) << day;

    return result.str();
}

// Process monthly membership renewals
void PaymentRenewal::processRenewals(string currentDate) {

    cout << "\n===== MONTHLY RENEWAL PROCESS =====\n";

    for (auto& member : members) {

        // Check if payment is due
        if (member.nextPaymentDate <= currentDate) {

            cout << "\nMember: "
                 << member.memberName;

            cout << "\nMember ID: "
                 << member.memberID;

            if (member.directDebitEnabled &&
                member.debitApproved) {

                // Successful payment
                cout << "\nDirect Debit: SUCCESS";

                cout << "\nAmount Deducted: Rs. "
                     << member.monthlyFee;

                member.nextPaymentDate =
                    addOneMonth(currentDate);

                cout << "\nMembership Renewed Successfully!";

                cout << "\nNext Payment Date: "
                     << member.nextPaymentDate << "\n";
            }

            else {

                // Failed payment
                cout << "\nDirect Debit: FAILED";

                if (!member.directDebitEnabled) {
                    cout << "\nReason: Direct Debit Disabled";
                }
                else {
                    cout << "\nReason: Payment Declined";
                }

                cout << "\nOVERDUE PAYMENT NOTICE!";
                cout << "\nAmount Due: Rs. "
                     << member.monthlyFee;

                cout << "\nPlease pay your membership fee.\n";
            }
        }
    }
}

// Display members whose payments are overdue
void PaymentRenewal::showOverdueMembers(string currentDate) {

    cout << "\n===== OVERDUE PAYMENT MEMBERS =====\n";

    bool found = false;

    for (const auto& member : members) {

        if (member.nextPaymentDate <= currentDate &&
            (!member.directDebitEnabled ||
             !member.debitApproved)) {

            found = true;

            cout << "\nMember ID: "
                 << member.memberID;

            cout << "\nMember Name: "
                 << member.memberName;

            cout << "\nMembership Plan: "
                 << member.membershipPlan;

            cout << "\nOverdue Amount: Rs. "
                 << member.monthlyFee;

            cout << "\nPayment Status: OVERDUE\n";
        }
    }

    if (!found) {
        cout << "No overdue payments found!\n";
    }
}
