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

	// 新しいステートをmapに追加
	void RegisterState(class State* state);
	class Actor* GetActor() { return mOwner; }
	std::string GetName();
private:
	// ＜ステート名、ステート＞map
	std::unordered_map<std::string, class State*> mStateMap;
	class State* mCurrentState;
};

