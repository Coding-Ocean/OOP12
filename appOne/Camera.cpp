#include "Camera.h"
#include "Game.h"
#include "Renderer.h"
#include "COLLISION_MAP.h"
#include "input.h"

Camera::Camera(Game* game)
    :Actor(game)
    ,OffsetPosY(1.3f)
    ,DistanceFromLookatPos(5)
    ,CorrectPos(1.0f)//�ǂ��班�����ꂽ�ʒu�Ɏ����Ă���␳�l�B1.0�ŕǂɂЂ����B
    ,ChangePosSpeed(0.1f)
    ,RotSpeed(0.02f)
    ,ZoomSpeed(0.02f)
    ,UpVec(0,1,0)
{
    SetPosition(GetGame()->GetPlayer()->GetPosition());
    SetRotationX(0.2f);
}

void Camera::UpdateActor()
{
    float rotY = GetRotationY();
    float rotX = GetRotationX();
    if (isPress(KEY_L)) { rotY += RotSpeed; }
    if (isPress(KEY_J)) { rotY -= RotSpeed; }
    if (isPress(KEY_I)) { rotX += RotSpeed; }
    if (isPress(KEY_K)) { rotX -= RotSpeed; }
    if (isPress(KEY_U)) { DistanceFromLookatPos -= ZoomSpeed; }
    if (isPress(KEY_O)) { DistanceFromLookatPos += ZoomSpeed; }
    if (rotX > 1.57f) { rotX = 1.57f; }
    if (rotX < -0.7f) { rotX = -0.7f; }
    if (DistanceFromLookatPos < 1.5f) { DistanceFromLookatPos = 1.5f; }

    VECTOR pos = GetPosition();
    //���݂̃J�����ʒu���Ƃ��Ă���
    VECTOR prePos = pos;
    //��̕ӂ�𒍎��_�Ƃ���
    LookatPos = GetGame()->GetPlayer()->GetPosition();
    LookatPos.y += OffsetPosY;
    //�J�����̈ʒu�����߂�
    pos = LookatPos;
    pos.x += sinf(rotY) * cosf(rotX) * DistanceFromLookatPos;
    pos.y += sinf(rotX) * DistanceFromLookatPos;
    pos.z += cosf(rotY) * cosf(rotX) * DistanceFromLookatPos;
    //�u�����_����J�����܂ł̐����v���}�b�v�̃|���S���Ɍ������Ă�����A
    //�J�����̈ʒu�i�����̏I�_�j�������_�ɕύX����B
    GetGame()->GetCollisionMap()->segment_trianglesEco(LookatPos, &pos, CorrectPos);
    //�������ԍ��ŃX���[�Y�ɃJ�����̈ʒu��ύX
    pos = prePos + (pos - prePos) * ChangePosSpeed;
    //�r���[�s�������
	MATRIX view;
    view.camera(pos, LookatPos, UpVec);
	GetGame()->GetRenderer()->SetView(view);

    SetRotationY(rotY);
    SetRotationX(rotX);
    SetPosition(pos);
}
