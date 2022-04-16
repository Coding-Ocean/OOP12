#pragma once
#include "Component.h"
#include "MATRIX.h"
class MeshComponent :
	public Component
{
public:
	MeshComponent(class Actor*);
	virtual ~MeshComponent();
	virtual void Update() {};
	virtual void Draw() {};
protected:
	MATRIX mWorld;
};

