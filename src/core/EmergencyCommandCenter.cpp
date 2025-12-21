#include "EmergencyCommandCenter.h"
#include <iostream>

EmergencyCommandCenter* EmergencyCommandCenter::instance = nullptr;

EmergencyCommandCenter::EmergencyCommandCenter() {
    emergencyManager = std::make_unique<EmergencyManager>();
    vehicleManager = std::make_unique<VehicleManager>();
    trafficManager = std::make_unique<TrafficManager>();
    
    std::cout << "🚨 Emergency Command Center initialized!\n";
}

EmergencyCommandCenter* EmergencyCommandCenter::getInstance() {
    if (instance == nullptr) {
        instance = new EmergencyCommandCenter();
    }
    return instance;
}

EmergencyCommandCenter::~EmergencyCommandCenter() {
    std::cout << "🔴 Emergency Command Center shut down.\n";
}

void EmergencyCommandCenter::receiveAlert(EmergencyType type, Position location, 
                                         const std::string& description) {
    std::cout << "\n📞 RECEIVING NEW ALERT:\n";
    std::cout << "Type: ";
    switch(type) {
        case EmergencyType::MEDICAL: std::cout << "MEDICAL"; break;
        case EmergencyType::FIRE: std::cout << "FIRE"; break;
        case EmergencyType::SECURITY: std::cout << "SECURITY"; break;
        case EmergencyType::TRAFFIC_ACCIDENT: std::cout << "TRAFFIC_ACCIDENT"; break;
    }
    std::cout << "\nLocation: " << location.toString() 
              << "\nDescription: " << description << std::endl;
    
    Emergency* emergency = new Emergency(type, location, description);
    emergencyManager->addEmergency(emergency);
    
    std::cout << "✅ Emergency #" << emergency->getId() 
              << " registered with priority: " << emergency->getPriorityString() << "\n";
}

void EmergencyCommandCenter::processNextEmergency() {
    Emergency* emergency = emergencyManager->getHighestPriorityEmergency();
    if (!emergency || emergency->getStatus() != "PENDING") {
        return;
    }
    
    EmergencyVehicleType requiredType;
    switch(emergency->getType()) {
        case EmergencyType::MEDICAL:
            requiredType = EmergencyVehicleType::AMBULANCE;
            break;
        case EmergencyType::FIRE:
            requiredType = EmergencyVehicleType::FIRE_TRUCK;
            break;
        case EmergencyType::SECURITY:
        case EmergencyType::TRAFFIC_ACCIDENT:
            requiredType = EmergencyVehicleType::POLICE;
            break;
    }
    
    EmergencyVehicle* vehicle = vehicleManager->findNearestAvailableVehicle(
        emergency->getLocation(), requiredType);
    
    if (vehicle) {
        vehicle->assignEmergency(emergency);
        emergency->setStatus("ASSIGNED");
        
        std::cout << "📡 Assigned " << vehicle->getId() 
                  << " to Emergency #" << emergency->getId() << std::endl;
        
        trafficManager->requestGreenWave(*vehicle);
    }
}

void EmergencyCommandCenter::addVehicle(EmergencyVehicleType type, Position position, 
                                       const std::string& id, float maxSpeed) {
    vehicleManager->addVehicle(type, position, id, maxSpeed);
}

EmergencyManager* EmergencyCommandCenter::getEmergencyManager() const {
    return emergencyManager.get();
}

VehicleManager* EmergencyCommandCenter::getVehicleManager() const {
    return vehicleManager.get();
}

TrafficManager* EmergencyCommandCenter::getTrafficManager() const {
    return trafficManager.get();
}

int EmergencyCommandCenter::getActiveEmergencyCount() const {
    return emergencyManager->getActiveEmergencyCount();
}

int EmergencyCommandCenter::getAvailableVehicleCount() const {
    return vehicleManager->getAvailableVehicleCount();
}

void EmergencyCommandCenter::update(float deltaTime) {
    trafficManager->update(deltaTime);
    vehicleManager->updateAllVehicles(deltaTime);
}

void EmergencyCommandCenter::draw() const {
    vehicleManager->drawAllVehicles();
    trafficManager->drawTrafficLights();
}

void EmergencyCommandCenter::reset() {
    emergencyManager->clearAll();
    vehicleManager->clearAll();
    trafficManager->resetAllLights();
}