#pragma once
#include <vector>
#include "VECTOR.h"
class Game
{
public:
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	// UI
	const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
	void PushUI(class UIScreen* uiScreen);
	enum GameState
	{
		EGameplay,
		EPaused,
		EQuit
	};
	void SetState(GameState gameState) { mGameState = gameState; }

	class Renderer* GetRenderer() { return mRenderer; }
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	std::vector<class Actor*>mActors;

	// UI
	std::vector<class UIScreen*>mUIStack;
	GameState mGameState;

	// Update���t���b�O
	bool mUpdatingActors;
	std::vector<class Actor*> mPendingActors;

	//�o�͌n�͂��̃N���X�ŊǗ�
	class Renderer* mRenderer;
};
