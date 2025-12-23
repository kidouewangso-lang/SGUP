#include "Vehicule.h"
#include <iostream>
#include <cmath>

Vehicle::Vehicle(const std::string& id, Position startPos, float maxSpeed)
    : id(id), position(startPos), destination(startPos), 
      speed(0.0f), maxSpeed(maxSpeed), isMoving(false), isSelected(false) {}

void Vehicle::move(float deltaTime) {
    if (!isMoving || hasArrived()) return;
    
    // Calculer la direction
    float dx = destination.getX() - position.getX();
    float dy = destination.getY() - position.getY();
    float distance = std::sqrt(dx*dx + dy*dy);
    
    if (distance > 0.1f) {
        // Normaliser et déplacer
        float dirX = dx / distance;
        float dirY = dy / distance;
        
        float moveDistance = speed * deltaTime;
        if (moveDistance > distance) {
            moveDistance = distance;
        }
        
        position.setX(position.getX() + dirX * moveDistance);
        position.setY(position.getY() + dirY * moveDistance);
    } else {
        // Arrivé à destination
        position = destination;
        isMoving = false;
        speed = 0.0f;
    }
}

void Vehicle::draw() const {
    // Dessin par défaut (cercle)
    // Sera override par les classes enfants
}

void Vehicle::update(float deltaTime) {
    move(deltaTime);
}

bool Vehicle::hasArrived() const {
    return position.distanceTo(destination) < 5.0f; // Tolérance de 5 unités
}

float Vehicle::distanceToDestination() const {
    return position.distanceTo(destination);
}

std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle) {
    os << "Vehicle " << vehicle.id 
       << " at " << vehicle.position.toString()
       << " moving to " << vehicle.destination.toString()
       << " speed: " << vehicle.speed << "/" << vehicle.maxSpeed;
    return os;
}