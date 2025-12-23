#include "GUI.h"
#include <iostream>
#include <sstream>

GUI::GUI(int width, int height) : screenWidth(width), screenHeight(height) {
    camera = {0};
    camera.target = Vector2{0, 0};
    camera.offset = Vector2{width/2.0f, height/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void GUI::init() {
    createButtons();
}

void GUI::update() {
    for (auto& button : buttons) {
        button.pressed = false;
        
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, button.bounds) && 
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            button.pressed = true;
        }
    }
    
    handleCameraControls();
}

void GUI::draw(EmergencyCommandCenter* commandCenter, float simulationTime, bool isPaused) {
    // Draw background panel
    DrawRectangle(10, 10, 300, screenHeight - 20, Color{0, 0, 0, 200});
    
    // Draw title
    DrawText("EMERGENCY CONTROL", 20, 20, 24, RED);
    
    // Draw buttons
    for (const auto& button : buttons) {
        Color btnColor = button.pressed ? Color{button.color.r/2, button.color.g/2, button.color.b/2, 255} : button.color;
        DrawRectangleRec(button.bounds, btnColor);
        DrawRectangleLinesEx(button.bounds, 2, WHITE);
        
        int textWidth = MeasureText(button.text.c_str(), 20);
        DrawText(button.text.c_str(), 
                button.bounds.x + button.bounds.width/2 - textWidth/2,
                button.bounds.y + button.bounds.height/2 - 10,
                20, WHITE);
    }
    
    // Draw statistics
    drawStats(commandCenter, simulationTime, isPaused);
    
    // Draw emergency list
    drawEmergencyList(commandCenter);
    
    // Draw vehicle list
    drawVehicleList(commandCenter);
}

bool GUI::isButtonPressed(const std::string& buttonText) const {
    for (const auto& button : buttons) {
        if (button.text == buttonText && button.pressed) {
            return true;
        }
    }
    return false;
}

Camera2D& GUI::getCamera() {
    return camera;
}

void GUI::handleCameraControls() {
    // Zoom with mouse wheel
    float wheelMove = GetMouseWheelMove();
    if (wheelMove > 0) {
        camera.zoom += 0.1f;
        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
    } else if (wheelMove < 0) {
        camera.zoom -= 0.1f;
        if (camera.zoom < 0.5f) camera.zoom = 0.5f;
    }
    
    // Pan with right mouse button
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 delta = GetMouseDelta();
        camera.target.x -= delta.x / camera.zoom;
        camera.target.y -= delta.y / camera.zoom;
    }
}

void GUI::createButtons() {
    buttons.clear();
    
    int startY = 70;
    int buttonWidth = 260;
    int buttonHeight = 40;
    int buttonSpacing = 10;
    
    std::vector<std::pair<std::string, Color>> buttonConfigs = {
        {"MEDICAL EMERGENCY", RED},
        {"FIRE EMERGENCY", ORANGE},
        {"SECURITY ALERT", BLUE},
        {"TRAFFIC ACCIDENT", YELLOW},
        {"PAUSE/RESUME", GRAY},
        {"RESET SYSTEM", DARKGRAY}
    };
    
    for (int i = 0; i < buttonConfigs.size(); i++) {
        Button btn;
        btn.bounds = Rectangle{20.0f, (float)(startY + i * (buttonHeight + buttonSpacing)), 
                              (float)buttonWidth, (float)buttonHeight};
        btn.text = buttonConfigs[i].first;
        btn.color = buttonConfigs[i].second;
        btn.pressed = false;
        buttons.push_back(btn);
    }
}

void GUI::drawStats(EmergencyCommandCenter* commandCenter, float simulationTime, bool isPaused) {
    int startY = 350;
    
    DrawText("SYSTEM STATISTICS", 20, startY, 20, GREEN);
    
    std::stringstream ss;
    ss << "Active Emergencies: " << commandCenter->getActiveEmergencyCount();
    DrawText(ss.str().c_str(), 30, startY + 30, 18, WHITE);
    
    ss.str("");
    ss << "Available Vehicles: " << commandCenter->getAvailableVehicleCount();
    DrawText(ss.str().c_str(), 30, startY + 55, 18, WHITE);
    
    ss.str("");
    ss << "Simulation Time: " << (int)simulationTime << "s";
    DrawText(ss.str().c_str(), 30, startY + 80, 18, WHITE);
    
    ss.str("");
    ss << "Status: " << (isPaused ? "PAUSED" : "RUNNING");
    DrawText(ss.str().c_str(), 30, startY + 105, 18, isPaused ? RED : GREEN);
}

void GUI::drawEmergencyList(EmergencyCommandCenter* commandCenter) {
    int startY = 480;
    
    DrawText("ACTIVE EMERGENCIES", 20, startY, 20, YELLOW);
    
    auto emergencies = commandCenter->getEmergencyManager()->getActiveEmergencies();
    for (size_t i = 0; i < emergencies.size() && i < 5; i++) {
        std::stringstream ss;
        ss << "E#" << emergencies[i]->getId() 
           << " - " << emergencies[i]->getPriorityString();
        
        DrawText(ss.str().c_str(), 30, startY + 30 + (int)i * 25, 16, WHITE);
    }
}

void GUI::drawVehicleList(EmergencyCommandCenter* commandCenter) {
    int startY = 600;
    
    DrawText("VEHICLE STATUS", 20, startY, 20, BLUE);
    
    auto vehicles = commandCenter->getVehicleManager()->getAllVehicles();
    for (size_t i = 0; i < vehicles.size() && i < 5; i++) {
        std::string status = vehicles[i]->getAssignedEmergency() ? "ON MISSION" : "AVAILABLE";
        Color color = vehicles[i]->getAssignedEmergency() ? RED : GREEN;
        
        std::string text = vehicles[i]->getId() + " - " + status;
        DrawText(text.c_str(), 30, startY + 30 + (int)i * 25, 16, color);
    }
}