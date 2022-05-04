#pragma once
#include "Actor.h"
#include "VECTOR.h"
class Camera :
    public Actor
{
public:
    Camera(class Game* game);
    void UpdateActor() override;
private:
    float DistanceFromLookatPos = 0;
    float RotSpeed = 0;
    float ZoomSpeed = 0;
    VECTOR LookatPos;
    VECTOR UpVec;
    float OffsetPosY = 0;
    float ChangePosSpeed = 0;
    //ï‚ê≥íl
    float CorrectPos = 0;
};

