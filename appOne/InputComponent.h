#pragma once
#include "Component.h"
class InputComponent :
    public Component
{
public:
    InputComponent(class Actor* owner);
    void ProcessInput() override;
    int Jump() { return mJump; }
    bool Right() { return mRight; }
    bool Left() { return mLeft; }
    bool Up() { return mUp; }
    bool Down() { return mDown; }
    bool StartWalk() { return mWalk; }
    bool StopWalk() { return !mWalk; }
private:
    int mJump;
    bool mWalk;

    int mRight;
    int mLeft;
    int mUp;
    int mDown;
};
