#include "MeshComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

MeshComponent::MeshComponent(Actor* owner)
	:Component(owner)
{
	mOwner->GetGame()->GetRenderer()->AddMesh(this);
}

MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMesh(this);
}
