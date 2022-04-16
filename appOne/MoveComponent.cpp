#include "MoveComponent.h"
#include "Actor.h"
#include "window.h"

MoveComponent::MoveComponent(Actor* owner, int order)
	:Component(owner,order)
	,mDirection(0,0)
	,mSpeed(0)
{
}

void MoveComponent::Update()
{
	if (mSpeed != 0.0f)
	{
		VECTOR pos = mOwner->GetPosition();
		pos += mDirection * mSpeed * delta;
		mOwner->SetPosition(pos);
	}
}
