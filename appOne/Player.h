#pragma once
#include "Actor.h"
class Player :
    public Actor
{
public:
    Player(class Game* gmae);
    void UpdateActor() override;
    class InputComponent* GetIn() { return mIn; }
    class TreeMeshComponent* GetMesh() { return mMesh; }
    enum AnimId { EWait, EWait2, EWalk, EJump };
    float GetForwardSpeed() { return mForwardSpeed; }
    float GetRotateRatio() { return mRotateRatio; }
    float GetJumpVelocity() { return mJumpVelocity; }
    void SetJumpVelocity(float vel) { mJumpVelocity = vel; }
    float GetGravity() { return mGravity; }
    int GetJumpFlag() { return mJumpFlag; }
    void SetJumpFlag() { mJumpFlag = 1; }
    void InitPos();
private:
    class InputComponent* mIn;
    class TreeMeshComponent* mMesh;
    class StateComponent* mState;
    //�ǉ�����
    float mForwardSpeed;
    float mRotateRatio;
    float mJumpVelocity;
    float mGravity;
    int mJumpFlag;
};
