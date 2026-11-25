/*
 * ============================================================
 *  Gym & Fitness Center Subscription Engine
 *  Module 4 - Locker & Facility Allocation
 *  Developed by: Saqib Ali (F2025332008)
 *  Team Leader : Daniyal Ahmad (F2025332048)
 * ============================================================
 *  File: src/locker_facility.h
 *  This header only DECLARES the classes of Module 4.
 *  All member functions are DEFINED in locker_facility.cpp.
 *
 *  Classes in this module:
 *   - MemberAccount (abstract)   -> StandardMember, GoldMember, VIPMember
 *   - Locker                     -> one digital locker with a PIN
 *   - LockerManager              -> assigns / opens / releases lockers
 *   - Facility (abstract)        -> Pool, Sauna, WeightRoom
 *   - FacilityManager            -> controls entry / exit of facilities
 *
 *  OOP concepts used:
 *   - Encapsulation : locker PIN, failed attempts and facility member
 *                     lists are private, changed only through methods
 *   - Abstraction   : MemberAccount and Facility are abstract classes
 *                     (they have pure virtual functions)
 *   - Inheritance   : Standard/Gold/VIP inherit MemberAccount;
 *                     Pool/Sauna/WeightRoom inherit Facility
 *   - Polymorphism  : hasFacilityAccess(FacilityType) and showRules()
 *                     give a different result for each derived class
 * ============================================================
 */

#ifndef LOCKER_FACILITY_H
#define LOCKER_FACILITY_H

#include <string>
#include <vector>

// ------------------------------------------------------------
// Enums
// ------------------------------------------------------------
enum class FacilityType { POOL, SAUNA, WEIGHT_ROOM };
enum class LockerType { STANDARD, PREMIUM };

// Helper functions that convert an enum value to readable text
std::string facilityTypeName(FacilityType type);
std::string lockerTypeName(LockerType type);


// ============================================================
// MEMBER ACCOUNT (abstract base class)
// Tier mapping with Module 1 plans:
//   Monthly plan -> StandardMember
//   Annual plan  -> GoldMember
//   VIP plan     -> VIPMember
// ============================================================
class MemberAccount {
private:
    int memberId;
    std::string name;
    bool active;            // true = subscription is active

public:
    MemberAccount(int id, const std::string& memberName, bool isActive = true);
    virtual ~MemberAccount();

    int getId() const;
    std::string getName() const;
    bool isActive() const;
    void setActive(bool status);

    // Pure virtual functions: every tier must define these
    virtual std::string getTier() const = 0;
    virtual bool hasFacilityAccess(FacilityType type) const = 0;

    // Only some tiers can use premium lockers
    virtual bool canUsePremiumLocker() const;
};

// Standard (Monthly plan): Weight Room only
class StandardMember : public MemberAccount {
public:
    StandardMember(int id, const std::string& memberName, bool isActive = true);

    std::string getTier() const override;
    bool hasFacilityAccess(FacilityType type) const override;
};

// Gold (Annual plan): Weight Room + Pool
class GoldMember : public MemberAccount {
public:
    GoldMember(int id, const std::string& memberName, bool isActive = true);

    std::string getTier() const override;
    bool hasFacilityAccess(FacilityType type) const override;
};

// VIP (VIP plan): all facilities + premium lockers
class VIPMember : public MemberAccount {
public:
    VIPMember(int id, const std::string& memberName, bool isActive = true);

    std::string getTier() const override;
    bool hasFacilityAccess(FacilityType type) const override;
    bool canUsePremiumLocker() const override;
};

// Creates the right member object from a Module 1 plan name
// ("Monthly", "Annual" or "VIP"). The caller must delete it.
MemberAccount* createMemberByPlan(int id, const std::string& memberName,
                                  const std::string& planType, bool isActive = true);


// ============================================================
// LOCKER (Encapsulation: the PIN can never be read from outside)
// ============================================================
class Locker {
private:
    int lockerNumber;
    LockerType type;
    bool occupied;
    int assignedMemberId;   // -1 when the locker is free
    int pin;                // 4-digit PIN, never exposed
    int failedAttempts;
    bool blocked;

    static const int MAX_ATTEMPTS = 3;

public:
    Locker(int number, LockerType lockerType);

    int getNumber() const;
    LockerType getType() const;
    bool isOccupied() const;
    int getAssignedMemberId() const;
    bool isBlocked() const;
    int getRemainingAttempts() const;

    int assign(int memberId);       // returns the generated PIN
    void release();
    bool unlock(int enteredPin);    // true if the PIN is correct
    void resetBlock();              // staff unblocks the locker
};


// ============================================================
// LOCKER MANAGER
// ============================================================
class LockerManager {
private:
    std::vector<Locker> lockers;

    int findLockerByMember(int memberId) const;
    int findLockerByNumber(int number) const;

public:
    // Lockers 1..standardCount are Standard, the rest are Premium
    LockerManager(int standardCount, int premiumCount);

    int assignLocker(const MemberAccount& member, LockerType wanted);  // PIN or -1
    bool openLocker(const MemberAccount& member, int enteredPin);
    bool releaseLocker(const MemberAccount& member);
    bool staffResetLocker(int lockerNumber);
    void showStatus() const;
};


// ============================================================
// FACILITY (abstract base class)
// ============================================================
class Facility {
private:
    std::string name;
    int capacity;
    std::vector<int> membersInside;

protected:
    Facility(const std::string& facilityName, int maxCapacity);

public:
    virtual ~Facility();

    // Pure virtual functions: Facility is abstract
    virtual FacilityType getType() const = 0;
    virtual void showRules() const = 0;

    std::string getName() const;
    int getCapacity() const;
    int getOccupancy() const;
    bool isFull() const;
    bool isInside(int memberId) const;

    bool addMember(int memberId);
    bool removeMember(int memberId);
    void displayInfo() const;
};

class Pool : public Facility {
private:
    int lanes;
    double waterTemperature;

public:
    Pool(int maxCapacity, int laneCount, double temperature);

    FacilityType getType() const override;
    void showRules() const override;
};

class Sauna : public Facility {
private:
    int temperature;
    int maxSessionMinutes;

public:
    Sauna(int maxCapacity, int saunaTemperature, int maxMinutes);

    FacilityType getType() const override;
    void showRules() const override;
};

class WeightRoom : public Facility {
private:
    int machineCount;

public:
    WeightRoom(int maxCapacity, int machines);

    FacilityType getType() const override;
    void showRules() const override;
};


// ============================================================
// FACILITY MANAGER (owns the Facility objects)
// ============================================================
class FacilityManager {
private:
    std::vector<Facility*> facilities;

    Facility* findFacility(FacilityType type) const;
    Facility* facilityMemberIsIn(int memberId) const;   // one facility at a time

public:
    FacilityManager();
    ~FacilityManager();

    // Copying is not allowed because the manager owns the pointers
    FacilityManager(const FacilityManager&) = delete;
    FacilityManager& operator=(const FacilityManager&) = delete;

    void addFacility(Facility* facility);
    bool requestEntry(const MemberAccount& member, FacilityType type);
    bool exitFacility(const MemberAccount& member);
    void showAll() const;
};

#endif
