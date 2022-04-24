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
    float GetRotateRatio() { return mRotateRatio; }
    float GetJumpVelocity() { return mJumpVelocity; }
    float GetGravity() { return mGravity; }
private:
    class InputComponent* mIn;
    class TreeMeshComponent* mMesh;
    class StateComponent* mState;
    //í«â¡ëÆê´
    float mForwardSpeed;
    float mRotateRatio;
    float mJumpVelocity;
    float mGravity;
};
