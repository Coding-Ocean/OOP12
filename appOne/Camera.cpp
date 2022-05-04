#include "Camera.h"
#include "Game.h"
#include "Renderer.h"
#include "COLLISION_MAP.h"
#include "input.h"

Camera::Camera(Game* game)
    :Actor(game)
    ,OffsetPosY(1.3f)
    ,DistanceFromLookatPos(5)
    ,CorrectPos(1.0f)//壁から少し離れた位置に持ってくる補正値。1.0で壁にひっつく。
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
    //現在のカメラ位置をとっておく
    VECTOR prePos = pos;
    //顔の辺りを注視点とする
    LookatPos = GetGame()->GetPlayer()->GetPosition();
    LookatPos.y += OffsetPosY;
    //カメラの位置を求める
    pos = LookatPos;
    pos.x += sinf(rotY) * cosf(rotX) * DistanceFromLookatPos;
    pos.y += sinf(rotX) * DistanceFromLookatPos;
    pos.z += cosf(rotY) * cosf(rotX) * DistanceFromLookatPos;
    //「注視点からカメラまでの線分」がマップのポリゴンに交差していたら、
    //カメラの位置（線分の終点）を交差点に変更する。
    GetGame()->GetCollisionMap()->segment_trianglesEco(LookatPos, &pos, CorrectPos);
    //少し時間差でスムーズにカメラの位置を変更
    pos = prePos + (pos - prePos) * ChangePosSpeed;
    //ビュー行列をつくる
	MATRIX view;
    view.camera(pos, LookatPos, UpVec);
	GetGame()->GetRenderer()->SetView(view);

    SetRotationY(rotY);
    SetRotationX(rotX);
    SetPosition(pos);
}
