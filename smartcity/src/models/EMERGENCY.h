#ifndef EMERGENCY_H
#define EMERGENCY_H

#include <string>
#include "Position.h"

// Types d'urgence
enum class EmergencyType {
    MEDICAL,
    FIRE, 
    SECURITY,
    TRAFFIC_ACCIDENT
};

// Niveaux de priorité
enum class PriorityLevel {
    CRITICAL,   // Vie en danger immédiat
    HIGH,       // Risque élevé
    MEDIUM,     // Urgence standard
    LOW         // Non urgent
};

class Emergency {
private:
    static int nextId;           // Génération auto des IDs
    
    int id;
    EmergencyType type;
    PriorityLevel priority;
    Position location;
    std::string description;
    std::string status;          // "PENDING", "ASSIGNED", "RESOLVED"
    double timestamp;           // Quand l'urgence a été créée

public:
    // Constructeur
    Emergency(EmergencyType type, Position location, 
              const std::string& description, PriorityLevel priority = PriorityLevel::MEDIUM);
    
    // Getters
    int getId() const;
    EmergencyType getType() const;
    PriorityLevel getPriority() const;
    Position getLocation() const;
    std::string getDescription() const;
    std::string getStatus() const;
    std::string getTypeString() const;
    std::string getPriorityString() const;
    
    // Setters
    void setPriority(PriorityLevel newPriority);
    void setStatus(const std::string& newStatus);
    
    // Méthodes utilitaires
    void updateStatus(const std::string& newStatus);
    std::string toString() const;
    
    // Méthode pour calculer la priorité automatiquement
    void calculateAutomaticPriority();
};

#endif