#include <iostream>
#include "src/models/Emergency.h"
#include "src/core/EmergencyCommandCenter.h"

int main() {
    std::cout << "🚨 SMART CITY EMERGENCY SYSTEM - STARTING 🚨\n";
    
    // Test de la classe Emergency
    Position pos(45.5017, -73.5673);
    Emergency medicalEmergency(EmergencyType::MEDICAL, pos, "Cardiac arrest");
    
    std::cout << medicalEmergency.toString() << std::endl;
    
    // Test du Command Center
    EmergencyCommandCenter* center = EmergencyCommandCenter::getInstance();
    
    std::cout << "\n✅ System ready!\n";
    return 0;
}