#include "UIQuit.h"
#include "Game.h"
#include "graphic.h"

UIQuit::UIQuit(Game* game)
	:UIScreen(game)
{
	//各パーツ位置調整
	mBackPos.set(width / 2, 350.0f);
	mTitlePos.set(width / 2, mBackPos.y - 100);
	mButtonPos.set(width/2, mTitlePos.y + 70);
	
	mBackImg = loadImage("Assets/DialogBG.png");

	mTitle = "Quit";
	mTitleColor = COLOR(0, 0, 0);
	mTextSize = 40;
	
	AddButton("OK",
		[this]() {
			mGame->SetState(Game::EQuit);
		}
	);
	
	AddButton("Cancel", 
		[this]() {
			CloseMe();
		}
	);
}
