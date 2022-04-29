#pragma once
#include<vector>
#include"VECTOR.h"
class CELL{
public:
    //Axis Aligned Cube Box�̈�ӂ̒����ƒ��_���WP[8]�ݒ�
    void setLen_Pos( float len, float x, float y, float z );
    //�O�p�|���S����AAB�ɐG��Ă����炱��CELL�ɓo�^
    void regist(class TRIANGLE& t );
    //�J�v�Z���ƕǁA�V��Ƃ̓����蔻�菈��
    int capsule_triangles( VECTOR* pos, float* dirY, int* jumpFlag, 
        //float speed, 
        float radius, float height, float degree, int* step ) const;
    //�J�v�Z���Ə��|���S���Ƃ̓����蔻�菈��
    int capsule_floor_triangles( VECTOR* pos, float* dirY, int* jumpFlag, float speed, float radius, float height, float degree, int step ) const;
    //
    int segment_triangles( VECTOR& sp, VECTOR* ep, float correct );
    int select( const VECTOR& sp, const VECTOR& ep );
    //Getter
    VECTOR pos(){ return VECTOR( X, Y, Z ); }
    std::vector<TRIANGLE*>& triangles(){ return Triangles; }
private:
    //�I�u�W�F�N�g��艺�ɂ���ł��߂����̍��W�𓾂�
    int GetNearestFloorY(
        const VECTOR* pos,
        float offsetY, //�������x���������āA����˂�������ꍇ�̒����l 
        float ny,
        float* tanTheta,
        float* floorY
    )const;

    //Axis Aligned Cube Box----------------------------------------------------
    //cube�̈�ӂ̒���
    float Len = 0;
    //�������̍��W
    float X=0, Z=0, Y=0;
    //���_�ʒu posision
    VECTOR P[ 8 ];
    //�S�p�|���S���̖@��
    VECTOR N[ 6 ] = {
        VECTOR( 0, 0, 1 ),
        VECTOR( 0, 0,-1 ),
        VECTOR( 1, 0, 0 ),
        VECTOR(-1, 0, 0 ),
        VECTOR( 0, 1, 0 ),
        VECTOR( 0,-1, 0 ),
    };
    //�S�p�|���S���̃C���f�b�N�X
    int Idx[ 6 ][ 4 ] = {
        0,1,2,3,
        4,5,6,7,
        1,5,3,7,
        0,4,2,6,
        4,5,0,1,
        6,7,2,3
    };
    //����CELL�ɑ�����O�p�|���S��---------------------------------------------------------
    std::vector<TRIANGLE*>Triangles;
    int NumTriangles = 0;
};