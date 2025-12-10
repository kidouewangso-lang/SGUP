#include "Emergency.h"
#include <sstream>
#include <chrono>

// Initialisation de la variable statique
int Emergency::nextId = 1;

// Constructeur
Emergency::Emergency(EmergencyType type, Position location, 
                     const std::string& description, PriorityLevel priority)
    : id(nextId++), type(type), location(location), 
      description(description), priority(priority) {
    
    // Timestamp actuel
    auto now = std::chrono::system_clock::now();
    timestamp = std::chrono::duration_cast<std::chrono::seconds>(
        now.time_since_epoch()).count();
    
    status = "PENDING";
    
    // Si priorité non spécifiée, calcul automatique
    if (priority == PriorityLevel::MEDIUM) {
        calculateAutomaticPriority();
    }
}

// Getters
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

// Setters
void Emergency::setPriority(PriorityLevel newPriority) {
    priority = newPriority;
}

void Emergency::setStatus(const std::string& newStatus) {
    status = newStatus;
}

// Méthodes utilitaires
void Emergency::updateStatus(const std::string& newStatus) {
    status = newStatus;
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

// Calcul automatique de priorité basé sur le type
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
        default:
            priority = PriorityLevel::MEDIUM;
    }
}