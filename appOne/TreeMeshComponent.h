#pragma once
#include "MeshComponent.h"
class TreeMeshComponent :
    public MeshComponent
{
public:
    TreeMeshComponent(class Actor* owner);
    ~TreeMeshComponent();
    void SetTree(const char* name);
    void Update();
    void Draw();
private:
    class TREE* mTree;
};

