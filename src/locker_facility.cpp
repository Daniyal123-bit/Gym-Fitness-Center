/*
 * ============================================================
 *  Gym & Fitness Center Subscription Engine
 *  Module 4 - Locker & Facility Allocation
 *  Developed by: Saqib Ali (F2025332008)
 * ============================================================
 *  File: src/locker_facility.cpp
 *  Definitions of all member functions declared in
 *  locker_facility.h (no main() in this file).
 * ============================================================
 */

#include "locker_facility.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;


// ============================================================
// ENUM HELPERS
// ============================================================
string facilityTypeName(FacilityType type) {
    switch (type) {
        case FacilityType::POOL:        return "Pool";
        case FacilityType::SAUNA:       return "Sauna";
        case FacilityType::WEIGHT_ROOM: return "Weight Room";
    }
    return "Unknown";
}

string lockerTypeName(LockerType type) {
    return (type == LockerType::PREMIUM) ? "Premium" : "Standard";
}


// ============================================================
// MEMBER ACCOUNT
// ============================================================
MemberAccount::MemberAccount(int id, const string& memberName, bool isActive)
    : memberId(id), name(memberName), active(isActive) {}

MemberAccount::~MemberAccount() {}

int MemberAccount::getId() const {
    return memberId;
}

string MemberAccount::getName() const {
    return name;
}

bool MemberAccount::isActive() const {
    return active;
}

void MemberAccount::setActive(bool status) {
    active = status;
}

bool MemberAccount::canUsePremiumLocker() const {
    return false;
}


// ---------------- StandardMember ----------------
StandardMember::StandardMember(int id, const string& memberName, bool isActive)
    : MemberAccount(id, memberName, isActive) {}

string StandardMember::getTier() const {
    return "Standard";
}

bool StandardMember::hasFacilityAccess(FacilityType type) const {
    return type == FacilityType::WEIGHT_ROOM;
}


// ---------------- GoldMember ----------------
GoldMember::GoldMember(int id, const string& memberName, bool isActive)
    : MemberAccount(id, memberName, isActive) {}

string GoldMember::getTier() const {
    return "Gold";
}

bool GoldMember::hasFacilityAccess(FacilityType type) const {
    return type == FacilityType::WEIGHT_ROOM || type == FacilityType::POOL;
}


// ---------------- VIPMember ----------------
VIPMember::VIPMember(int id, const string& memberName, bool isActive)
    : MemberAccount(id, memberName, isActive) {}

string VIPMember::getTier() const {
    return "VIP";
}

bool VIPMember::hasFacilityAccess(FacilityType type) const {
    (void)type;             // VIP can access every facility
    return true;
}

bool VIPMember::canUsePremiumLocker() const {
    return true;
}


// ---------------- Factory (links with Module 1 plan names) ----------------
MemberAccount* createMemberByPlan(int id, const string& memberName,
                                  const string& planType, bool isActive) {
    if (planType == "VIP")
        return new VIPMember(id, memberName, isActive);
    if (planType == "Annual")
        return new GoldMember(id, memberName, isActive);
    return new StandardMember(id, memberName, isActive);   // "Monthly" or unknown
}


// ============================================================
// LOCKER
// ============================================================
Locker::Locker(int number, LockerType lockerType)
    : lockerNumber(number), type(lockerType), occupied(false),
      assignedMemberId(-1), pin(0), failedAttempts(0), blocked(false) {}

int Locker::getNumber() const {
    return lockerNumber;
}

LockerType Locker::getType() const {
    return type;
}

bool Locker::isOccupied() const {
    return occupied;
}

int Locker::getAssignedMemberId() const {
    return assignedMemberId;
}

bool Locker::isBlocked() const {
    return blocked;
}

int Locker::getRemainingAttempts() const {
    return MAX_ATTEMPTS - failedAttempts;
}

int Locker::assign(int memberId) {
    occupied = true;
    assignedMemberId = memberId;
    pin = 1000 + rand() % 9000;         // random 4-digit PIN
    failedAttempts = 0;
    blocked = false;
    return pin;
}

void Locker::release() {
    occupied = false;
    assignedMemberId = -1;
    pin = 0;
    failedAttempts = 0;
    blocked = false;
}

bool Locker::unlock(int enteredPin) {
    if (blocked) return false;

    if (enteredPin == pin) {
        failedAttempts = 0;
        return true;
    }

    failedAttempts++;
    if (failedAttempts >= MAX_ATTEMPTS) blocked = true;
    return false;
}

void Locker::resetBlock() {
    blocked = false;
    failedAttempts = 0;
}


// ============================================================
// LOCKER MANAGER
// ============================================================
LockerManager::LockerManager(int standardCount, int premiumCount) {
    srand(static_cast<unsigned>(time(0)));      // different PINs every run

    int number = 1;
    for (int i = 0; i < standardCount; i++)
        lockers.push_back(Locker(number++, LockerType::STANDARD));
    for (int i = 0; i < premiumCount; i++)
        lockers.push_back(Locker(number++, LockerType::PREMIUM));
}

int LockerManager::findLockerByMember(int memberId) const {
    for (size_t i = 0; i < lockers.size(); i++) {
        if (lockers[i].isOccupied() && lockers[i].getAssignedMemberId() == memberId)
            return static_cast<int>(i);
    }
    return -1;
}

int LockerManager::findLockerByNumber(int number) const {
    for (size_t i = 0; i < lockers.size(); i++) {
        if (lockers[i].getNumber() == number)
            return static_cast<int>(i);
    }
    return -1;
}

int LockerManager::assignLocker(const MemberAccount& member, LockerType wanted) {
    if (!member.isActive()) {
        cout << "  [DENIED] " << member.getName()
             << "'s subscription is not active. Locker cannot be assigned.\n";
        return -1;
    }

    if (wanted == LockerType::PREMIUM && !member.canUsePremiumLocker()) {
        cout << "  [DENIED] Premium lockers are for VIP members only. "
             << member.getName() << " is " << member.getTier() << ".\n";
        return -1;
    }

    int existing = findLockerByMember(member.getId());
    if (existing != -1) {
        cout << "  [DENIED] " << member.getName() << " already has locker #"
             << lockers[existing].getNumber() << ". One locker per member.\n";
        return -1;
    }

    for (size_t i = 0; i < lockers.size(); i++) {
        if (!lockers[i].isOccupied() && lockers[i].getType() == wanted) {
            int newPin = lockers[i].assign(member.getId());
            cout << "  [OK] " << lockerTypeName(wanted) << " locker #"
                 << lockers[i].getNumber() << " assigned to " << member.getName()
                 << ". Digital PIN: " << newPin << "\n";
            return newPin;
        }
    }

    cout << "  [FULL] No free " << lockerTypeName(wanted) << " locker available.\n";
    return -1;
}

bool LockerManager::openLocker(const MemberAccount& member, int enteredPin) {
    int idx = findLockerByMember(member.getId());
    if (idx == -1) {
        cout << "  [ERROR] " << member.getName() << " has no locker assigned.\n";
        return false;
    }

    Locker& locker = lockers[idx];

    if (locker.isBlocked()) {
        cout << "  [BLOCKED] Locker #" << locker.getNumber()
             << " is blocked. Please contact the reception.\n";
        return false;
    }

    if (locker.unlock(enteredPin)) {
        cout << "  [OK] Locker #" << locker.getNumber() << " opened.\n";
        return true;
    }

    if (locker.isBlocked())
        cout << "  [BLOCKED] Wrong PIN 3 times. Locker #" << locker.getNumber()
             << " is now blocked.\n";
    else
        cout << "  [WRONG PIN] Attempts left: " << locker.getRemainingAttempts() << "\n";
    return false;
}

bool LockerManager::releaseLocker(const MemberAccount& member) {
    int idx = findLockerByMember(member.getId());
    if (idx == -1) {
        cout << "  [ERROR] " << member.getName() << " has no locker to release.\n";
        return false;
    }

    cout << "  [OK] Locker #" << lockers[idx].getNumber() << " released by "
         << member.getName() << ".\n";
    lockers[idx].release();
    return true;
}

bool LockerManager::staffResetLocker(int lockerNumber) {
    int idx = findLockerByNumber(lockerNumber);
    if (idx == -1) {
        cout << "  [ERROR] Locker #" << lockerNumber << " does not exist.\n";
        return false;
    }

    if (!lockers[idx].isBlocked()) {
        cout << "  [INFO] Locker #" << lockerNumber << " is not blocked.\n";
        return false;
    }

    lockers[idx].resetBlock();
    cout << "  [OK] Locker #" << lockerNumber << " unblocked by staff.\n";
    return true;
}

void LockerManager::showStatus() const {
    cout << "\n  ---------- Locker Status ----------\n";
    int freeCount = 0;

    for (size_t i = 0; i < lockers.size(); i++) {
        const Locker& l = lockers[i];
        cout << "  #" << l.getNumber() << "\t" << lockerTypeName(l.getType()) << "\t";

        if (!l.isOccupied()) {
            cout << "Free";
            freeCount++;
        } else {
            cout << "Member ID " << l.getAssignedMemberId();
            if (l.isBlocked()) cout << " (BLOCKED)";
        }
        cout << "\n";
    }

    cout << "  Free lockers: " << freeCount << " / " << lockers.size() << "\n";
}


// ============================================================
// FACILITY
// ============================================================
Facility::Facility(const string& facilityName, int maxCapacity)
    : name(facilityName), capacity(maxCapacity) {}

Facility::~Facility() {}

string Facility::getName() const {
    return name;
}

int Facility::getCapacity() const {
    return capacity;
}

int Facility::getOccupancy() const {
    return static_cast<int>(membersInside.size());
}

bool Facility::isFull() const {
    return getOccupancy() >= capacity;
}

bool Facility::isInside(int memberId) const {
    for (size_t i = 0; i < membersInside.size(); i++)
        if (membersInside[i] == memberId) return true;
    return false;
}

bool Facility::addMember(int memberId) {
    if (isFull() || isInside(memberId)) return false;
    membersInside.push_back(memberId);
    return true;
}

bool Facility::removeMember(int memberId) {
    for (size_t i = 0; i < membersInside.size(); i++) {
        if (membersInside[i] == memberId) {
            membersInside.erase(membersInside.begin() + i);
            return true;
        }
    }
    return false;
}

void Facility::displayInfo() const {
    cout << "  " << name << " | Occupancy: " << getOccupancy()
         << "/" << capacity << (isFull() ? " (FULL)" : "") << "\n";
    showRules();            // polymorphic call
}


// ---------------- Pool ----------------
Pool::Pool(int maxCapacity, int laneCount, double temperature)
    : Facility("Pool", maxCapacity), lanes(laneCount), waterTemperature(temperature) {}

FacilityType Pool::getType() const {
    return FacilityType::POOL;
}

void Pool::showRules() const {
    cout << "     Lanes: " << lanes << ", Water: " << waterTemperature << " C\n"
         << "     Rules: Swimming cap required, shower before entry.\n";
}


// ---------------- Sauna ----------------
Sauna::Sauna(int maxCapacity, int saunaTemperature, int maxMinutes)
    : Facility("Sauna", maxCapacity), temperature(saunaTemperature),
      maxSessionMinutes(maxMinutes) {}

FacilityType Sauna::getType() const {
    return FacilityType::SAUNA;
}

void Sauna::showRules() const {
    cout << "     Temperature: " << temperature << " C, Max session: "
         << maxSessionMinutes << " min\n"
         << "     Rules: Stay hydrated, towel required.\n";
}


// ---------------- WeightRoom ----------------
WeightRoom::WeightRoom(int maxCapacity, int machines)
    : Facility("Weight Room", maxCapacity), machineCount(machines) {}

FacilityType WeightRoom::getType() const {
    return FacilityType::WEIGHT_ROOM;
}

void WeightRoom::showRules() const {
    cout << "     Machines: " << machineCount << "\n"
         << "     Rules: Re-rack weights, wipe equipment after use.\n";
}


// ============================================================
// FACILITY MANAGER
// ============================================================
FacilityManager::FacilityManager() {}

FacilityManager::~FacilityManager() {
    for (size_t i = 0; i < facilities.size(); i++)
        delete facilities[i];
}

Facility* FacilityManager::findFacility(FacilityType type) const {
    for (size_t i = 0; i < facilities.size(); i++)
        if (facilities[i]->getType() == type) return facilities[i];
    return nullptr;
}

Facility* FacilityManager::facilityMemberIsIn(int memberId) const {
    for (size_t i = 0; i < facilities.size(); i++)
        if (facilities[i]->isInside(memberId)) return facilities[i];
    return nullptr;
}

void FacilityManager::addFacility(Facility* facility) {
    if (facility != nullptr) facilities.push_back(facility);
}

bool FacilityManager::requestEntry(const MemberAccount& member, FacilityType type) {
    Facility* f = findFacility(type);
    if (f == nullptr) {
        cout << "  [ERROR] " << facilityTypeName(type) << " is not available.\n";
        return false;
    }

    if (!member.isActive()) {
        cout << "  [DENIED] " << member.getName() << "'s subscription is not active.\n";
        return false;
    }

    // Polymorphism: result depends on Standard / Gold / VIP
    if (!member.hasFacilityAccess(type)) {
        cout << "  [DENIED] " << member.getTier() << " members cannot access the "
             << f->getName() << ". Upgrade required.\n";
        return false;
    }

    Facility* current = facilityMemberIsIn(member.getId());
    if (current != nullptr) {
        cout << "  [DENIED] " << member.getName() << " is already inside the "
             << current->getName() << ". Please exit first.\n";
        return false;
    }

    if (f->isFull()) {
        cout << "  [FULL] " << f->getName() << " is at full capacity ("
             << f->getCapacity() << "). Please wait.\n";
        return false;
    }

    f->addMember(member.getId());
    cout << "  [GRANTED] " << member.getName() << " (" << member.getTier()
         << ") entered the " << f->getName() << ".\n";
    return true;
}

bool FacilityManager::exitFacility(const MemberAccount& member) {
    Facility* f = facilityMemberIsIn(member.getId());
    if (f == nullptr) {
        cout << "  [ERROR] " << member.getName() << " is not inside any facility.\n";
        return false;
    }

    f->removeMember(member.getId());
    cout << "  [OK] " << member.getName() << " exited the " << f->getName() << ".\n";
    return true;
}

void FacilityManager::showAll() const {
    cout << "\n  ---------- Facility Status ----------\n";
    for (size_t i = 0; i < facilities.size(); i++)
        facilities[i]->displayInfo();       // polymorphic call
}
