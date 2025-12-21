#include "TrafficManager.h"
#include "raylib.h"
#include <algorithm>

TrafficManager::TrafficManager() {}

void TrafficManager::generateTrafficLights(const std::vector<Position>& positions) {
    for (const auto& pos : positions) {
        trafficLights.push_back({pos, false, 0.0f});
    }
}

void TrafficManager::update(float deltaTime) {
    for (auto& light : trafficLights) {
        light.timer -= deltaTime;
        if (light.timer <= 0) {
            light.isGreen = !light.isGreen;
            light.timer = light.isGreen ? 5.0f : 3.0f; // 5s vert, 3s rouge
        }
    }
}

bool TrafficManager::requestGreenWave(const EmergencyVehicle& vehicle, float duration) {
    Position vehiclePos = vehicle.getPosition();
    
    for (auto& light : trafficLights) {
        if (vehiclePos.distanceTo(light.position) < 100.0f) {
            light.isGreen = true;
            light.timer = duration;
            return true;
        }
    }
    
    return false;
}

void TrafficManager::resetAllLights() {
    for (auto& light : trafficLights) {
        light.isGreen = false;
        light.timer = 3.0f;
    }
}

std::vector<Position> TrafficManager::getGreenLights() const {
    std::vector<Position> result;
    for (const auto& light : trafficLights) {
        if (light.isGreen) {
            result.push_back(light.position);
        }
    }
    return result;
}

std::vector<Position> TrafficManager::getAllLights() const {
    std::vector<Position> result;
    for (const auto& light : trafficLights) {
        result.push_back(light.position);
    }
    return result;
}

void TrafficManager::drawTrafficLights() const {
    for (const auto& light : trafficLights) {
        Color color = light.isGreen ? GREEN : RED;
        DrawCircle(light.position.getX(), light.position.getY(), 8, color);
        DrawCircleLines(light.position.getX(), light.position.getY(), 8, WHITE);
    }
}