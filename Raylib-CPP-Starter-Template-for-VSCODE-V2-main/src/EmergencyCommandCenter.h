#ifndef EMERGENCYCOMMANDCENTER_H
#define EMERGENCYCOMMANDCENTER_H

#include "EmergencyManager.h"
#include "VehiculeManager.h"
#include "TrafficManager.h"
#include <memory>

class EmergencyCommandCenter {
private:
    static EmergencyCommandCenter* instance;
    
    std::unique_ptr<EmergencyManager> emergencyManager;
    std::unique_ptr<VehicleManager> vehicleManager;
    std::unique_ptr<TrafficManager> trafficManager;
    
    EmergencyCommandCenter();
    EmergencyCommandCenter(const EmergencyCommandCenter&) = delete;
    EmergencyCommandCenter& operator=(const EmergencyCommandCenter&) = delete;
    
public:
    static EmergencyCommandCenter* getInstance();
    ~EmergencyCommandCenter();
    
    void receiveAlert(EmergencyType type, Position location, 
                     const std::string& description);
    void processNextEmergency();
    
    void addVehicle(EmergencyVehicleType type, Position position, 
                   const std::string& id, float maxSpeed = 120.0f);
    
    EmergencyManager* getEmergencyManager() const;
    VehicleManager* getVehicleManager() const;
    TrafficManager* getTrafficManager() const;
    
    int getActiveEmergencyCount() const;
    int getAvailableVehicleCount() const;
    
    void update(float deltaTime);
    void draw() const;
    
    void reset();
};

#endif