#ifndef GUI_H
#define GUI_H

#include "raylib.h"
#include "EmergencyCommandCenter.h"
#include <vector>
#include <string>

class GUI {
private:
    int screenWidth;
    int screenHeight;
    
    struct Button {
        Rectangle bounds;
        std::string text;
        Color color;
        bool pressed;
    };
    
    std::vector<Button> buttons;
    Camera2D camera;
    
public:
    GUI(int width, int height);
    ~GUI() = default;
    
    void init();
    void update();
    void draw(EmergencyCommandCenter* commandCenter, float simulationTime, bool isPaused);
    
    bool isButtonPressed(const std::string& buttonText) const;
    
    Camera2D& getCamera();
    void handleCameraControls();
    
private:
    void createButtons();
    void drawStats(EmergencyCommandCenter* commandCenter, float simulationTime, bool isPaused);
    void drawEmergencyList(EmergencyCommandCenter* commandCenter);
    void drawVehicleList(EmergencyCommandCenter* commandCenter);
};

#endif