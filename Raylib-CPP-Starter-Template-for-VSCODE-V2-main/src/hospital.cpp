#include "Hospital.h"
#include <sstream>

Hospital::Hospital(const std::string& name, Position location, int capacity)
    : name(name), location(location), capacity(capacity), occupiedBeds(0) {}

std::string Hospital::getName() const { return name; }
Position Hospital::getLocation() const { return location; }
int Hospital::getCapacity() const { return capacity; }
int Hospital::getOccupiedBeds() const { return occupiedBeds; }

int Hospital::getAvailableBeds() const {
    return capacity - occupiedBeds;
}

bool Hospital::admitPatient() {
    if (occupiedBeds < capacity) {
        occupiedBeds++;
        return true;
    }
    return false;
}

bool Hospital::dischargePatient() {
    if (occupiedBeds > 0) {
        occupiedBeds--;
        return true;
    }
    return false;
}

std::string Hospital::toString() const {
    std::stringstream ss;
    ss << "Hospital: " << name
       << "\nLocation: " << location.toString()
       << "\nBeds: " << occupiedBeds << "/" << capacity
       << " (" << getAvailableBeds() << " available)";
    return ss.str();
}