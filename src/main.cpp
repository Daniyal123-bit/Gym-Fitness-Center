#include <iostream>
#include "Membership.h"
using namespace std;

int main() {
    GymMembership m1(101, "Ali Ahmed", "0300-1234567",
                     "ali@gmail.com", "Monthly", "25-11-2026");

    GymMembership m2(102, "Sara Khan", "0321-7654321",
                     "sara@gmail.com", "Annual", "25-11-2026");

    GymMembership m3(103, "Usman Malik", "0333-9876543",
                     "usman@gmail.com", "VIP", "25-11-2026");

    m1.displayMemberDetails();
    m2.displayMemberDetails();
    m3.displayMemberDetails();

    m1.updatePhoneNumber("0311-1112233");
    m1.updateEmail("aliahmed@gmail.com");

    m2.requestFreeze();
    m2.approveFreeze(15);

    m3.requestFreeze();
    m3.rejectFreeze();

    m1.changePlan("Annual", "25-11-2026");

    cout << "\n========== UPDATED MEMBER DETAILS ==========" << endl;
    m1.displayMemberDetails();
    m2.displayMemberDetails();
    m3.displayMemberDetails();

    return 0;
}
