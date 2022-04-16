#include "AnimSpriteComponent.h"
#include "window.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurIdx(0)
	, mTimer(0.0f)
	, mInterval(0.0f)
{
}

void AnimSpriteComponent::AddImage(int img)
{
	mImgs.emplace_back(img);
}

void AnimSpriteComponent::Update()
{
	SpriteComponent::Update();

	mTimer += delta;
	if (mTimer > mInterval)
	{
		mTimer -= mInterval;
		mCurIdx++;
		mCurIdx %= mImgs.size();
	}
}

void AnimSpriteComponent::Draw()
{
	SetImage(mImgs[mCurIdx]);
	SpriteComponent::Draw();
}

