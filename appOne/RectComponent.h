#pragma once
#include "Component.h"
#include "VECTOR.h"
class RectComponent :
    public Component
{
public:
    RectComponent(class Actor* owner, int order);
    const VECTOR& GetCenter()const;
    void SetHalfW(float w) { mHalfW = w; }
    void SetHalfH(float h) { mHalfH = h; }
    float GetHalfW() const{ return mHalfW; }
    float GetHalfH() const{ return mHalfH; }
private:
    float mHalfW;
    float mHalfH;
};

bool Intersect(const RectComponent* a, const RectComponent* b);
