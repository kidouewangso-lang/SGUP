#ifndef TRAFFICMANAGER_H
#define TRAFFICMANAGER_H

#include "Position.h"
#include "EmergencyVehicule.h"
#include <vector>

class TrafficManager {
private:
    struct TrafficLight {
        Position position;
        bool isGreen;
        float timer;
    };
    
    std::vector<TrafficLight> trafficLights;
    
public:
    TrafficManager();
    
    void generateTrafficLights(const std::vector<Position>& positions);
    void update(float deltaTime);
    
    bool requestGreenWave(const EmergencyVehicle& vehicle, float duration = 10.0f);
    void resetAllLights();
    
    std::vector<Position> getGreenLights() const;
    std::vector<Position> getAllLights() const;
    
    void drawTrafficLights() const;
};

#endif