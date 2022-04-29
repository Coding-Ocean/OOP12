#include "StateComponent.h"
#include "State.h"
#include "Actor.h"

StateComponent::StateComponent(class Actor* owner)
	:Component(owner)
	, mCurrentState(nullptr)
{
}

StateComponent::~StateComponent()
{
	for (auto& stateMap : mStateMap)
	{
		delete stateMap.second;
	}
}

void StateComponent::Update()
{
	if (mCurrentState)
	{
		mCurrentState->Update();
	}
}

void StateComponent::ChangeState(const std::string& name)
{
	// 現在のステートから出る
	if (mCurrentState)
	{
		mCurrentState->OnExit();
	}

	// mapから新しいステートを探す
	auto iter = mStateMap.find(name);
	if (iter != mStateMap.end())
	{
		mCurrentState = iter->second;
		// 新しい状態に入る
		mCurrentState->OnEnter();
	}
	else
	{
		mCurrentState = nullptr;
	}
}

void StateComponent::RegisterState(State* state)
{
	mStateMap.emplace(state->GetName(), state);
}

std::string StateComponent::GetName() 
{ 
	return mCurrentState->GetName(); 
}
