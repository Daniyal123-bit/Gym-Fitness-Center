#include <iostream>
#include <string>
using namespace std;

class Trainer {
private:
    int trainerID;
    string trainerName;
    string specialization;
    int experience;

public:
    void setData(int id, string name, string spec, int exp) {
        trainerID = id;
        trainerName = name;
        specialization = spec;
        experience = exp;
    }

    void displayData() {
        cout << "Trainer ID: " << trainerID << endl;
        cout << "Trainer Name: " << trainerName << endl;
        cout << "Specialization: " << specialization << endl;
        cout << "Experience: " << experience << " years" << endl;
    }
};

int main() {
    Trainer t1;

    t1.setData(101, "Ali", "Fitness Training", 3);

    t1.displayData();

    return 0;
}
