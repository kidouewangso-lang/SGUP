#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include <vector>
#include "Position.h"

class EmergencyVehicle;
class Emergency;

class Renderer {
private:
    int screenWidth;
    int screenHeight;
    Camera2D camera;
    
    // Éléments de la ville
    std::vector<Rectangle> roads;
    std::vector<Rectangle> buildings;
    std::vector<Vector2> trafficLights;
    std::vector<Vector2> hospitals;
    
    // Couleurs
    Color roadColor;
    Color buildingColor;
    Color trafficLightColor;
    Color hospitalColor;
    
public:
    Renderer(int width = 1200, int height = 800);
    ~Renderer() = default;
    
    // Initialisation
    void init();
    void generateCityLayout();
    
    // Rendu
    void beginDrawing();
    void endDrawing();
    
    // Dessin des éléments
    void drawCity();
    void drawRoads();
    void drawBuildings();
    void drawTrafficLights();
    void drawHospitals();
    
    // Dessin des entités
    void drawVehicle(const class EmergencyVehicle& vehicle);
    void drawEmergency(const class Emergency& emergency);
    
    // Interface
    void drawHUD(const std::vector<EmergencyVehicle>& vehicles, 
                const std::vector<Emergency>& emergencies);
    void drawStats(int activeEmergencies, int availableVehicles, float time);
    
    // Utilitaires caméra
    void updateCamera();
    void zoomIn();
    void zoomOut();
    void panCamera(Vector2 delta);
    
    // Getters
    Camera2D& getCamera() { return camera; }
    int getScreenWidth() const { return screenWidth; }
    int getScreenHeight() const { return screenHeight; }
    
    // Conversion coordonnées
    Vector2 worldToScreen(Position worldPos) const;
    Position screenToWorld(Vector2 screenPos) const;
};

#endif