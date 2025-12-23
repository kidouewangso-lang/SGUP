#ifndef EMERGENCY_H
#define EMERGENCY_H

#include "Position.h"
#include <string>
#include <chrono>

enum class EmergencyType {
    MEDICAL,
    FIRE,
    SECURITY,
    TRAFFIC_ACCIDENT
};

enum class PriorityLevel {
    CRITICAL = 1,
    HIGH = 2,
    MEDIUM = 3,
    LOW = 4
};

class Emergency {
private:
    static int nextId;
    int id;
    EmergencyType type;
    PriorityLevel priority;
    Position location;
    std::string description;
    std::string status;
    std::chrono::system_clock::time_point timestamp;

public:
    Emergency(EmergencyType type, Position location, 
              const std::string& description, 
              PriorityLevel priority = PriorityLevel::MEDIUM);
    
    int getId() const;
    EmergencyType getType() const;
    PriorityLevel getPriority() const;
    Position getLocation() const;
    std::string getDescription() const;
    std::string getStatus() const;
    std::string getTypeString() const;
    std::string getPriorityString() const;
    
    void setPriority(PriorityLevel priority);
    void setStatus(const std::string& status);
    
    void calculateAutomaticPriority();
    std::string toString() const;
    
    bool operator<(const Emergency& other) const;
};

#endif