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
	// ���݂̃X�e�[�g����o��
	if (mCurrentState)
	{
		mCurrentState->OnExit();
	}

	// map����V�����X�e�[�g��T��
	auto iter = mStateMap.find(name);
	if (iter != mStateMap.end())
	{
		mCurrentState = iter->second;
		// �V������Ԃɓ���
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
