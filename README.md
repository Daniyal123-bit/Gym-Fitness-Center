*
 * ============================================================
 *  Gym & Fitness Center Subscription Engine
 *  Module 3 - Personal Trainer Scheduling
 *  Developed by: Umer Yousaf (F2025332004)
 * ============================================================
 *  File: trainer_scheduling.cpp
 *  Definitions of all member functions declared in
 *  trainer_scheduling.h (no main() in this file).
 * ============================================================
 */

#include "trainer_scheduling.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

/
// ============================================================
// HELPER FUNCTIONS
// ============================================================
string dayName(int day) {
    const string names[DAYS_IN_WEEK] = {"Monday", "Tuesday", "Wednesday", "Thursday",
                                        "Friday", "Saturday", "Sunday"};
    if (day < 0 || day >= DAYS_IN_WEEK) return "Invalid day";
    return names[day];
}

string hourText(int hour) {
    int shown = hour % 12;
    if (shown == 0) shown = 12;

    stringstream output;
    output << setfill('0') << setw(2) << shown << ":00 " << (hour < 12 ? "AM" : "PM");
    return output.str();
}

string statusName(BookingStatus status) {
    switch (status) {
        case BookingStatus::BOOKED:    return "Booked";
        case BookingStatus::COMPLETED: return "Completed";
        case BookingStatus::CANCELLED: return "Cancelled";
    }
    return "Unknown";
}

bool isValidSlot(int day, int hour) {
    return day >= 0 && day < DAYS_IN_WEEK &&
           hour >= OPENING_HOUR && hour < CLOSING_HOUR;
}


// ============================================================
// Class 1: Trainer
// ============================================================
Trainer::Trainer(int id, string name, string spec, int exp, double rate) {
    trainerID = id;
    trainerName = name;
    specialization = spec;
    experience = exp;
    hourlyRate = rate;

    for (int d = 0; d < DAYS_IN_WEEK; d++) {
        for (int h = 0; h < HOURS_IN_DAY; h++) {
            available[d][h] = false;
            booked[d][h] = false;
        }
    }
}

Trainer::~Trainer() {}

int Trainer::getTrainerID() const {
    return trainerID;
}

string Trainer::getTrainerName() const {
    return trainerName;
}

string Trainer::getSpecialization() const {
    return specialization;
}

int Trainer::getExperience() const {
    return experience;
}

double Trainer::getHourlyRate() const {
    return hourlyRate;
}

void Trainer::setHourlyRate(double rate) {
    if (rate > 0) hourlyRate = rate;
}

// Marks every hour from startHour up to (not including) endHour as available
void Trainer::setAvailability(int day, int startHour, int endHour) {
    for (int h = startHour; h < endHour; h++) {
        if (isValidSlot(day, h)) available[day][h] = true;
    }
}

void Trainer::removeAvailability(int day, int startHour, int endHour) {
    for (int h = startHour; h < endHour; h++) {
        if (isValidSlot(day, h) && !booked[day][h]) available[day][h] = false;
    }
}

bool Trainer::isAvailable(int day, int hour) const {
    if (!isValidSlot(day, hour)) return false;
    return available[day][hour] && !booked[day][hour];
}

bool Trainer::isBooked(int day, int hour) const {
    if (!isValidSlot(day, hour)) return false;
    return booked[day][hour];
}

void Trainer::markBooked(int day, int hour) {
    if (isValidSlot(day, hour)) booked[day][hour] = true;
}

void Trainer::freeSlot(int day, int hour) {
    if (isValidSlot(day, hour)) booked[day][hour] = false;
}

void Trainer::displayData() const {
    cout << "Trainer ID:      " << trainerID << endl;
    cout << "Trainer Name:    " << trainerName << endl;
    cout << "Level:           " << getLevel() << endl;
    cout << "Specialization:  " << specialization << endl;
    cout << "Experience:      " << experience << " years" << endl;
    cout << "Hourly Rate:     Rs. " << hourlyRate << endl;
    cout << "Commission:      " << getCommissionRate() * 100 << "%" << endl;
}

void Trainer::displayCalendar() const {
    cout << "\n---------- Calendar: " << trainerName << " ----------" << endl;
    bool anySlot = false;

    for (int d = 0; d < DAYS_IN_WEEK; d++) {
        bool dayPrinted = false;

        for (int h = OPENING_HOUR; h < CLOSING_HOUR; h++) {
            if (!available[d][h] && !booked[d][h]) continue;

            if (!dayPrinted) {
                cout << dayName(d) << ":" << endl;
                dayPrinted = true;
                anySlot = true;
            }
            cout << "   " << hourText(h) << "  " << (booked[d][h] ? "[BOOKED]" : "[FREE]") << endl;
        }
    }

    if (!anySlot) cout << "No availability set." << endl;
}

double Trainer::calculateCommission(double sessionFee) const {
    return sessionFee * getCommissionRate();
}


// ============================================================
// Class 2: JuniorTrainer
// ============================================================
JuniorTrainer::JuniorTrainer(int id, string name, string spec, int exp, double rate)
    : Trainer(id, name, spec, exp, rate) {}

string JuniorTrainer::getLevel() const {
    return "Junior";
}

double JuniorTrainer::getCommissionRate() const {
    return 0.30;
}


// ============================================================
// Class 3: SeniorTrainer
// ============================================================
SeniorTrainer::SeniorTrainer(int id, string name, string spec, int exp, double rate)
    : Trainer(id, name, spec, exp, rate) {}

string SeniorTrainer::getLevel() const {
    return "Senior";
}

double SeniorTrainer::getCommissionRate() const {
    return 0.40;
}


// ============================================================
// Class 4: HeadTrainer
// ============================================================
HeadTrainer::HeadTrainer(int id, string name, string spec, int exp, double rate,
                         double bonus)
    : Trainer(id, name, spec, exp, rate), sessionBonus(bonus) {}

string HeadTrainer::getLevel() const {
    return "Head";
}

double HeadTrainer::getCommissionRate() const {
    return 0.50;
}

// Head trainer gets the normal commission plus a fixed bonus per session
double HeadTrainer::calculateCommission(double sessionFee) const {
    return Trainer::calculateCommission(sessionFee) + sessionBonus;
}


// ============================================================
// Class 5: Booking
// ============================================================
Booking::Booking(int id, int memID, string memName, int trID, int bookingDay,
                 int bookingHour, double sessionFee) {
    bookingID = id;
    memberID = memID;
    memberName = memName;
    trainerID = trID;
    day = bookingDay;
    hour = bookingHour;
    fee = sessionFee;
    status = BookingStatus::BOOKED;
}

int Booking::getBookingID() const {
    return bookingID;
}

int Booking::getMemberID() const {
    return memberID;
}

string Booking::getMemberName() const {
    return memberName;
}

int Booking::getTrainerID() const {
    return trainerID;
}

int Booking::getDay() const {
    return day;
}

int Booking::getHour() const {
    return hour;
}

double Booking::getFee() const {
    return fee;
}

BookingStatus Booking::getStatus() const {
    return status;
}

void Booking::complete() {
    status = BookingStatus::COMPLETED;
}

void Booking::cancel() {
    status = BookingStatus::CANCELLED;
}

void Booking::display() const {
    cout << "Booking #" << bookingID << " | Member " << memberID << " (" << memberName
         << ") | Trainer " << trainerID << " | " << dayName(day) << " "
         << hourText(hour) << " | Rs. " << fee << " | " << statusName(status) << endl;
}


// ============================================================
// Class 6: TrainerScheduler
// ============================================================
TrainerScheduler::TrainerScheduler() {
    nextBookingID = 1;
}

TrainerScheduler::~TrainerScheduler() {
    for (size_t i = 0; i < trainers.size(); i++)
        delete trainers[i];
}

Trainer* TrainerScheduler::findTrainer(int trainerID) const {
    for (size_t i = 0; i < trainers.size(); i++)
        if (trainers[i]->getTrainerID() == trainerID) return trainers[i];
    return nullptr;
}

int TrainerScheduler::findBooking(int bookingID) const {
    for (size_t i = 0; i < bookings.size(); i++)
        if (bookings[i].getBookingID() == bookingID) return static_cast<int>(i);
    return -1;
}

// A member cannot have two sessions at the same time
bool TrainerScheduler::memberHasSession(int memberID, int day, int hour) const {
    for (size_t i = 0; i < bookings.size(); i++) {
        const Booking& b = bookings[i];
        if (b.getMemberID() == memberID && b.getDay() == day && b.getHour() == hour &&
            b.getStatus() == BookingStatus::BOOKED)
            return true;
    }
    return false;
}

void TrainerScheduler::addTrainer(Trainer* trainer) {
    if (trainer == nullptr) return;

    if (findTrainer(trainer->getTrainerID()) != nullptr) {
        cout << "Trainer ID " << trainer->getTrainerID() << " already exists." << endl;
        delete trainer;
        return;
    }

    trainers.push_back(trainer);
    cout << trainer->getLevel() << " trainer " << trainer->getTrainerName()
         << " added." << endl;
}

bool TrainerScheduler::setTrainerAvailability(int trainerID, int day,
                                              int startHour, int endHour) {
    Trainer* t = findTrainer(trainerID);
    if (t == nullptr) {
        cout << "Trainer ID " << trainerID << " not found." << endl;
        return false;
    }

    if (day < 0 || day >= DAYS_IN_WEEK || startHour < OPENING_HOUR ||
        endHour > CLOSING_HOUR || startHour >= endHour) {
        cout << "Invalid availability for " << t->getTrainerName()
             << ". Gym hours are " << hourText(OPENING_HOUR) << " to "
             << hourText(CLOSING_HOUR) << "." << endl;
        return false;
    }

    t->setAvailability(day, startHour, endHour);
    cout << t->getTrainerName() << " is available on " << dayName(day) << " from "
         << hourText(startHour) << " to " << hourText(endHour) << "." << endl;
    return true;
}

int TrainerScheduler::bookSession(int memberID, string memberName, int trainerID,
                                  int day, int hour) {
    Trainer* t = findTrainer(trainerID);
    if (t == nullptr) {
        cout << "[DENIED] Trainer ID " << trainerID << " not found." << endl;
        return -1;
    }

    if (!isValidSlot(day, hour)) {
        cout << "[DENIED] " << dayName(day) << " " << hourText(hour)
             << " is outside gym hours." << endl;
        return -1;
    }

    if (t->isBooked(day, hour)) {
        cout << "[DENIED] " << t->getTrainerName() << " is already booked on "
             << dayName(day) << " at " << hourText(hour) << "." << endl;
        return -1;
    }

    if (!t->isAvailable(day, hour)) {
        cout << "[DENIED] " << t->getTrainerName() << " is not available on "
             << dayName(day) << " at " << hourText(hour) << "." << endl;
        return -1;
    }

    if (memberHasSession(memberID, day, hour)) {
        cout << "[DENIED] " << memberName << " already has a session on "
             << dayName(day) << " at " << hourText(hour) << "." << endl;
        return -1;
    }

    int id = nextBookingID++;
    bookings.push_back(Booking(id, memberID, memberName, trainerID, day, hour,
                               t->getHourlyRate()));
    t->markBooked(day, hour);

    cout << "[BOOKED] #" << id << ": " << memberName << " with " << t->getTrainerName()
         << " on " << dayName(day) << " at " << hourText(hour)
         << " (Rs. " << t->getHourlyRate() << ")" << endl;
    return id;
}

bool TrainerScheduler::cancelBooking(int bookingID) {
    int idx = findBooking(bookingID);
    if (idx == -1) {
        cout << "Booking #" << bookingID << " not found." << endl;
        return false;
    }

    Booking& b = bookings[idx];
    if (b.getStatus() != BookingStatus::BOOKED) {
        cout << "Booking #" << bookingID << " cannot be cancelled. It is already "
             << statusName(b.getStatus()) << "." << endl;
        return false;
    }

    b.cancel();
    Trainer* t = findTrainer(b.getTrainerID());
    if (t != nullptr) t->freeSlot(b.getDay(), b.getHour());   // slot opens again

    cout << "Booking #" << bookingID << " cancelled. The slot is free again." << endl;
    return true;
}

bool TrainerScheduler::completeSession(int bookingID) {
    int idx = findBooking(bookingID);
    if (idx == -1) {
        cout << "Booking #" << bookingID << " not found." << endl;
        return false;
    }

    Booking& b = bookings[idx];
    if (b.getStatus() != BookingStatus::BOOKED) {
        cout << "Booking #" << bookingID << " cannot be completed. It is "
             << statusName(b.getStatus()) << "." << endl;
        return false;
    }

    b.complete();
    cout << "Session #" << bookingID << " completed." << endl;
    return true;
}

void TrainerScheduler::showTrainers() const {
    cout << "\n========== TRAINERS ==========" << endl;
    for (size_t i = 0; i < trainers.size(); i++) {
        trainers[i]->displayData();
        cout << "------------------------------" << endl;
    }
}

void TrainerScheduler::showTrainerCalendar(int trainerID) const {
    Trainer* t = findTrainer(trainerID);
    if (t == nullptr) {
        cout << "Trainer ID " << trainerID << " not found." << endl;
        return;
    }
    t->displayCalendar();
}

void TrainerScheduler::showFreeSlots(int trainerID, int day) const {
    Trainer* t = findTrainer(trainerID);
    if (t == nullptr) {
        cout << "Trainer ID " << trainerID << " not found." << endl;
        return;
    }

    cout << "\nFree slots for " << t->getTrainerName() << " on " << dayName(day) << ": ";
    bool found = false;
    for (int h = OPENING_HOUR; h < CLOSING_HOUR; h++) {
        if (t->isAvailable(day, h)) {
            cout << hourText(h) << "  ";
            found = true;
        }
    }
    if (!found) cout << "none";
    cout << endl;
}

void TrainerScheduler::showBookings() const {
    cout << "\n========== ALL BOOKINGS ==========" << endl;
    if (bookings.empty()) {
        cout << "No bookings yet." << endl;
        return;
    }
    for (size_t i = 0; i < bookings.size(); i++)
        bookings[i].display();
}

// Commission is only paid for completed sessions
double TrainerScheduler::calculatePayout(int trainerID) const {
    Trainer* t = findTrainer(trainerID);
    if (t == nullptr) return 0;

    double total = 0;
    for (size_t i = 0; i < bookings.size(); i++) {
        const Booking& b = bookings[i];
        if (b.getTrainerID() == trainerID && b.getStatus() == BookingStatus::COMPLETED)
            total += t->calculateCommission(b.getFee());    // polymorphic call
    }
    return total;
}

void TrainerScheduler::showCommissionPayouts() const {
    cout << "\n========== COMMISSION PAYOUTS ==========" << endl;
    for (size_t i = 0; i < trainers.size(); i++) {
        const Trainer* t = trainers[i];

        int completed = 0;
        for (size_t j = 0; j < bookings.size(); j++) {
            if (bookings[j].getTrainerID() == t->getTrainerID() &&
                bookings[j].getStatus() == BookingStatus::COMPLETED)
                completed++;
        }

        cout << t->getTrainerName() << " (" << t->getLevel() << "): "
             << completed << " completed session(s), payout Rs. "
             << calculatePayout(t->getTrainerID()) << endl;
    }
}
