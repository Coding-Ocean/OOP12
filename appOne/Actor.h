#pragma once
#include "VECTOR.h"
#include <vector>
class Actor
{
public:
	enum State
	{
		EActive, EPause, EDead
	};

	Actor(class Game* game);
	virtual ~Actor();
	
	void ProcessInput();
	virtual void ActorInput() {}

	void Update();
	virtual void UpdateActor() {}

	//Getters,Setters
	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	const VECTOR& GetPosition() const { return mPosition; }
	void SetPosition(const VECTOR& pos) { mPosition = pos; }
	float GetRotationY() const { return mRotationY; }
	float GetRotationX() const { return mRotationX; }
	float GetRotationZ() const { return mRotationZ; }
	void SetRotationY(float rotationY) { mRotationY = rotationY; }
	void SetRotationX(float rotationX) { mRotationX = rotationX; }
	void SetRotationZ(float rotationZ) { mRotationZ = rotationZ; }
	const VECTOR& GetScale() const { return mScale; }
	void SetScale(const VECTOR& scale) { mScale = scale; }
	void SetScale(float scale) { mScale = VECTOR(scale,scale,scale); }

	class Game* GetGame() { return mGame; }

	void AddComponent(class Component*);
	void RemoveComponent(class Component*);
private:
	//Actorの状態
	State mState;
	//トランスフォーム
	VECTOR mPosition;
	float mRotationY;
	float mRotationX;
	float mRotationZ;
	VECTOR mScale;
	//Gameクラスのpublicメンバにアクセスするポインタ
	class Game* mGame;
	//component配列
	std::vector<class Component*> mComponents;
};

