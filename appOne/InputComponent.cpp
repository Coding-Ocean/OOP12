#include "InputComponent.h"
#include "input.h"

InputComponent::InputComponent(Actor* owner)
	:Component(owner)
{
}

void InputComponent::ProcessInput()
{
	mJump = isTrigger(KEY_J);
	mLeft = isPress(KEY_A);
	mRight = isPress(KEY_D);
	mUp = isPress(KEY_W);
	mDown = isPress(KEY_S);
	mWalk = mLeft || mRight || mUp || mDown;
}
