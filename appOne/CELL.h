#pragma once
#include<vector>
#include"VECTOR.h"
class CELL{
public:
    //Axis Aligned Cube Boxの一辺の長さと頂点座標P[8]設定
    void setLen_Pos( float len, float x, float y, float z );
    //三角ポリゴンがAABに触れていたらこのCELLに登録
    void regist(class TRIANGLE& t );
    //カプセルと壁、天井との当たり判定処理
    int capsule_triangles( VECTOR* pos, float* dirY, int* jumpFlag, 
        //float speed, 
        float radius, float height, float degree, int* step ) const;
    //カプセルと床ポリゴンとの当たり判定処理
    int capsule_floor_triangles( VECTOR* pos, float* dirY, int* jumpFlag, float speed, float radius, float height, float degree, int step ) const;
    //
    int segment_triangles( VECTOR& sp, VECTOR* ep, float correct );
    int select( const VECTOR& sp, const VECTOR& ep );
    //Getter
    VECTOR pos(){ return VECTOR( X, Y, Z ); }
    std::vector<TRIANGLE*>& triangles(){ return Triangles; }
private:
    //オブジェクトより下にある最も近い床の座標を得る
    int GetNearestFloorY(
        const VECTOR* pos,
        float offsetY, //落下速度が速すぎて、床を突き抜ける場合の調整値 
        float ny,
        float* tanTheta,
        float* floorY
    )const;

    //Axis Aligned Cube Box----------------------------------------------------
    //cubeの一辺の長さ
    float Len = 0;
    //左奥下の座標
    float X=0, Z=0, Y=0;
    //頂点位置 posision
    VECTOR P[ 8 ];
    //４角ポリゴンの法線
    VECTOR N[ 6 ] = {
        VECTOR( 0, 0, 1 ),
        VECTOR( 0, 0,-1 ),
        VECTOR( 1, 0, 0 ),
        VECTOR(-1, 0, 0 ),
        VECTOR( 0, 1, 0 ),
        VECTOR( 0,-1, 0 ),
    };
    //４角ポリゴンのインデックス
    int Idx[ 6 ][ 4 ] = {
        0,1,2,3,
        4,5,6,7,
        1,5,3,7,
        0,4,2,6,
        4,5,0,1,
        6,7,2,3
    };
    //このCELLに属する三角ポリゴン---------------------------------------------------------
    std::vector<TRIANGLE*>Triangles;
    int NumTriangles = 0;
};