#ifndef VEHICLE_H
#define VEHICLE_H

#include "Position.h"
#include <string>

class Vehicle {
protected:
    std::string id;
    Position position;
    Position destination;
    float speed;
    float maxSpeed;
    bool isMoving;
    bool isSelected;
    
public:
    Vehicle(const std::string& id, Position startPos, float maxSpeed = 60.0f);
    virtual ~Vehicle() = default;
    
    // Getters
    std::string getId() const { return id; }
    Position getPosition() const { return position; }
    Position getDestination() const { return destination; }
    float getSpeed() const { return speed; }
    bool getIsMoving() const { return isMoving; }
    bool getIsSelected() const { return isSelected; }
    
    // Setters
    void setDestination(Position dest) { destination = dest; }
    void setSpeed(float newSpeed) { speed = newSpeed; }
    void setIsMoving(bool moving) { isMoving = moving; }
    void setIsSelected(bool selected) { isSelected = selected; }
    
    // Méthodes virtuelles (polymorphisme)
    virtual void move(float deltaTime);
    virtual void draw() const;
    virtual void update(float deltaTime);
    
    // Utilitaires
    bool hasArrived() const;
    float distanceToDestination() const;
    void stop() { isMoving = false; speed = 0.0f; }
    
    // Opérateur
    friend std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle);
};

#endif