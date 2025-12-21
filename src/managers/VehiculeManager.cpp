#include "VehicleManager.h"
#include <algorithm>
#include <limits>

VehicleManager::VehicleManager() {}

void VehicleManager::addVehicle(EmergencyVehicleType type, Position position, 
                               const std::string& id, float maxSpeed) {
    vehicles.push_back(std::make_unique<EmergencyVehicle>(type, position, id, maxSpeed));
}

void VehicleManager::removeVehicle(const std::string& id) {
    auto it = std::remove_if(vehicles.begin(), vehicles.end(),
        [id](const std::unique_ptr<EmergencyVehicle>& v) { 
            return v->getId() == id; 
        });
    
    if (it != vehicles.end()) {
        vehicles.erase(it, vehicles.end());
    }
}

EmergencyVehicle* VehicleManager::getVehicleById(const std::string& id) const {
    for (const auto& vehicle : vehicles) {
        if (vehicle->getId() == id) {
            return vehicle.get();
        }
    }
    return nullptr;
}

std::vector<EmergencyVehicle*> VehicleManager::getAllVehicles() const {
    std::vector<EmergencyVehicle*> result;
    for (const auto& vehicle : vehicles) {
        result.push_back(vehicle.get());
    }
    return result;
}

std::vector<EmergencyVehicle*> VehicleManager::getAvailableVehicles() const {
    std::vector<EmergencyVehicle*> result;
    for (const auto& vehicle : vehicles) {
        if (!vehicle->getAssignedEmergency()) {
            result.push_back(vehicle.get());
        }
    }
    return result;
}

std::vector<EmergencyVehicle*> VehicleManager::getVehiclesByType(EmergencyVehicleType type) const {
    std::vector<EmergencyVehicle*> result;
    for (const auto& vehicle : vehicles) {
        if (vehicle->getVehicleType() == type) {
            result.push_back(vehicle.get());
        }
    }
    return result;
}

EmergencyVehicle* VehicleManager::findNearestAvailableVehicle(Position position, 
                                                             EmergencyVehicleType preferredType) {
    EmergencyVehicle* nearest = nullptr;
    float minDistance = std::numeric_limits<float>::max();
    
    for (const auto& vehicle : vehicles) {
        if (!vehicle->getAssignedEmergency()) {
            float distance = vehicle->getPosition().distanceTo(position);
            if (distance < minDistance) {
                // Priorité au type demandé
                if (vehicle->getVehicleType() == preferredType) {
                    minDistance = distance;
                    nearest = vehicle.get();
                } else if (nearest == nullptr || nearest->getVehicleType() != preferredType) {
                    minDistance = distance;
                    nearest = vehicle.get();
                }
            }
        }
    }
    
    return nearest;
}

int VehicleManager::getVehicleCount() const {
    return vehicles.size();
}

int VehicleManager::getAvailableVehicleCount() const {
    return getAvailableVehicles().size();
}

void VehicleManager::updateAllVehicles(float deltaTime) {
    for (auto& vehicle : vehicles) {
        vehicle->update(deltaTime);
    }
}

void VehicleManager::drawAllVehicles() const {
    for (const auto& vehicle : vehicles) {
        vehicle->draw();
    }
}

void VehicleManager::clearAll() {
    vehicles.clear();
}