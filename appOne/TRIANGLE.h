#include "VECTOR.h"

class TRIANGLE{
public:
    TRIANGLE();
    ~TRIANGLE();
    //３つの頂点から三角形データを作る
    void create( const VECTOR& p1, const VECTOR& p2, const VECTOR& p3 );
    //posのXZ座標が真上からみてこのポリゴン内にいるかどうか判定
    int insideMe( const VECTOR& pos, float ny ) const;
    //XZ座標からY座標を算出
    float getY( const VECTOR& pos ) const;
    //このポリゴンにカプセルが食い込んでいたら追い出す
    int wallAndCeilingPolygon( VECTOR* pos, float* dirY, int jumpFlag, float radius, float height, float ny ) const;
    //線分にカプセルが食い込んでいたら追い出す
    int wallAndCeilingSegment( VECTOR* pos, float* dirY, int jumpFlag, float radius, float height, float ny ) const;
    //このポリゴンに線分が交差していたら、線分の終点を交点にする
    int intersection( VECTOR& sp, VECTOR* ep, float correct=1.0f );
    //Getter
    float tanTheta(){ return TanTheta; }
    VECTOR* p(){ return P; }
private:
    float Cross( int i, const VECTOR& pos ) const;
    void SphereSegment( const VECTOR& p, float radius, 
        VECTOR* n, float* sq_distance, int* flag )const;
    void CylinderSegment( const VECTOR& p, float radius, float cylinderLen, 
        VECTOR* n, float* sq_distance, int* flag )const;
    
    VECTOR P[ 3 ];
    VECTOR V[ 3 ];
    float   VLen[ 3 ];
    //平面
    VECTOR N;
    float Nw=0;
    //辺のベクトルと法線との外積ベクトル
    VECTOR C[ 3 ];
    float TanTheta=0;
};
