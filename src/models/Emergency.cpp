#include "Emergency.h"
#include <sstream>
#include <iostream>

int Emergency::nextId = 1;

Emergency::Emergency(EmergencyType type, Position location, 
                     const std::string& description, PriorityLevel priority)
    : id(nextId++), type(type), location(location), 
      description(description), priority(priority),
      timestamp(std::chrono::system_clock::now()) {
    status = "PENDING";
    
    if (priority == PriorityLevel::MEDIUM) {
        calculateAutomaticPriority();
    }
}

int Emergency::getId() const { return id; }
EmergencyType Emergency::getType() const { return type; }
PriorityLevel Emergency::getPriority() const { return priority; }
Position Emergency::getLocation() const { return location; }
std::string Emergency::getDescription() const { return description; }
std::string Emergency::getStatus() const { return status; }

std::string Emergency::getTypeString() const {
    switch(type) {
        case EmergencyType::MEDICAL: return "MEDICAL";
        case EmergencyType::FIRE: return "FIRE";
        case EmergencyType::SECURITY: return "SECURITY";
        case EmergencyType::TRAFFIC_ACCIDENT: return "TRAFFIC_ACCIDENT";
        default: return "UNKNOWN";
    }
}

std::string Emergency::getPriorityString() const {
    switch(priority) {
        case PriorityLevel::CRITICAL: return "CRITICAL";
        case PriorityLevel::HIGH: return "HIGH";
        case PriorityLevel::MEDIUM: return "MEDIUM";
        case PriorityLevel::LOW: return "LOW";
        default: return "UNKNOWN";
    }
}

void Emergency::setPriority(PriorityLevel priority) {
    this->priority = priority;
}

void Emergency::setStatus(const std::string& status) {
    this->status = status;
}

void Emergency::calculateAutomaticPriority() {
    switch(type) {
        case EmergencyType::MEDICAL:
            priority = PriorityLevel::CRITICAL;
            break;
        case EmergencyType::FIRE:
            priority = PriorityLevel::HIGH;
            break;
        case EmergencyType::SECURITY:
            priority = PriorityLevel::HIGH;
            break;
        case EmergencyType::TRAFFIC_ACCIDENT:
            priority = PriorityLevel::MEDIUM;
            break;
    }
}

std::string Emergency::toString() const {
    std::stringstream ss;
    ss << "Emergency #" << id 
       << " [" << getTypeString() << " | " << getPriorityString() << "]"
       << "\nLocation: " << location.toString()
       << "\nDescription: " << description
       << "\nStatus: " << status;
    return ss.str();
}

bool Emergency::operator<(const Emergency& other) const {
    return static_cast<int>(priority) < static_cast<int>(other.priority);
}