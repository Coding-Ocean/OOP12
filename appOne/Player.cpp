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
	mForwardSpeed = 0.04f;
	mRotateSpeed = 0.2f;

	//入力コンポーネント
	mIn = new InputComponent(this);

	//メッシュコンポーネント
	mMesh = new TreeMeshComponent(this);
	mMesh->SetTree("explorer");
	mMesh->SetAnim("explorer_neutral");
	mMesh->SetAnim("explorer_run");
	mMesh->SetAnimId(EWait);

	//Actor状態切り替えコンポーネント
	mState = new StateComponent(this);
	mState->RegisterState(new PlayerWait(mState));
	mState->RegisterState(new PlayerWalk(mState));
	mState->RegisterState(new PlayerJump(mState));
	mState->ChangeState("Wait");
}
