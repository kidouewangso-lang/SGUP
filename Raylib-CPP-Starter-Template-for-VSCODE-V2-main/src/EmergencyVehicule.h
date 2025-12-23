#ifndef EMERGENCYVEHICLE_H
#define EMERGENCYVEHICLE_H

#include "Vehicule.h"
#include "Emergency.h"
#include <string>
#include "raylib.h"

enum class EmergencyVehicleType {
    AMBULANCE,
    FIRE_TRUCK,
    POLICE
};

class EmergencyVehicle : public Vehicle {
private:
    EmergencyVehicleType vehicleType;
    Emergency* assignedEmergency;
    bool emergencyLightsOn;
    bool sirenOn;
    Color vehicleColor;
    
public:
    EmergencyVehicle(EmergencyVehicleType type, Position startPos, 
                    const std::string& id, float maxSpeed = 120.0f);
    
    // Getters spécifiques
    EmergencyVehicleType getVehicleType() const { return vehicleType; }
    std::string getTypeString() const;
    Emergency* getAssignedEmergency() const { return assignedEmergency; }
    bool areLightsOn() const { return emergencyLightsOn; }
    bool isSirenOn() const { return sirenOn; }
    Color getColor() const { return vehicleColor; }
    
    // Méthodes d'urgence
    void assignEmergency(Emergency* emergency);
    void completeMission();
    
    // Activation des équipements
    void activateEmergencyLights(bool activate);
    void activateSiren(bool activate);
    
    // Override de Vehicle
    void move(float deltaTime) override;
    void draw() const override;
    void update(float deltaTime) override;
    
    // Calcul
    float calculateTimeToDestination() const;
    
    // Opérateur
    friend std::ostream& operator<<(std::ostream& os, const EmergencyVehicle& vehicle);
};

#endif