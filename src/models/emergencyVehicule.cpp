#include "EmergencyVehicle.h"
#include <iostream>
#include "raylib.h"

EmergencyVehicle::EmergencyVehicle(EmergencyVehicleType type, Position startPos, 
                                 const std::string& id, float maxSpeed)
    : Vehicle(id, startPos, maxSpeed), vehicleType(type), 
      assignedEmergency(nullptr), emergencyLightsOn(false), sirenOn(false) {
    
    // Définir la couleur selon le type
    switch(type) {
        case EmergencyVehicleType::AMBULANCE:
            vehicleColor = RED;
            break;
        case EmergencyVehicleType::FIRE_TRUCK:
            vehicleColor = Color{255, 140, 0, 255}; // Orange
            break;
        case EmergencyVehicleType::POLICE:
            vehicleColor = BLUE;
            break;
    }
}

std::string EmergencyVehicle::getTypeString() const {
    switch(vehicleType) {
        case EmergencyVehicleType::AMBULANCE: return "Ambulance";
        case EmergencyVehicleType::FIRE_TRUCK: return "Fire Truck";
        case EmergencyVehicleType::POLICE: return "Police";
        default: return "Unknown";
    }
}

void EmergencyVehicle::assignEmergency(Emergency* emergency) {
    assignedEmergency = emergency;
    if (emergency) {
        setDestination(emergency->getLocation());
        setIsMoving(true);
        setSpeed(maxSpeed * 1.5f); // Plus rapide en urgence
        activateEmergencyLights(true);
        activateSiren(true);
    }
}

void EmergencyVehicle::completeMission() {
    assignedEmergency = nullptr;
    activateEmergencyLights(false);
    activateSiren(false);
    setSpeed(maxSpeed);
}

void EmergencyVehicle::activateEmergencyLights(bool activate) {
    emergencyLightsOn = activate;
}

void EmergencyVehicle::activateSiren(bool activate) {
    sirenOn = activate;
}

void EmergencyVehicle::move(float deltaTime) {
    Vehicle::move(deltaTime);
    
    // Logique spécifique aux véhicules d'urgence
    if (assignedEmergency && hasArrived()) {
        std::cout << "🚑 " << getTypeString() << " " << id << " arrived at emergency site!" << std::endl;
        completeMission();
    }
}

void EmergencyVehicle::draw() const {
    // Dessiner le véhicule
    Vector2 screenPos = {(float)position.getX(), (float)position.getY()};
    
    // Corps du véhicule
    DrawRectangle(screenPos.x - 15, screenPos.y - 10, 30, 20, vehicleColor);
    
    // Gyrophares si activés
    if (emergencyLightsOn) {
        Color lightColor = sirenOn ? YELLOW : WHITE;
        DrawCircle(screenPos.x, screenPos.y - 12, 5, lightColor);
        DrawCircle(screenPos.x + 10, screenPos.y - 12, 5, lightColor);
        DrawCircle(screenPos.x - 10, screenPos.y - 12, 5, lightColor);
    }
    
    // Texte d'ID
    DrawText(id.c_str(), screenPos.x - 20, screenPos.y + 15, 10, WHITE);
    
    // Si sélectionné, dessiner un contour
    if (isSelected) {
        DrawRectangleLines(screenPos.x - 18, screenPos.y - 12, 36, 24, GREEN);
    }
}

void EmergencyVehicle::update(float deltaTime) {
    Vehicle::update(deltaTime);
    
    // Animation des gyrophares (clignotement)
    static float blinkTimer = 0.0f;
    blinkTimer += deltaTime;
    if (blinkTimer > 0.2f) { // Clignote toutes les 0.2 secondes
        blinkTimer = 0.0f;
    }
}

float EmergencyVehicle::calculateTimeToDestination() const {
    if (!isMoving) return 0.0f;
    float distance = distanceToDestination();
    return distance / speed;
}

std::ostream& operator<<(std::ostream& os, const EmergencyVehicle& vehicle) {
    os << vehicle.getTypeString() << " " << vehicle.id
       << " at " << vehicle.getPosition().toString()
       << " assigned to: ";
    if (vehicle.assignedEmergency) {
        os << "Emergency #" << vehicle.assignedEmergency->getId();
    } else {
        os << "None";
    }
    return os;
}