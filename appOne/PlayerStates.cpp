#include "PlayerStates.h"
#include "Player.h"
#include "VECTOR2.h"
#include "window.h"
#include "InputComponent.h"
#include "StateComponent.h"
#include "TreeMeshComponent.h"

//Actor < StateComponent < States

//-------------------------------------------------------------------
void PlayerWait::OnEnter()
{
	Player* p = dynamic_cast<Player*>(mOwnerCompo->GetActor());
	p->GetMesh()->SetNextAnimId(p->EWait,10);
}
void PlayerWait::Update()
{
	Player* p = dynamic_cast<Player*>(mOwnerCompo->GetActor());

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

//walk,jumpで共有
void move(Player* p)
{
	//指定方向へ移動
	VECTOR mDir = VECTOR(0, 0, 0);
	if (p->GetIn()->Left())mDir.x = -1;
	if (p->GetIn()->Right())mDir.x = 1;
	if (p->GetIn()->Up())mDir.z = -1;
	if (p->GetIn()->Down())mDir.z = 1;
	VECTOR pos = p->GetPosition();
	p->SetPosition(pos + mDir.normalize() * p->GetForwardSpeed());
	//指定方向へゆるりと回転
	float angle = p->GetRotationY();
	VECTOR2 a(sinf(angle), cosf(angle));//現在向いている方向a
	VECTOR2 b(mDir.x, mDir.z);//これから向く方向b
	angle += -atan2f(cross(a, b), dot(a, b)) * p->GetRotateSpeed();
	p->SetRotationY(angle);
}

//-------------------------------------------------------------------
void PlayerWalk::OnEnter()
{
	Player* p = dynamic_cast<Player*>(mOwnerCompo->GetActor());
	p->GetMesh()->SetNextAnimId(p->EWalk,10);
}
void PlayerWalk::Update()
{
	Player* p = dynamic_cast<Player*>(mOwnerCompo->GetActor());
	move(p);
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
	Player* p = dynamic_cast<Player*>(mOwnerCompo->GetActor());
	p->GetMesh()->SetNextAnimId(p->EWait,10);
	mVel = 0.2f * 60;
	mAcc = -0.01f * 60 * 60;
}
void PlayerJump::Update()
{
	Player* p = dynamic_cast<Player*>(mOwnerCompo->GetActor());
	
	move(p);

	//ジャンプ
	VECTOR pos = p->GetPosition();
	pos.y += mVel * delta;
	mVel += mAcc * delta;
	//着地
	float landingY = 0.0f;
	if (pos.y < landingY)
	{
		pos.y = landingY;
		mOwnerCompo->ChangeState("Wait");
	}
	p->SetPosition(pos);
}