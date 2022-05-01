#include "Player.h"
#include "PlayerStates.h"
#include "InputComponent.h"
#include "StateComponent.h"
#include "TreeMeshComponent.h"
#include "Game.h"
#include "COLLISION_MAP.h"
#include "window.h"

Player::Player(Game* game)
	:Actor(game)
{
	//�ʒu�E��]�E�X�P�[��
	SetPosition(VECTOR(0,0,0));

	//�ǉ�����
	mForwardSpeed = 0.05f;
	mRotateRatio = 0.2f;
	mJumpVelocity = 0;
	mGravity = -36.0f;

	//���̓R���|�[�l���g
	mIn = new InputComponent(this);

	//Actor��Ԑ؂�ւ��R���|�[�l���g
	mState = new StateComponent(this);
	mState->RegisterState(new PlayerWait(mState));
	mState->RegisterState(new PlayerWalk(mState));
	mState->RegisterState(new PlayerJump(mState));
	
	//���b�V���R���|�[�l���g
	mMesh = new TreeMeshComponent(this);
	mMesh->SetTree("unitychan");
	mMesh->SetAnim("unitychanWait00");
	mMesh->SetAnim("unitychanWait02");
	mMesh->SetAnim("unitychanRun");
	mMesh->SetAnim("unitychanJump00");

	//�ŏ��̏�Ԃ�ݒ�
	mState->ChangeState("Wait");
}

void Player::UpdateActor()
{
	VECTOR pos = GetPosition();
	float radius = 0.3f;//�J�v�Z�����a
	float height = 1.6f;//�J�v�Z������
	float keisya = 44.0f;//���̊p�x�ȏ�̃|���S�����ǂɂȂ�
	GetGame()->GetCollisionMap()->capsule_triangles(
		&pos, &mJumpVelocity, &mJumpFlag, mForwardSpeed,
		radius, height, keisya
	);
	//�󒆂ɂ��鎞�̏����͂��������ł���Ă܂�
	if (mJumpFlag) {
		pos.y += mJumpVelocity * delta;
		mJumpVelocity += mGravity * delta;
	}
	SetPosition(pos);
}

