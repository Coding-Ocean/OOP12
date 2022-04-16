#pragma once
#include "Component.h"
#include "graphic.h"
class SpriteComponent :
    public Component
{
public:
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Update();
	virtual void Draw();
	virtual void SetImage(int img);

	int GetDrawOrder() const { return mDrawOrder; }
	void SetRectMode(RECT_MODE rectMode) { mRectMode = rectMode; }
	void SetColor(const COLOR& color) { mColor = color; }
private:
    int mDrawOrder;
    int mImg;
	RECT_MODE mRectMode;
	COLOR mColor;
public:
	void StartFlash(float time, const COLOR& color);
private:
	float mFlashTime;
};

