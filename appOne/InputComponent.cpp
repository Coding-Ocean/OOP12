#include "InputComponent.h"
#include "input.h"

InputComponent::InputComponent(Actor* owner)
	:MoveComponent(owner)
	,mMoveSpeed(300)
{
}

void InputComponent::ProcessInput()
{
	VECTOR dir(0,0);
	if (isPress(KEY_W))
	{
		dir.y = -1;
	}
	else if (isPress(KEY_S))
	{
		dir.y = 1;
	}

	if (isPress(KEY_D))
	{
		dir.x = 1;
	}
	else if (isPress(KEY_A))
	{
		dir.x = -1;
	}

	SetSpeed(0);
	if (dir.x != 0 || dir.y != 0)
	{
		SetSpeed(mMoveSpeed);
		SetDirection(normalize(dir));
	}
}
