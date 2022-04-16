#pragma once
#include<functional>
#include"COLOR.h"
#include"VECTOR2.h"
//ユーザーインターフェイス基底クラス
class UIScreen
{
public:
	UIScreen(class Game* game);
	virtual ~UIScreen();
	// 派生クラスでオーバーライド可能
	virtual void ProcessInput();
	virtual void Update(){};
	virtual void Draw();
	// UIステート。アクティブか、閉じるか
	enum UIState
	{
		EActive,
		EClosing
	};
	// UIステートをClosingに設定
	void CloseMe();
	// UIステートを取得
	UIState GetState() const { return mState; }
	// UIタイトルを設定
	void SetTitle(const char* text) { mTitle = text; }
	// ボタンを追加（縦に配置されていく）
	void AddButton(const char* name, std::function<void()> onClick);
protected:
	class Game* mGame;

	// ステート
	UIState mState;

	//背景画像
	int mBackImg;
	VECTOR2 mBackPos;

	//タイトル文字とその属性
	std::string mTitle;
	COLOR mTitleColor;
	VECTOR2 mTitlePos;
	float mTextSize;

	//ボタン
	std::vector<class Button*> mButtons;
	VECTOR2 mButtonPos;
};

