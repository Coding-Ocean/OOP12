#include "UIScreen.h"
#include "graphic.h"
#include "input.h"
#include "Game.h"
#include "UIButton.h"

UIScreen::UIScreen(Game* game)
	:mGame(game)
	, mState(EActive)
	, mBackImg(-1)
	, mTextSize(40)
	, mTitlePos(width / 2, 100.0f)
	, mButtonPos(width / 2, 170.0f)
{
	// GameのUIStackに追加
	mGame->PushUI(this);
	// ボタン画像
	Button::SetButtonOnImg(loadImage("Assets/ButtonYellow.png"));
	Button::SetButtonOffImg(loadImage("Assets/ButtonBlue.png"));
}

UIScreen::~UIScreen()
{
	for (auto b : mButtons)
	{
		delete b;
	}
	mButtons.clear();
}

void UIScreen::ProcessInput()
{
	// ボタンがあるなら
	if (!mButtons.empty())
	{
		// マウスの位置を取得
		VECTOR2 mousePos(mouseX, mouseY);
		// 全ボタンのチェック
		for (auto b : mButtons)
		{
			// ロールオーバーしていたら
			if (b->ContainsPoint(mousePos))
			{
				if (isTrigger(MOUSE_LBUTTON))
				{
					b->OnClick();
				}
			}
		}
	}
}

void UIScreen::Draw()
{
	rectMode(CENTER);
	// 背景画像表示（もしあったら）
	if (mBackImg >= 0)
	{
		image(mBackImg, mBackPos.x, mBackPos.y);
	}
	
	// タイトル表示（もしあったら）
	if (mTitle.c_str())
	{
		textSize(mTextSize);
		fill(mTitleColor);
		text(mTitle.c_str(),
			mTitlePos.x - mTitle.length() * mTextSize / 4,//半角文字のみ対応
			mTitlePos.y + mTextSize / 2
		);
	}

	// ボタン表示（もしあったら）
	for (auto b : mButtons)
	{
		b->Draw();
	}
}

void UIScreen::CloseMe()
{
	mState = EClosing;
}

//ボタンは縦に並んで配置される
void UIScreen::AddButton(const char* name, std::function<void()> onClick)
{
	Button* b = new Button(name, onClick, mButtonPos);
	mButtons.emplace_back(b);

	// 次に追加されるボタンの位置を計算しておく（画像の高さ＋間隔）
	mButtonPos.y += b->GetDimensions().y + 20.0f;
}
