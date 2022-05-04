#pragma once
#include <vector>
#include "Actor.h"
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
	class COLLISION_MAP* GetCollisionMap() { return mCollisionMap; }
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	std::vector<class Actor*>mActors;

	// UI
	std::vector<class UIScreen*>mUIStack;
	GameState mGameState;

	// Update中フラッグ
	bool mUpdatingActors;
	std::vector<class Actor*> mPendingActors;

	//出力系管理クラス
	class Renderer* mRenderer;

	class COLLISION_MAP* mCollisionMap;

	//このゲームに固有のロジック
	void LoadData();
public:
	class Actor* GetPlayer() { return mPlayer; }
	float GetCameraRotationY() { return mCamera->GetRotationY(); }
private:
	class Actor* mPlayer;
	class Actor* mCamera;
};

