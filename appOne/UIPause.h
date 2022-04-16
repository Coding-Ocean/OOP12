#pragma once
#include "UIScreen.h"
class UIPause :
    public UIScreen
{
public:
    UIPause(class Game* game);
    ~UIPause();
    void ProcessInput() override;
    void Draw() override;
};
