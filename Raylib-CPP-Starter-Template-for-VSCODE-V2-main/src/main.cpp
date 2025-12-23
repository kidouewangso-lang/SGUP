#include "raylib.h"
#include "EmergencyCommandCenter.h"
#include "GUI.h"
#include <iostream>
#include <vector>

const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 900;
const char* WINDOW_TITLE = "Smart City - Emergency & Priority Management";

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
    
    EmergencyCommandCenter* commandCenter = EmergencyCommandCenter::getInstance();
    GUI gui(SCREEN_WIDTH, SCREEN_HEIGHT);
    gui.init(); 
    
    // Add initial vehicles
    commandCenter->addVehicle(EmergencyVehicleType::AMBULANCE, Position(200, 200), "AMB-001");
    commandCenter->addVehicle(EmergencyVehicleType::AMBULANCE, Position(400, 300), "AMB-002");
    commandCenter->addVehicle(EmergencyVehicleType::FIRE_TRUCK, Position(600, 400), "FT-001");
    commandCenter->addVehicle(EmergencyVehicleType::POLICE, Position(800, 500), "POL-001");
    commandCenter->addVehicle(EmergencyVehicleType::POLICE, Position(1000, 600), "POL-002");
    
    // Generate traffic lights
    std::vector<Position> trafficLightPositions = {
        Position(300, 300), Position(500, 300), Position(700, 300),
        Position(300, 500), Position(500, 500), Position(700, 500),
        Position(300, 700), Position(500, 700), Position(700, 700)
    };
    commandCenter->getTrafficManager()->generateTrafficLights(trafficLightPositions);
    
    float simulationTime = 0.0f;
    bool simulationPaused = false;
    
    std::cout << "=============================================\n";
    std::cout << "🚨 SMART CITY EMERGENCY MANAGEMENT SYSTEM 🚨\n";
    std::cout << "=============================================\n";
    std::cout << "Controls:\n";
    std::cout << "  Use buttons in the left panel\n";
    std::cout << "  Mouse Wheel - Zoom in/out\n";
    std::cout << "  Right Click - Pan camera\n";
    std::cout << "=============================================\n";
    
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        
        // Update GUI
        gui.update();
        
        // Handle button presses
        if (gui.isButtonPressed("MEDICAL EMERGENCY")) {
            Position randomPos(GetRandomValue(100, 1300), GetRandomValue(100, 800));
            commandCenter->receiveAlert(EmergencyType::MEDICAL, randomPos, "Medical emergency reported");
        }
        if (gui.isButtonPressed("FIRE EMERGENCY")) {
            Position randomPos(GetRandomValue(100, 1300), GetRandomValue(100, 800));
            commandCenter->receiveAlert(EmergencyType::FIRE, randomPos, "Fire reported");
        }
        if (gui.isButtonPressed("SECURITY ALERT")) {
            Position randomPos(GetRandomValue(100, 1300), GetRandomValue(100, 800));
            commandCenter->receiveAlert(EmergencyType::SECURITY, randomPos, "Security alert");
        }
        if (gui.isButtonPressed("TRAFFIC ACCIDENT")) {
            Position randomPos(GetRandomValue(100, 1300), GetRandomValue(100, 800));
            commandCenter->receiveAlert(EmergencyType::TRAFFIC_ACCIDENT, randomPos, "Traffic accident");
        }
        if (gui.isButtonPressed("PAUSE/RESUME")) {
            simulationPaused = !simulationPaused;
            std::cout << (simulationPaused ? "⏸️ Simulation Paused" : "▶️ Simulation Resumed") << std::endl;
        }
        if (gui.isButtonPressed("RESET SYSTEM")) {
            commandCenter->reset();
            std::cout << "🔄 System Reset" << std::endl;
        }
        
        // Update simulation if not paused
        if (!simulationPaused) {
            simulationTime += deltaTime;
            commandCenter->update(deltaTime);
            commandCenter->processNextEmergency();
        }
        
        // Drawing
        BeginDrawing();
        ClearBackground(Color{30, 30, 30, 255});
        
        BeginMode2D(gui.getCamera());
        
        // Draw city elements (simplified)
        DrawRectangle(0, 0, 1400, 900, Color{40, 40, 40, 255});
        
        // Draw roads
        for (int i = 0; i < 7; i++) {
            DrawRectangle(0, 150 + i * 150, 1400, 40, Color{60, 60, 60, 255});
            DrawRectangle(150 + i * 150, 0, 40, 900, Color{60, 60, 60, 255});
        }
        
        // Draw buildings
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 6; j++) {
                if ((i + j) % 3 != 0) { // Skip some spots for roads
                    DrawRectangle(200 + i * 175, 200 + j * 200, 100, 80, Color{80, 80, 80, 255});
                    DrawRectangleLines(200 + i * 175, 200 + j * 200, 100, 80, DARKGRAY);
                }
            }
        }
        
        // Draw system elements
        commandCenter->draw();
        
        // Draw hospitals
        DrawCircle(800, 600, 30, BLUE);
        DrawText("H", 795, 590, 20, WHITE);
        DrawCircle(200, 500, 30, BLUE);
        DrawText("H", 195, 490, 20, WHITE);
        DrawCircle(1000, 400, 30, BLUE);
        DrawText("H", 995, 390, 20, WHITE);
        
        EndMode2D();
        
        // Draw GUI
        gui.draw(commandCenter, simulationTime, simulationPaused);
        
        // Draw pause indicator
        if (simulationPaused) {
            DrawRectangle(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 50, 200, 100, Color{0, 0, 0, 200});
            DrawText("PAUSED", SCREEN_WIDTH/2 - 60, SCREEN_HEIGHT/2 - 30, 40, RED);
        }
        
        EndDrawing();
    }
    
    CloseWindow();
    std::cout << "\n✅ System shutdown complete.\n";
    return 0;
}