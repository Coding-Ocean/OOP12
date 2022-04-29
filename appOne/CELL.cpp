#include"COLOR.h"
#include"TRIANGLE.h"
#include"CELL.h"
#include"mathUtil.h"

void CELL::setLen_Pos( float len, float x, float y, float z ) 
{
    //引数のxは左、ｙは下、ｚは奥の座標
    Len = len;  X = x; Y = y; Z = z;

    //右、上、手前の座標を求める
    float r = x + len;//right右
    float t = y + len;//top上
    float fr = z + len;//front手前

    P[ 0 ] = VECTOR( x, t, fr );//左上手前
    P[ 1 ] = VECTOR( r, t, fr );//右上手前
    P[ 2 ] = VECTOR( x, y, fr );//左下手前
    P[ 3 ] = VECTOR( r, y, fr );//右下手前

    P[ 4 ] = VECTOR( x, t, z );//左上奥
    P[ 5 ] = VECTOR( r, t, z );//右上奥
    P[ 6 ] = VECTOR( x, y, z );//左下奥
    P[ 7 ] = VECTOR( r, y, z );//右下奥
}
void CELL::regist(class TRIANGLE& t )
{
    //TRIANGLEの３頂点
    VECTOR* tp = t.p();

    //三角形の全頂点がAABBの各面より上下右左前後に存在する＝当たっていない
    if(
        tp[ 0 ].y < P[ 2 ].y && tp[ 1 ].y < P[ 2 ].y && tp[ 2 ].y < P[ 2 ].y ||
        tp[ 0 ].y > P[ 0 ].y && tp[ 1 ].y > P[ 0 ].y && tp[ 2 ].y > P[ 0 ].y ||
        tp[ 0 ].x < P[ 0 ].x && tp[ 1 ].x < P[ 0 ].x && tp[ 2 ].x < P[ 0 ].x ||
        tp[ 0 ].x > P[ 1 ].x && tp[ 1 ].x > P[ 1 ].x && tp[ 2 ].x > P[ 1 ].x ||
        tp[ 0 ].z < P[ 4 ].z && tp[ 1 ].z < P[ 4 ].z && tp[ 2 ].z < P[ 4 ].z ||
        tp[ 0 ].z > P[ 0 ].z && tp[ 1 ].z > P[ 0 ].z && tp[ 2 ].z > P[ 0 ].z )
    {
        return;
    }

    int hitSw = 0;

    //「三角形のいずれかの頂点」が「AAB」の中に入っているか
    for( int i = 0; hitSw == 0 && i < 3; i++ ){
        if( P[ 0 ].x <= tp[ i ].x && tp[ i ].x <= P[ 1 ].x &&
            P[ 2 ].y <= tp[ i ].y && tp[ i ].y <= P[ 0 ].y &&
            P[ 4 ].z <= tp[ i ].z && tp[ i ].z <= P[ 0 ].z )
        {
            hitSw = 1;
        }
    }
    
    //「三角形のいずれかの辺」が「AABのいずれかの面」に交差しているか
    if( hitSw == 0 )
    {
        //三角形の辺の始点sp 終点ep
        VECTOR sp;
        VECTOR ep;
        for( int i = 0; hitSw == 0 && i < 3; i++ ){
            sp = tp[ i ];
            ep = tp[ (i + 1) % 3 ];
            for( int j = 0; hitSw == 0 && j < 6; j++ ){
                VECTOR v1, v2;
                float d1, d2;
                v1 = sp - P[ Idx[ j ][ 0 ] ];
                v2 = ep - P[ Idx[ j ][ 0 ] ];
                d1 = dot( v1, N[ j ] );
                d2 = dot( v2, N[ j ] );
                if( d1 * d2 <= 0.0f ){
                    //貫通している点の座標hpを求める
                    d1 = absolute( d1 );
                    d2 = absolute( d2 );
                    float a = d1 / (d1 + d2);//貫通点までの内分比
                    VECTOR v3 = v1 * (1.0f - a) + v2 * a;
                    VECTOR hp = P[ Idx[ j ][ 0 ] ] + v3;//ヒットポイント
                    //hpが４角ポリゴンの範囲内か判定
                    if( N[ j ].z != 0.0f &&
                        P[ Idx[ j ][ 0 ] ].x <= hp.x && hp.x <= P[ Idx[ j ][ 1 ] ].x &&
                        P[ Idx[ j ][ 2 ] ].y <= hp.y && hp.y <= P[ Idx[ j ][ 0 ] ].y )
                    {
                        hitSw = 2;
                    }
                    if( N[ j ].y != 0.0f &&
                        P[ Idx[ j ][ 0 ] ].x <= hp.x && hp.x <= P[ Idx[ j ][ 1 ] ].x &&
                        P[ Idx[ j ][ 0 ] ].z <= hp.z && hp.z <= P[ Idx[ j ][ 2 ] ].z )
                    {
                        hitSw = 2;
                    }
                    if( N[ j ].x != 0.0f &&
                        P[ Idx[ j ][ 2 ] ].y <= hp.y && hp.y <= P[ Idx[ j ][ 0 ] ].y &&
                        P[ Idx[ j ][ 1 ] ].z <= hp.z && hp.z <= P[ Idx[ j ][ 0 ] ].z )
                    {
                        hitSw = 2;
                    }
                }
            }
        }
    }
    
    //「AABの対角線のいずれか」が「三角形」と交差しているか
    if( hitSw == 0 )
    {
        for( int j = 0; hitSw == 0 && j < 4; j++ ){
            VECTOR ep = P[ 7 - j ];
            if( t.intersection( P[ j ], &ep ) ){
                hitSw = 3;
            }
        }
    }

    //登録
    if( hitSw ){
        Triangles.push_back( &t );
        NumTriangles++;
    }
}

//「カプセル」と「壁または天井」との当たり判定
int CELL::capsule_triangles( VECTOR* pos, float* dirY, int* jumpFlag, 
    //float speed, 
    float radius, float height, float degree, int* step ) const 
{
    //ポリゴンの法線ベクトルのY座標がこのnyより小さいものを壁とする
    float ny = cosf( 3.141592f * degree / 180.0f );
    //カプセルが壁と天井のポリゴンに食い込んだ分だけposを追い出す
    int cnt = 0;
    for( int i = 0; cnt < 2 && i < NumTriangles; i++ ){
        if( Triangles[ i ]->wallAndCeilingPolygon( pos, dirY, *jumpFlag, radius, height, ny ) ){
            cnt++;
        }
    }
    if( cnt )return cnt;
    //カプセルが壁と天井の線分に食い込んだ分だけposを追い出す
    for( int i = 0; cnt < 3 && i < NumTriangles; i++ ){
        if( Triangles[ i ]->wallAndCeilingSegment( pos, dirY, *jumpFlag, radius, height, ny ) ){
            cnt++;
            *step = 1;
        }
    }
    return cnt;
}
//「カプセル」と「床」との当たり判定
int CELL::capsule_floor_triangles( VECTOR* pos, float* dirY, int* jumpFlag, float speed, 
    float radius, float height, float degree, int step ) const 
{
    //ポリゴンの法線ベクトルのY座標がこのnyより小さいものを壁とする
    float ny = cosf( 3.141592f * degree / 180.0f );
    //床に立たせる。床でなければ落下
    float floorY = -100.0f;//床の最低値以下を入れておく
    float tanTheta = 0.0f;//坂道を下るときジャンプにならない落差を求めるために使う
    GetNearestFloorY( pos, 1.0f, ny, &tanTheta, &floorY );
    if( *jumpFlag == 0 ){
        float diff = pos->y - floorY;
        if( step && 0.0f < diff && diff < radius / 1.0f ){//階段を上らせる
            ;
        }
        else if(  diff <= speed * tanTheta ){//speed*tanThetaで坂道を下るときの最大落差が求められる
            pos->y = floorY;
        }
        else{//落下
            *jumpFlag = 1;
            *dirY = 0;
        }
    }
    else{
        if( pos->y <= floorY ){//着地
            pos->y = floorY;
            *jumpFlag = 0;
        }
    }

    return 0;
}
//オブジェクトより低くて、もっとも近い床のＹ座標を取得
int CELL::GetNearestFloorY(
    const VECTOR* pos, 
    float offsetY, //落下速度が速すぎて、床を突き抜ける場合の調整値 
    float ny,
    float* tanTheta,
    float* floorY
)const
{
    int flag = 0;
    for( int i = 0; i < NumTriangles; i++ ){
        if( Triangles[ i ]->insideMe( *pos, ny ) ){
            flag = 1;
            float tempY = Triangles[ i ]->getY( *pos );
            if( pos->y + offsetY >= tempY ){
                if( *floorY < tempY ){
                    *floorY = tempY;
                    *tanTheta = Triangles[ i ]->tanTheta();
                }
            }
        }
    }
    return flag;
}

//カメラの位置をオブジェクトの前に移動
int CELL::segment_triangles( VECTOR& sp, VECTOR* ep, float correct )
{
    int cnt = 0;
    size_t num = Triangles.size();
    for( size_t i = 0; cnt < 1 && i < num; i++ ){
        if( Triangles[ i ]->intersection( sp, ep, correct ) ){
            cnt++;
        }
    }
    return cnt;
}

int CELL::select( const VECTOR& sp, const VECTOR& ep )
{
    //始点と終点ともAABBの各面より上下右左前後に存在する＝当たっていない
    if(
        sp.y < P[ 2 ].y && ep.y < P[ 2 ].y ||
        sp.y > P[ 0 ].y && ep.y > P[ 0 ].y ||
        sp.x < P[ 0 ].x && ep.x < P[ 0 ].x ||
        sp.x > P[ 1 ].x && ep.x > P[ 1 ].x ||
        sp.z < P[ 4 ].z && ep.z < P[ 4 ].z ||
        sp.z > P[ 0 ].z && ep.z > P[ 0 ].z )
    {
        return 0;
    }

    int hitSw = 0;
    if( P[ 0 ].x <= sp.x && sp.x <= P[ 1 ].x &&
        P[ 2 ].y <= sp.y && sp.y <= P[ 0 ].y &&
        P[ 4 ].z <= sp.z && sp.z <= P[ 0 ].z )
    {
        hitSw=1;
    }

    if( hitSw == 0 ){
        if( P[ 0 ].x <= ep.x && ep.x <= P[ 1 ].x &&
            P[ 2 ].y <= ep.y && ep.y <= P[ 0 ].y &&
            P[ 4 ].z <= ep.z && ep.z <= P[ 0 ].z )
        {
            hitSw = 1;
        }
    }

    //「線分」が「AABBのいずれかの面」に交差しているか
    if( hitSw == 0 )
    {
        for( int j = 0; hitSw == 0 && j < 6; j++ ){
            VECTOR v1, v2;
            float d1, d2;
            v1 = sp - P[ Idx[ j ][ 0 ] ];
            v2 = ep - P[ Idx[ j ][ 0 ] ];
            d1 = dot( v1, N[ j ] );
            d2 = dot( v2, N[ j ] );
            if( d1 * d2 <= 0.0f ){
                //貫通している点の座標hpを求める
                d1 = absolute( d1 );
                d2 = absolute( d2 );
                float a = d1 / (d1 + d2);//貫通点までの内分比
                VECTOR v3 = v1 * (1.0f - a) + v2 * a;
                VECTOR hp = P[ Idx[ j ][ 0 ] ] + v3;//ヒットポイント
                //hpが４角ポリゴンの範囲内か判定
                if( N[ j ].z != 0.0f &&
                    P[ Idx[ j ][ 0 ] ].x <= hp.x && hp.x <= P[ Idx[ j ][ 1 ] ].x &&
                    P[ Idx[ j ][ 2 ] ].y <= hp.y && hp.y <= P[ Idx[ j ][ 0 ] ].y )
                {
                    hitSw = 2;
                }
                if( N[ j ].y != 0.0f &&
                    P[ Idx[ j ][ 0 ] ].x <= hp.x && hp.x <= P[ Idx[ j ][ 1 ] ].x &&
                    P[ Idx[ j ][ 0 ] ].z <= hp.z && hp.z <= P[ Idx[ j ][ 2 ] ].z )
                {
                    hitSw = 2;
                }
                if( N[ j ].x != 0.0f &&
                    P[ Idx[ j ][ 2 ] ].y <= hp.y && hp.y <= P[ Idx[ j ][ 0 ] ].y &&
                    P[ Idx[ j ][ 1 ] ].z <= hp.z && hp.z <= P[ Idx[ j ][ 0 ] ].z )
                {
                    hitSw = 2;
                }
            }
        }
    }
    return hitSw;
}
