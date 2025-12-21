#include "Renderer.h"
#include "EmergencyVehicle.h"
#include "Emergency.h"
#include <iostream>
#include <string>

Renderer::Renderer(int width, int height) 
    : screenWidth(width), screenHeight(height) {
    
    // Initialisation des couleurs
    roadColor = Color{50, 50, 50, 255};
    buildingColor = Color{100, 100, 100, 255};
    trafficLightColor = Color{255, 255, 0, 255};
    hospitalColor = BLUE;
    
    // Initialisation de la caméra
    camera = {0};
    camera.target = Vector2{0, 0};
    camera.offset = Vector2{screenWidth/2.0f, screenHeight/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void Renderer::init() {
    generateCityLayout();
}

void Renderer::generateCityLayout() {
    // Génération simple d'une ville
    
    // Routes principales
    roads.clear();
    for (int i = 0; i < 5; i++) {
        roads.push_back(Rectangle{100.0f + i * 200.0f, 0, 40, 800});
        roads.push_back(Rectangle{0, 100.0f + i * 200.0f, 1200, 40});
    }
    
    // Bâtiments
    buildings.clear();
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            float x = 50.0f + i * 120.0f;
            float y = 50.0f + j * 120.0f;
            
            // Éviter les routes
            bool onRoad = false;
            for (const auto& road : roads) {
                if (CheckCollisionPointRec(Vector2{x, y}, road)) {
                    onRoad = true;
                    break;
                }
            }
            
            if (!onRoad) {
                buildings.push_back(Rectangle{x, y, 60, 60});
            }
        }
    }
    
    // Feux de circulation aux intersections
    trafficLights.clear();
    for (int i = 1; i < 5; i++) {
        for (int j = 1; j < 5; j++) {
            trafficLights.push_back(Vector2{100.0f + i * 200.0f, 100.0f + j * 200.0f});
        }
    }
    
    // Hôpitaux
    hospitals.clear();
    hospitals.push_back(Vector2{800, 600});
    hospitals.push_back(Vector2{200, 500});
    hospitals.push_back(Vector2{1000, 400});
}

void Renderer::beginDrawing() {
    ClearBackground(Color{30, 30, 30, 255});
    BeginMode2D(camera);
}

void Renderer::endDrawing() {
    EndMode2D();
}

void Renderer::drawCity() {
    drawRoads();
    drawBuildings();
    drawTrafficLights();
    drawHospitals();
}

void Renderer::drawRoads() {
    for (const auto& road : roads) {
        DrawRectangleRec(road, roadColor);
        // Lignes de séparation
        DrawRectangleLinesEx(road, 2, Color{255, 255, 255, 100});
    }
}

void Renderer::drawBuildings() {
    for (const auto& building : buildings) {
        DrawRectangleRec(building, buildingColor);
        DrawRectangleLinesEx(building, 2, DARKGRAY);
    }
}

void Renderer::drawTrafficLights() {
    for (const auto& light : trafficLights) {
        DrawCircle(light.x, light.y, 8, trafficLightColor);
        DrawCircleLines(light.x, light.y, 8, WHITE);
    }
}

void Renderer::drawHospitals() {
    for (const auto& hospital : hospitals) {
        DrawCircle(hospital.x, hospital.y, 25, hospitalColor);
        DrawCircleLines(hospital.x, hospital.y, 25, WHITE);
        DrawText("H", hospital.x - 8, hospital.y - 15, 20, WHITE);
    }
}

void Renderer::drawVehicle(const EmergencyVehicle& vehicle) {
    vehicle.draw(); // Le véhicule se dessine lui-même
}

void Renderer::drawEmergency(const Emergency& emergency) {
    Vector2 pos = worldToScreen(emergency.getLocation());
    
    // Dessiner un cercle pulsant pour l'urgence
    static float pulse = 0.0f;
    pulse += 0.05f;
    float radius = 20 + sin(pulse) * 5;
    
    Color emergencyColor;
    switch(emergency.getPriority()) {
        case PriorityLevel::CRITICAL: emergencyColor = RED; break;
        case PriorityLevel::HIGH: emergencyColor = ORANGE; break;
        case PriorityLevel::MEDIUM: emergencyColor = YELLOW; break;
        case PriorityLevel::LOW: emergencyColor = GREEN; break;
    }
    
    DrawCircle(pos.x, pos.y, radius, Fade(emergencyColor, 0.7f));
    DrawCircleLines(pos.x, pos.y, radius, emergencyColor);
    
    // Texte
    DrawText(("E#" + std::to_string(emergency.getId())).c_str(), 
             pos.x - 15, pos.y - 30, 12, WHITE);
}

void Renderer::drawHUD(const std::vector<EmergencyVehicle>& vehicles, 
                      const std::vector<Emergency>& emergencies) {
    // Panneau latéral
    DrawRectangle(screenWidth - 300, 0, 300, screenHeight, Color{0, 0, 0, 200});
    
    // Titre
    DrawText("EMERGENCY CONTROL", screenWidth - 290, 20, 20, RED);
    
    // Véhicules
    DrawText("VEHICLES:", screenWidth - 290, 60, 18, WHITE);
    for (size_t i = 0; i < vehicles.size(); i++) {
        const auto& v = vehicles[i];
        std::string text = v.getId() + " - " + v.getTypeString();
        if (v.getAssignedEmergency()) {
            text += " (On Mission)";
        }
        DrawText(text.c_str(), screenWidth - 280, 90 + i * 30, 14, 
                v.getAssignedEmergency() ? RED : GREEN);
    }
    
    // Urgences
    DrawText("ACTIVE EMERGENCIES:", screenWidth - 290, 300, 18, WHITE);
    for (size_t i = 0; i < emergencies.size(); i++) {
        const auto& e = emergencies[i];
        std::string text = "E#" + std::to_string(e.getId()) + 
                          " - " + e.getPriorityString();
        DrawText(text.c_str(), screenWidth - 280, 330 + i * 25, 14, WHITE);
    }
}

void Renderer::drawStats(int activeEmergencies, int availableVehicles, float time) {
    DrawRectangle(10, 10, 250, 120, Color{0, 0, 0, 180});
    
    DrawText("SYSTEM STATUS", 20, 20, 20, GREEN);
    DrawText(("Active Emergencies: " + std::to_string(activeEmergencies)).c_str(), 
             20, 50, 18, WHITE);
    DrawText(("Available Vehicles: " + std::to_string(availableVehicles)).c_str(), 
             20, 75, 18, WHITE);
    DrawText(("Simulation Time: " + std::to_string((int)time) + "s").c_str(), 
             20, 100, 18, WHITE);
}

void Renderer::updateCamera() {
    // Logique de mise à jour de la caméra
}

void Renderer::zoomIn() {
    camera.zoom += 0.1f;
    if (camera.zoom > 3.0f) camera.zoom = 3.0f;
}

void Renderer::zoomOut() {
    camera.zoom -= 0.1f;
    if (camera.zoom < 0.5f) camera.zoom = 0.5f;
}

void Renderer::panCamera(Vector2 delta) {
    camera.target.x -= delta.x / camera.zoom;
    camera.target.y -= delta.y / camera.zoom;
}

Vector2 Renderer::worldToScreen(Position worldPos) const {
    Vector2 result;
    result.x = (worldPos.getX() - camera.target.x) * camera.zoom + camera.offset.x;
    result.y = (worldPos.getY() - camera.target.y) * camera.zoom + camera.offset.y;
    return result;
}

Position Renderer::screenToWorld(Vector2 screenPos) const {
    float worldX = (screenPos.x - camera.offset.x) / camera.zoom + camera.target.x;
    float worldY = (screenPos.y - camera.offset.y) / camera.zoom + camera.target.y;
    return Position(worldX, worldY);
}