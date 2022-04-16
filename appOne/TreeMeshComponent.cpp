#include "TreeMeshComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "SHADER/SHADER.h"
#include "CONTAINER/TREE.h"
#include "CONTAINER/CONTAINER.h"

TreeMeshComponent::TreeMeshComponent(Actor* owner)
	:MeshComponent(owner)
{
}

TreeMeshComponent::~TreeMeshComponent()
{
	delete mTree;
}

void TreeMeshComponent::SetTree(const char* name)
{
	CONTAINER* c = mOwner->GetGame()->GetRenderer()->GetContainer();
	mTree = new TREE(c->treeOrigin(name));
}

void TreeMeshComponent::Update()
{
	mWorld.identity();
	mWorld.mulTranslate(mOwner->GetPosition());
	mTree->update(mWorld);
}

void TreeMeshComponent::Draw()
{
	mTree->draw(mOwner->GetGame()->GetRenderer()->GetShader());
}
