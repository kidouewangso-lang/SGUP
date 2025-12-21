#include "EmergencyManager.h"
#include <algorithm>
#include <iostream>

EmergencyManager::EmergencyManager() {}

EmergencyManager::~EmergencyManager() {
    clearAll();
}

void EmergencyManager::addEmergency(Emergency* emergency) {
    emergencies.push_back(emergency);
}

void EmergencyManager::removeEmergency(int id) {
    auto it = std::remove_if(emergencies.begin(), emergencies.end(),
        [id](Emergency* e) { return e->getId() == id; });
    
    if (it != emergencies.end()) {
        delete *it;
        emergencies.erase(it, emergencies.end());
    }
}

Emergency* EmergencyManager::getEmergencyById(int id) const {
    for (auto emergency : emergencies) {
        if (emergency->getId() == id) {
            return emergency;
        }
    }
    return nullptr;
}

Emergency* EmergencyManager::getHighestPriorityEmergency() const {
    if (emergencies.empty()) return nullptr;
    
    Emergency* highest = emergencies[0];
    for (auto emergency : emergencies) {
        if (emergency->getPriority() < highest->getPriority() && 
            emergency->getStatus() == "PENDING") {
            highest = emergency;
        }
    }
    return highest;
}

std::vector<Emergency*> EmergencyManager::getAllEmergencies() const {
    return emergencies;
}

std::vector<Emergency*> EmergencyManager::getActiveEmergencies() const {
    std::vector<Emergency*> active;
    for (auto emergency : emergencies) {
        if (emergency->getStatus() != "RESOLVED") {
            active.push_back(emergency);
        }
    }
    return active;
}

std::vector<Emergency*> EmergencyManager::getEmergenciesByType(EmergencyType type) const {
    std::vector<Emergency*> result;
    for (auto emergency : emergencies) {
        if (emergency->getType() == type) {
            result.push_back(emergency);
        }
    }
    return result;
}

int EmergencyManager::getEmergencyCount() const {
    return emergencies.size();
}

int EmergencyManager::getActiveEmergencyCount() const {
    return getActiveEmergencies().size();
}

void EmergencyManager::updateEmergencyStatus(int id, const std::string& status) {
    Emergency* emergency = getEmergencyById(id);
    if (emergency) {
        emergency->setStatus(status);
    }
}

void EmergencyManager::prioritizeEmergencies() {
    std::sort(emergencies.begin(), emergencies.end(),
        [](Emergency* a, Emergency* b) {
            return a->getPriority() < b->getPriority();
        });
}

void EmergencyManager::clearAll() {
    for (auto emergency : emergencies) {
        delete emergency;
    }
    emergencies.clear();
}