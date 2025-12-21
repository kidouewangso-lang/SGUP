#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "Position.h"
#include <string>

class Hospital {
private:
    std::string name;
    Position location;
    int capacity;
    int occupiedBeds;
    
public:
    Hospital(const std::string& name, Position location, int capacity);
    
    std::string getName() const;
    Position getLocation() const;
    int getCapacity() const;
    int getOccupiedBeds() const;
    int getAvailableBeds() const;
    
    bool admitPatient();
    bool dischargePatient();
    
    std::string toString() const;
};

#endif