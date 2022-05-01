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
	Player* p = static_cast<Player*>(mOwnerCompo->GetActor());
	p->GetMesh()->SetNextAnimId(p->EWait,10);
	mElapsedTime = timeGetTime();
	mSwitch = 0;
}
void PlayerWait::Update()
{
	Player* p = static_cast<Player*>(mOwnerCompo->GetActor());

	//���Ԋu�ŃA�j���[�V�����؂�ւ�
	if (timeGetTime() - mElapsedTime > 8000)
	{
		mElapsedTime = timeGetTime();
		mSwitch = 1 - mSwitch;
		if(mSwitch)
			p->GetMesh()->SetNextAnimId(p->EWait2, 60);
		else
			p->GetMesh()->SetNextAnimId(p->EWait, 10);
	}

	//�L�[���͂ŃX�e�[�g�؂�ւ�
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
	Player* p = static_cast<Player*>(mOwnerCompo->GetActor());
	p->GetMesh()->SetNextAnimId(p->EJump, 20, 30);

	p->SetJumpFlag();
	p->SetJumpVelocity(10.0f);

	//�����ł�����ƕ������Ȃ��ƃW�����v�t���b�O���|��܂�
	VECTOR pos = p->GetPosition();
	pos.y += p->GetJumpVelocity() * delta;
	p->SetPosition(pos);

	mSecondJump = 0;
}
void PlayerJump::Update()
{
	Player* p = static_cast<Player*>(mOwnerCompo->GetActor());

	move(p);

	//�Q�i�W�����v
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