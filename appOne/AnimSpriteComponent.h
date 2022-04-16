#pragma once
#include "SpriteComponent.h"
#include <vector>
class AnimSpriteComponent :
    public SpriteComponent
{
public:
    AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
    void AddImage(int img);
    void Update() override;
    void Draw() override;
    void SetInterval(float interval) { mInterval = interval; }
private:
    std::vector<int>mImgs;
    int mCurIdx;
    float mTimer;
    float mInterval;
};

