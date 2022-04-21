#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "window.h"
#include "Renderer.h"

SpriteComponent::SpriteComponent(class Actor* owner, int drawOrder)
	:Component(owner)
	, mImg(-1)
	, mDrawOrder(drawOrder)
	, mRectMode(CENTER)
	, mColor(255,255,255)
	, mFlashTime(0.0f)
{
	mOwner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::SetImage(int img)
{
	mImg = img;
}

void SpriteComponent::StartFlash(float time, const COLOR& color)
{
	mFlashTime = time;
	mColor = color;
}

void SpriteComponent::Update()
{
	if (mFlashTime > 0) 
	{ 
		mFlashTime -= delta; 
	}
	else 
	{ 
		mColor = COLOR(255, 255, 255, 255); 
	}
}

void SpriteComponent::Draw()
{
	VECTOR p = mOwner->GetPosition();
	float a = mOwner->GetRotationZ();
	float s = mOwner->GetScale().x;
	rectMode(mRectMode);
	imageColor(mColor);
	image(mImg, p.x, p.y, a, s);
}
