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
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }

	class Game* GetGame() { return mGame; }

	void AddComponent(class Component*);
	void RemoveComponent(class Component*);
private:
	//Actor�̏��
	State mState;
	//�g�����X�t�H�[��
	VECTOR mPosition;
	float mRotation;
	float mScale;
	//Game�N���X��public�����o�ɃA�N�Z�X����|�C���^
	class Game* mGame;
	//component�z��
	std::vector<class Component*> mComponents;
};

