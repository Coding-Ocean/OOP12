#include "BatchMeshComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "CONTAINER/CONTAINER.h"
#include "CONTAINER/BATCH.h"

BatchMeshComponent::BatchMeshComponent(Actor* owner)
	:MeshComponent(owner)
	,mBatch(nullptr)
{
}

BatchMeshComponent::~BatchMeshComponent()
{
}

void BatchMeshComponent::SetBatch(const char* name)
{
	CONTAINER* c = mOwner->GetGame()->GetRenderer()->GetContainer();
	mBatch = c->batch(name);
}

void BatchMeshComponent::Draw()
{
	mWorld.identity();
	mWorld.mulTranslate(mOwner->GetPosition());
	mWorld.mulRotateY(mOwner->GetRotationY());
	mWorld.mulRotateX(mOwner->GetRotationX());
	mWorld.mulRotateZ(mOwner->GetRotationZ());
	mWorld.mulScaling(mOwner->GetScale());
	mBatch->draw(mOwner->GetGame()->GetRenderer()->GetShader(), mWorld);
}
