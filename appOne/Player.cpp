#include "Player.h"
#include "PlayerStates.h"
#include "InputComponent.h"
#include "StateComponent.h"
#include "TreeMeshComponent.h"

Player::Player(Game* game)
	:Actor(game)
{
	//�ʒu�E��]�E�X�P�[��
	SetPosition(VECTOR(0,0,0));
	mForwardSpeed = 0.04f;
	mRotateSpeed = 0.2f;

	//���̓R���|�[�l���g
	mIn = new InputComponent(this);

	//���b�V���R���|�[�l���g
	mMesh = new TreeMeshComponent(this);
	mMesh->SetTree("explorer");
	mMesh->SetAnim("explorer_neutral");
	mMesh->SetAnim("explorer_run");
	mMesh->SetAnimId(EWait);

	//Actor��Ԑ؂�ւ��R���|�[�l���g
	mState = new StateComponent(this);
	mState->RegisterState(new PlayerWait(mState));
	mState->RegisterState(new PlayerWalk(mState));
	mState->RegisterState(new PlayerJump(mState));
	mState->ChangeState("Wait");
}
