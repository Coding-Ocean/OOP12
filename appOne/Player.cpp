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
	//�ʒu�E��]�E�X�P�[��
	InitPos();

	//�ǉ�����
	mForwardSpeed = 0.1f;
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

void Player::InitPos()
{
	//SetPosition(VECTOR(24.26766f,-19.339411f, -354.5625f));
	SetPosition(VECTOR(22.27f, -13.27f, -412.0f));
	//SetPosition(VECTOR(110.61f, -41.41f, -998.0f));

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
	SetPosition(pos);

	printSize(30);
	print((let)"player x:" + pos.x + " y:" + pos.y + " z:" + pos.z);
	
	if (pos.y < -45.0f) {
		InitPos();
	}

}


