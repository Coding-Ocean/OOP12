#include "PlayerStates.h"
#include "Player.h"
#include "Game.h"
#include "VECTOR2.h"
#include "InputComponent.h"
#include "StateComponent.h"
#include "TreeMeshComponent.h"
#include "window.h"

//PlayerWalk,PlayerJumpで共有する関数
void jumping(Player*p) {
	VECTOR pos = p->GetPosition();
	if (p->GetJumpFlag()) {
		float jumpVelo = p->GetJumpVelocity();
		pos.y +=  jumpVelo * delta;
		p->SetJumpVelocity(jumpVelo + p->GetGravity() * delta);
	}

	p->SetPosition(pos);
}
void move(Player* p)
{

	//指定方向へ移動
	VECTOR dir = VECTOR(0, 0, 0);
	if (p->GetIn()->Left())dir.x = -1;
	if (p->GetIn()->Right())dir.x = 1;
	if (p->GetIn()->Up())dir.z = -1;
	if (p->GetIn()->Down())dir.z = 1;
	if (dir.x == 0 && dir.z == 0)return;

	float rad = p->GetGame()->GetCameraRotationY();
	MATRIX m;
	m.identity();
	m.mulRotateY(rad);
	dir = m * dir;
	VECTOR pos = p->GetPosition();
	p->SetPosition(pos + dir.normalize() * p->GetForwardSpeed());
	//指定方向へゆるりと回転
	float angle = p->GetRotationY();
	VECTOR2 a(sinf(angle), cosf(angle));//現在向いている方向a
	VECTOR2 b(dir.x, dir.z);//これから向く方向b
	angle += -atan2f(cross(a, b), dot(a, b)) * p->GetRotateRatio();
	p->SetRotationY(angle);
}

/*
以下のStateを継承したクラス群は、StateComponentで管理される
*/ 

//-------------------------------------------------------------------
void PlayerWait::OnEnter()
{
	Player* p = static_cast<Player*>(mOwnerCompo->GetActor());
	p->GetMesh()->SetNextAnimId(p->EWait,10);
	mElapsedTime = timeGetTime();
	mSwitch = 0;
}
void PlayerWait::Update()
{
	Player* p = static_cast<Player*>(mOwnerCompo->GetActor());
	jumping(p);
	//一定間隔でアニメーション切り替え
	if (timeGetTime() - mElapsedTime > 8000)
	{
		mElapsedTime = timeGetTime();
		mSwitch = 1 - mSwitch;
		if(mSwitch)
			p->GetMesh()->SetNextAnimId(p->EWait2, 60);
		else
			p->GetMesh()->SetNextAnimId(p->EWait, 10);
	}

	//キー入力でステート切り替え
	if (p->GetIn()->StartWalk())
	{
		mOwnerCompo->ChangeState("Walk");
		return;
	}
	if (p->GetIn()->Jump())
	{
		mOwnerCompo->ChangeState("Jump");
		return;
	}
}

//-------------------------------------------------------------------
void PlayerWalk::OnEnter()
{
	Player* p = static_cast<Player*>(mOwnerCompo->GetActor());
	p->GetMesh()->SetNextAnimId(p->EWalk,10);
}
void PlayerWalk::Update()
{
	Player* p = static_cast<Player*>(mOwnerCompo->GetActor());
	
	move(p);
	jumping(p);

	//ステート変更
	if (p->GetIn()->StopWalk())
	{
		mOwnerCompo->ChangeState("Wait");
		return;
	}
	if (p->GetIn()->Jump())
	{
		mOwnerCompo->ChangeState("Jump");
		return;
	}
}

//-------------------------------------------------------------------
void PlayerJump::OnEnter()
{
	Player* p = static_cast<Player*>(mOwnerCompo->GetActor());
	p->GetMesh()->SetNextAnimId(p->EJump, 20, 30);

	p->SetJumpFlag();
	p->SetJumpVelocity(10.0f);

	//ここでちょっと浮かせないとジャンプフラッグが倒れます
	VECTOR pos = p->GetPosition();
	pos.y += p->GetJumpVelocity() * delta;
	p->SetPosition(pos);

	mSecondJump = 0;
}
void PlayerJump::Update()
{
	Player* p = static_cast<Player*>(mOwnerCompo->GetActor());

	move(p);
	jumping(p);

	//２段ジャンプ
	if (mSecondJump == 0 && p->GetIn()->Jump()) {
		mSecondJump = 1;
		p->SetJumpVelocity(10.0f);
		p->GetMesh()->SetNextAnimId(p->EJump, 4, 14);
	}

	if (p->GetJumpFlag()==0) {
		mOwnerCompo->ChangeState("Wait");
		return;
	}
}