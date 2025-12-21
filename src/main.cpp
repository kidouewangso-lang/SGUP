#include "raylib.h"
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

#include "src/core/EmergencyCommandCenter.h"
#include "src/models/EmergencyVehicle.h"
#include "src/models/Emergency.h"
#include "src/graphics/Renderer.h"

// Configuration
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const char* WINDOW_TITLE = "Smart City - Emergency & Priority Management";

int main() {
    // Initialisation Raylib
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
    
    // Initialisation du renderer
    Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT);
    renderer.init();
    
    // Initialisation du système d'urgence
    EmergencyCommandCenter* commandCenter = EmergencyCommandCenter::getInstance();
    
    // Création des véhicules d'urgence
    std::vector<EmergencyVehicle> vehicles = {
        EmergencyVehicle(EmergencyVehicleType::AMBULANCE, Position(100, 100), "AMB-001"),
        EmergencyVehicle(EmergencyVehicleType::AMBULANCE, Position(300, 200), "AMB-002"),
        EmergencyVehicle(EmergencyVehicleType::FIRE_TRUCK, Position(500, 150), "FT-001"),
        EmergencyVehicle(EmergencyVehicleType::POLICE, Position(700, 300), "POL-001"),
        EmergencyVehicle(EmergencyVehicleType::POLICE, Position(900, 400), "POL-002")
    };
    
    // Variables simulation
    float simulationTime = 0.0f;
    bool simulationPaused = false;
    int selectedVehicle = -1;
    
    std::cout << "=============================================\n";
    std::cout << "🚨 SMART CITY EMERGENCY MANAGEMENT SYSTEM 🚨\n";
    std::cout << "=============================================\n";
    std::cout << "Controls:\n";
    std::cout << "  SPACE       - Pause/Resume simulation\n";
    std::cout << "  1-4         - Create emergency types\n";
    std::cout << "  R           - Reset simulation\n";
    std::cout << "  Mouse Wheel - Zoom in/out\n";
    std::cout << "  Right Click - Pan camera\n";
    std::cout << "  Left Click  - Select vehicle\n";
    std::cout << "=============================================\n";
    
    // Boucle principale
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        
        // === GESTION DES ENTREES ===
        
        // Pause/Reprise
        if (IsKeyPressed(KEY_SPACE)) {
            simulationPaused = !simulationPaused;
            std::cout << (simulationPaused ? "⏸️ Simulation Paused" : "▶️ Simulation Resumed") << std::endl;
        }
        
        // Création d'urgences
        if (IsKeyPressed(KEY_ONE)) {
            Position randomPos(GetRandomValue(100, 1100), GetRandomValue(100, 700));
            commandCenter->receiveAlert(EmergencyType::MEDICAL, randomPos, "Medical emergency");
        }
        if (IsKeyPressed(KEY_TWO)) {
            Position randomPos(GetRandomValue(100, 1100), GetRandomValue(100, 700));
            commandCenter->receiveAlert(EmergencyType::FIRE, randomPos, "Fire reported");
        }
        if (IsKeyPressed(KEY_THREE)) {
            Position randomPos(GetRandomValue(100, 1100), GetRandomValue(100, 700));
            commandCenter->receiveAlert(EmergencyType::SECURITY, randomPos, "Security alert");
        }
        if (IsKeyPressed(KEY_FOUR)) {
            Position randomPos(GetRandomValue(100, 1100), GetRandomValue(100, 700));
            commandCenter->receiveAlert(EmergencyType::TRAFFIC_ACCIDENT, randomPos, "Traffic accident");
        }
        
        // Reset
        if (IsKeyPressed(KEY_R)) {
            commandCenter = EmergencyCommandCenter::getInstance();
            for (auto& v : vehicles) {
                // Réinitialiser les véhicules
            }
            std::cout << "🔄 Simulation Reset" << std::endl;
        }
        
        // Gestion caméra
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 delta = GetMouseDelta();
            renderer.panCamera(delta);
        }
        
        float wheelMove = GetMouseWheelMove();
        if (wheelMove > 0) renderer.zoomIn();
        else if (wheelMove < 0) renderer.zoomOut();
        
        // Sélection véhicule
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            Position worldPos = renderer.screenToWorld(mousePos);
            
            for (size_t i = 0; i < vehicles.size(); i++) {
                float distance = vehicles[i].getPosition().distanceTo(worldPos);
                if (distance < 20) {
                    selectedVehicle = i;
                    vehicles[i].setIsSelected(true);
                    std::cout << "🚗 Selected: " << vehicles[i].getId() << std::endl;
                    
                    // Désélectionner les autres
                    for (size_t j = 0; j < vehicles.size(); j++) {
                        if (j != i) vehicles[j].setIsSelected(false);
                    }
                    break;
                }
            }
        }
        
        // === UPDATE SIMULATION ===
        if (!simulationPaused) {
            simulationTime += deltaTime;
            
            // Traiter les urgences
            commandCenter->processNextEmergency();
            
            // Assigner des véhicules aux urgences (logique simple)
            // Récupérer l'urgence la plus prioritaire non assignée
            Emergency* nextEmergency = commandCenter->getHighestPriorityEmergency();
            
            if (nextEmergency && nextEmergency->getStatus() == "PENDING") {
                // Trouver un véhicule disponible du bon type
                for (auto& vehicle : vehicles) {
                    if (!vehicle.getAssignedEmergency()) {
                        // Vérifier si le type correspond (simplifié)
                        bool typeMatches = true;
                        if (nextEmergency->getType() == EmergencyType::MEDICAL && 
                            vehicle.getVehicleType() != EmergencyVehicleType::AMBULANCE) {
                            typeMatches = false;
                        }
                        else if (nextEmergency->getType() == EmergencyType::FIRE && 
                                 vehicle.getVehicleType() != EmergencyVehicleType::FIRE_TRUCK) {
                            typeMatches = false;
                        }
                        
                        if (typeMatches) {
                            vehicle.assignEmergency(nextEmergency);
                            nextEmergency->setStatus("ASSIGNED");
                            std::cout << "📡 Assigned " << vehicle.getId() 
                                     << " to Emergency #" << nextEmergency->getId() << std::endl;
                            break;
                        }
                    }
                }
            }
            
            // Mettre à jour les véhicules
            for (auto& vehicle : vehicles) {
                vehicle.update(deltaTime);
            }
        }
        
        // === RENDU ===
        BeginDrawing();
        renderer.beginDrawing();
        
        // Dessiner la ville
        renderer.drawCity();
        
        // Dessiner les urgences actives
        // (Note: Vous devriez ajouter une méthode pour récupérer les urgences du command center)
        
        // Dessiner les véhicules
        for (const auto& vehicle : vehicles) {
            renderer.drawVehicle(vehicle);
        }
        
        // Dessiner le HUD
        // (Note: Vous devriez avoir une liste d'urgences)
        std::vector<Emergency> emergencies; // À remplacer avec vraies données
        renderer.drawHUD(vehicles, emergencies);
        
        // Dessiner les statistiques
        int availableVehicles = std::count_if(vehicles.begin(), vehicles.end(),
            [](const EmergencyVehicle& v) { return !v.getAssignedEmergency(); });
        renderer.drawStats(commandCenter->getActiveEmergencyCount(), 
                          availableVehicles, simulationTime);
        
        // Afficher état pause
        if (simulationPaused) {
            DrawRectangle(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 25, 200, 50, Fade(RED, 0.7f));
            DrawText("PAUSED", SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2 - 10, 30, WHITE);
        }
        
        // Instructions
        DrawText("1:Medical 2:Fire 3:Security 4:Traffic  SPACE:Pause  R:Reset", 
                 10, SCREEN_HEIGHT - 30, 18, LIGHTGRAY);
        
        renderer.endDrawing();
        EndDrawing();
    }
    
    // Nettoyage
    CloseWindow();
    
    std::cout << "\n✅ System shutdown complete.\n";
    return 0;
}