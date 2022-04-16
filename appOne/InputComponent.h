#pragma once
#include "MoveComponent.h"
class InputComponent :
    public MoveComponent
{
public:
	// Lower update order to update first
	InputComponent(class Actor* owner);
	void ProcessInput() override;
	void SetMoveSpeed(float speed) { mMoveSpeed = speed; }
private:
	float mMoveSpeed;
};
