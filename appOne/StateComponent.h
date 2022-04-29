#pragma once
#include <unordered_map>
#include <string>
#include "Component.h"

class StateComponent :
	public Component
{
public:
	StateComponent(class Actor* owner);
	~StateComponent();

	void Update() override;
	void ChangeState(const std::string& name);

	// �V�����X�e�[�g��map�ɒǉ�
	void RegisterState(class State* state);
	class Actor* GetActor() { return mOwner; }
	std::string GetName();
private:
	// ���X�e�[�g���A�X�e�[�g��map
	std::unordered_map<std::string, class State*> mStateMap;
	class State* mCurrentState;
};

