#ifndef VEHICLEMANAGER_H
#define VEHICLEMANAGER_H

#include "EmergencyVehicle.h"
#include <vector>
#include <memory>

class VehicleManager {
private:
    std::vector<std::unique_ptr<EmergencyVehicle>> vehicles;
    
public:
    VehicleManager();
    
    void addVehicle(EmergencyVehicleType type, Position position, 
                   const std::string& id, float maxSpeed = 120.0f);
    void removeVehicle(const std::string& id);
    EmergencyVehicle* getVehicleById(const std::string& id) const;
    
    std::vector<EmergencyVehicle*> getAllVehicles() const;
    std::vector<EmergencyVehicle*> getAvailableVehicles() const;
    std::vector<EmergencyVehicle*> getVehiclesByType(EmergencyVehicleType type) const;
    
    EmergencyVehicle* findNearestAvailableVehicle(Position position, 
                                                 EmergencyVehicleType preferredType);
    
    int getVehicleCount() const;
    int getAvailableVehicleCount() const;
    
    void updateAllVehicles(float deltaTime);
    void drawAllVehicles() const;
    
    void clearAll();
};

#endif