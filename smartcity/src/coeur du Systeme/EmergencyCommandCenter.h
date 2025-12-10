#ifndef EMERGENCYCOMMANDCENTER_H
#define EMERGENCYCOMMANDCENTER_H

#include <vector>
#include <string>
#include "Emergency.h"

class EmergencyCommandCenter {
private:
    // Instance unique (Singleton)
    static EmergencyCommandCenter* instance;
    
    // Liste des urgences en cours
    std::vector<Emergency*> activeEmergencies;
    
    // Constructeur privé pour Singleton
    EmergencyCommandCenter();
    
    // Empêcher la copie
    EmergencyCommandCenter(const EmergencyCommandCenter&) = delete;
    EmergencyCommandCenter& operator=(const EmergencyCommandCenter&) = delete;

public:
    // Méthode Singleton pour obtenir l'instance
    static EmergencyCommandCenter* getInstance();
    
    // Destructeur
    ~EmergencyCommandCenter();
    
    // Méthode principale pour recevoir une alerte
    void receiveAlert(EmergencyType type, Position location, const std::string& description);
    
    // Méthodes de gestion des urgences
    void processNextEmergency();
    void displayAllEmergencies() const; 
    int getActiveEmergencyCount() const;
    
    // Méthodes utilitaires
    Emergency* getHighestPriorityEmergency() const;
    void resolveEmergency(int emergencyId);
};

#endif