#pragma once
#include "Actor.h"
class Player :
    public Actor
{
public:
    Player(class Game* gmae);
    class InputComponent* GetIn() { return mIn; }
    class TreeMeshComponent* GetMesh() { return mMesh; }
    enum AnimId { EWait, EWalk, EJump };
    float GetForwardSpeed() { return mForwardSpeed; }
    float GetRotateSpeed() { return mRotateSpeed; }
private:
    class InputComponent* mIn;
    class TreeMeshComponent* mMesh;
    class StateComponent* mState;
    float mForwardSpeed;
    float mRotateSpeed;
};
