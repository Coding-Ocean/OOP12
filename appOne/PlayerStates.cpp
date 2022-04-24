#include "PlayerStates.h"
#include "Player.h"
#include "VECTOR2.h"
#include "InputComponent.h"
#include "StateComponent.h"
#include "TreeMeshComponent.h"
#include "window.h"

//PlayerWalk,PlayerJump�ŋ��L����֐�
void move(Player* p)
{
	//�w������ֈړ�
	VECTOR mDir = VECTOR(0, 0, 0);
	if (p->GetIn()->Left())mDir.x = -1;
	if (p->GetIn()->Right())mDir.x = 1;
	if (p->GetIn()->Up())mDir.z = -1;
	if (p->GetIn()->Down())mDir.z = 1;
	VECTOR pos = p->GetPosition();
	p->SetPosition(pos + mDir.normalize() * p->GetForwardSpeed());
	//�w������ւ���Ɖ�]
	float angle = p->GetRotationY();
	VECTOR2 a(sinf(angle), cosf(angle));//���݌����Ă������a
	VECTOR2 b(mDir.x, mDir.z);//���ꂩ���������b
	angle += -atan2f(cross(a, b), dot(a, b)) * p->GetRotateRatio();
	p->SetRotationY(angle);
}

/*
�ȉ���State���p�������N���X�Q�́AStateComponent�ŊǗ������
*/ 

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
	
	//�X�e�[�g�ύX
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
	mJumpVelocity = p->GetJumpVelocity();
	mGravity = p->GetGravity();
}
void PlayerJump::Update()
{
	Player* p = dynamic_cast<Player*>(mOwnerCompo->GetActor());
	
	move(p);

	//�W�����v
	VECTOR pos = p->GetPosition();
	pos.y += mJumpVelocity * delta;
	mJumpVelocity += mGravity * delta;
	//���n
	float landingY = 0.0f;
	if (pos.y < landingY)
	{
		pos.y = landingY;
		mOwnerCompo->ChangeState("Wait");
	}
	p->SetPosition(pos);
}