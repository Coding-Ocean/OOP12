#pragma once
#include "Component.h"
#include "VECTOR.h"

class MoveComponent :
    public Component
{
public:
    MoveComponent(class Actor* owner, int order=100);
    void Update() override;
    void SetDirection(const VECTOR& direction) { mDirection = direction; }
    void SetSpeed(float speed) { mSpeed = speed; }
    float GetSpeed() { return mSpeed; }
private:
    VECTOR mDirection;
    float mSpeed;
};

