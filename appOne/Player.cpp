#include "Player.h"
#include "PlayerStates.h"
#include "InputComponent.h"
#include "StateComponent.h"
#include "TreeMeshComponent.h"

Player::Player(Game* game)
	:Actor(game)
{
	//位置・回転・スケール
	SetPosition(VECTOR(0,0,0));

	//追加属性
	mForwardSpeed = 0.04f;
	mRotateRatio = 0.2f;
	mJumpVelocity = 0.2f * 60;
	mGravity = -0.6f * 60;

	//入力コンポーネント
	mIn = new InputComponent(this);

	//Actor状態切り替えコンポーネント
	mState = new StateComponent(this);
	mState->RegisterState(new PlayerWait(mState));
	mState->RegisterState(new PlayerWalk(mState));
	mState->RegisterState(new PlayerJump(mState));
	
	//メッシュコンポーネント
	mMesh = new TreeMeshComponent(this);
	mMesh->SetTree("explorer");
	mMesh->SetAnim("explorer_neutral");
	mMesh->SetAnim("explorer_run");

	//最初の状態を設定
	mState->ChangeState("Wait");
}
