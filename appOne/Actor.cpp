#include "Actor.h"
#include "Game.h"
#include "Component.h"

Actor::Actor(Game* game)
	: mGame(game)
	, mState(EActive)
	, mRotationY(0.0f)
	, mRotationX(0.0f)
	, mRotationZ(0.0f)
	, mScale(1.0f,1.0f,1.0f)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);

	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Actor::ProcessInput()
{
	if (mState == EActive)
	{
		for (auto comp : mComponents)
		{
			comp->ProcessInput();
		}

		ActorInput();
	}
}

void Actor::Update()
{
	if (mState == EActive)
	{
		for (auto component : mComponents)
		{
			component->Update();
		}

		UpdateActor();
	}
}

void Actor::AddComponent(Component* component)
{
	// ソート済み配列の挿入場所を探す
	// (自分より順番の大きい最初の要素を探す)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (  ; iter != mComponents.end(); ++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	// 探し出した要素の前に自分を挿入
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}
