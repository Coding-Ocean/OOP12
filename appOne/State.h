#pragma once
class State
{
public:
	State(class StateComponent* OwnerCompo)
		:mOwnerCompo(OwnerCompo){}
	// �X�e�[�g���̃Q�b�^�[
	virtual const char* GetName() const = 0;
	// �X�e�[�g�ŗL�̓���
	virtual void Update() = 0;
	virtual void OnEnter() {};
	virtual void OnExit() {};
protected:
	class StateComponent* mOwnerCompo;
};
