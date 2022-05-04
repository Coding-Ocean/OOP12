#include "Player.h"
#include "PlayerStates.h"
#include "InputComponent.h"
#include "StateComponent.h"
#include "TreeMeshComponent.h"
#include "Game.h"
#include "COLLISION_MAP.h"
#include "window.h"
#include "graphic.h"

Player::Player(Game* game)
	:Actor(game)
{
	//位置・回転・スケール
	InitPos();

	//追加属性
	mForwardSpeed = 0.1f;
	mRotateRatio = 0.2f;
	mJumpVelocity = 0;
	mGravity = -36.0f;

	//入力コンポーネント
	mIn = new InputComponent(this);

	//Actor状態切り替えコンポーネント
	mState = new StateComponent(this);
	mState->RegisterState(new PlayerWait(mState));
	mState->RegisterState(new PlayerWalk(mState));
	mState->RegisterState(new PlayerJump(mState));
	
	//メッシュコンポーネント
	mMesh = new TreeMeshComponent(this);
	mMesh->SetTree("unitychan");
	mMesh->SetAnim("unitychanWait00");
	mMesh->SetAnim("unitychanWait02");
	mMesh->SetAnim("unitychanRun");
	mMesh->SetAnim("unitychanJump00");

	//最初の状態を設定
	mState->ChangeState("Wait");
}

void Player::InitPos()
{
	//SetPosition(VECTOR(24.26766f,-19.339411f, -354.5625f));
	SetPosition(VECTOR(22.27f, -13.27f, -412.0f));
	//SetPosition(VECTOR(110.61f, -41.41f, -998.0f));

}

void Player::UpdateActor()
{
	VECTOR pos = GetPosition();
	float radius = 0.3f;//カプセル半径
	float height = 1.6f;//カプセル高さ
	float keisya = 44.0f;//この角度以上のポリゴンが壁になる
	GetGame()->GetCollisionMap()->capsule_triangles(
		&pos, &mJumpVelocity, &mJumpFlag, mForwardSpeed,
		radius, height, keisya
	);
	SetPosition(pos);

	printSize(30);
	print((let)"player x:" + pos.x + " y:" + pos.y + " z:" + pos.z);
	
	if (pos.y < -45.0f) {
		InitPos();
	}

}


