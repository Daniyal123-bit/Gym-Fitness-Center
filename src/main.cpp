
#include <iostream>
#include "../include/adam_payment.h"

using namespace std;

int main() {

    PaymentRenewal paymentSystem;

    // Sample Member 1
    MemberPayment m1 = {
        101,
        "Ali Ahmed",
        "Monthly",
        3000,
        "2026-11-25",
        true,
        true
    };

    // Sample Member 2
    MemberPayment m2 = {
        102,
        "Ahmed Khan",
        "Annual",
        2500,
        "2026-11-20",
        true,
        false
    };

    // Sample Member 3
    MemberPayment m3 = {
        103,
        "Usman Ali",
        "VIP",
        5000,
        "2026-11-25",
        false,
        false
    };

    // Add members to system
    paymentSystem.addMember(m1);
    paymentSystem.addMember(m2);
    paymentSystem.addMember(m3);

    int choice;

    string currentDate;

    cout << "\n====================================";
    cout << "\n   GYM PAYMENT RENEWAL SYSTEM";
    cout << "\n====================================\n";

    cout << "\nEnter Current Date (YYYY-MM-DD): ";
    cin >> currentDate;

    do {

        cout << "\n\n========== MAIN MENU ==========";
        cout << "\n1. Display All Members";
        cout << "\n2. Process Monthly Renewals";
        cout << "\n3. Show Overdue Payments";
        cout << "\n4. Exit";
        cout << "\nEnter your choice: ";

        cin >> choice;

        switch (choice) {

            case 1:
                paymentSystem.displayMembers();
                break;

            case 2:
                paymentSystem.processRenewals(currentDate);
                break;

            case 3:
                paymentSystem.showOverdueMembers(currentDate);
                break;

            case 4:
                cout << "\nThank you for using Gym Payment System!";
                break;

            default:
                cout << "\nInvalid choice! Try again.";
        }

    } while (choice != 4);

    return 0;
}
