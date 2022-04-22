#pragma once
class State
{
public:
	State(class StateComponent* OwnerCompo)
		:mOwnerCompo(OwnerCompo){}
	// ステート名のゲッター
	virtual const char* GetName() const = 0;
	// ステート固有の動き
	virtual void Update() = 0;
	virtual void OnEnter() {};
	virtual void OnExit() {};
protected:
	class StateComponent* mOwnerCompo;
};
