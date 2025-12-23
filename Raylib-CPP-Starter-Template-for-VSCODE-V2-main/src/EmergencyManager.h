#ifndef EMERGENCYMANAGER_H
#define EMERGENCYMANAGER_H

#include "Emergency.h"
#include <vector>
#include <queue>

class EmergencyManager {
private:
    std::vector<Emergency*> emergencies;
    
public:
    EmergencyManager();
    ~EmergencyManager();
    
    void addEmergency(Emergency* emergency);
    void removeEmergency(int id);
    Emergency* getEmergencyById(int id) const;
    Emergency* getHighestPriorityEmergency() const;
    
    std::vector<Emergency*> getAllEmergencies() const;
    std::vector<Emergency*> getActiveEmergencies() const;
    std::vector<Emergency*> getEmergenciesByType(EmergencyType type) const;
    
    int getEmergencyCount() const;
    int getActiveEmergencyCount() const;
    
    void updateEmergencyStatus(int id, const std::string& status);
    void prioritizeEmergencies();
    
    void clearAll();
};

#endif