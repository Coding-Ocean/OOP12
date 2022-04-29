#include "common.h"
#include "TRIANGLE.h"
#include "mathUtil.h"

TRIANGLE::TRIANGLE()
:VLen()
{
}
TRIANGLE::~TRIANGLE()
{
}

//３つの頂点から三角形データを作る
void TRIANGLE::create( const VECTOR& p1, const VECTOR& p2, const VECTOR& p3 ){
    //頂点
    P[ 0 ] = p1;
    P[ 1 ] = p2;
    P[ 2 ] = p3;
    //３辺のベクトル
    V[ 0 ] = p2 - p1;
    V[ 1 ] = p3 - p2;
    V[ 2 ] = p1 - p3;
    //法線ベクトル
    N = cross( V[ 0 ], V[ 1 ] );
    N = normalize( N );
    //
    for( int i = 0; i < 3; i++ ){
        //ベクトルの大きさ
        VLen[ i ] = V[ i ].mag();
        //正規化
        V[ i ] /= VLen[ i ];
        //辺のベクトルと法線との外積ベクトル
        C[ i ] = cross( V[ i ], N );
    }
    //平面４番目の要素
    Nw = -( N.x * p1.x + N.y * p1.y + N.z * p1.z );
    //坂を下るときにジャンプにならないような最大落差を求めるための準備
    TanTheta = tan( acos( N.y - 0.01f ) );//0.01は調整値。余計に傾かせておく。
}

//posのXZ座標が真上からみてこのポリゴン内にいるかどうか判定
int TRIANGLE::insideMe( const VECTOR& pos, float ny ) const{ 
    int flag = 0;
    if( N.y > ny ){
        if( Cross( 0, pos ) >= 0 ){
            if( Cross( 1, pos ) >= 0 ){
                if( Cross( 2, pos ) >= 0 ){
                    flag = 1;
                }
            }
        }
    }
    return flag;
}
float TRIANGLE::Cross( int i, const VECTOR& pos ) const{
    float bx = pos.x - P[ i ].x;
    float bz = pos.z - P[ i ].z;
    return V[ i ].z * bx  -  V[ i ].x * bz;
}
//posがいるべき高さYを返す
float TRIANGLE::getY( const VECTOR& pos ) const{
    return ( N.x * pos.x + N.z * pos.z + Nw ) / -N.y;
}

//このポリゴンにカプセルが食い込んでいたら追い出す-----------------------------
int TRIANGLE::wallAndCeilingPolygon( 
    VECTOR* pos, float* dirY, int jumpFlag, 
    float radius, float height, float ny 
) const{
    int flag = 0;
    if( N.y <= ny ){
        VECTOR p = *pos;
        if( N.y >= 0.0f ){//壁ポリゴン
            p.y += radius;//下半球の中心
        }
        else{//天井ポリゴン
            p.y += height - radius;//上半球の中心
        }
        //ポリゴンを含む平面までの距離
        VECTOR v;
        v = p - P[ 0 ];
        float distance = dot( N, v );
        if( 0.0f <= distance  && distance < radius ){
            //ポリゴンの法線方向にpが含まれているかチェック
            if( dot( v, C[ 0 ] ) <= 0.0f ){
                v = p - P[ 1 ];
                if( dot( v, C[ 1 ] ) <= 0.0f ){
                    v = p - P[ 2 ];
                    if( dot( v, C[ 2 ] ) <= 0.0f ){
                        //ポリゴンに食い込んでいる-----------------------------------
                        flag = 1;
                        float d = radius - distance;
                        VECTOR n = N * d;
                        if( N.y >= 0.0f ){//壁
                            if( !jumpFlag ){
                                n.y = 0.0f;//斜めになっている壁を登れなくする
                            }
                        }
                        else{//天井
                            if( jumpFlag ){
                                *dirY = -0.05f;//天井に当たったらすぐ下へ
                            }
                        }
                        *pos +=  n;
                    }
                }
            }
        }
    }
    return flag;
}

//線分（辺）にカプセルが食い込んでいたら追い出す
int TRIANGLE::wallAndCeilingSegment( 
    VECTOR* pos, float* dirY, int jumpFlag, 
    float radius, float height, float ny 
) const{
    int flag = 0;
    if(N.y <= ny ){//壁と天井に属する線分のみ判定対象とする
        VECTOR p = *pos;
        VECTOR n;
        float sq_distance;
        //「カプセルの下半球」と「線分」の当たり判定
        p.y = pos->y + radius;
        SphereSegment( p, radius, &n, &sq_distance, &flag );
        if( flag == 0 ){
            //「カプセルの上半球」と「線分」の当たり判定
            p.y = pos->y + height - radius;
            SphereSegment( p, radius, &n, &sq_distance, &flag );
            if( flag == 0 ){
                //「カプセルの円柱部分」と「線分」の当たり判定
                p.y = pos->y + radius;
                float cylinderLen = height - radius * 2.0f;
                CylinderSegment( p, radius, cylinderLen, &n, &sq_distance, &flag );
            }
        }
        //追い出す
        if( flag ){
            float d = radius - sqrt( sq_distance );
            n = normalize( n );
            if( jumpFlag && n.y < -0.99f ){//頭が線分の下に食い込んだとき、下に落ちるのがもたつくのでこれで修正
                *dirY = -0.05f;
            }
            *pos +=  n * d;
        }
    }
    return flag;
}
void TRIANGLE::SphereSegment(
    const VECTOR& p, //球の中心
    float radius, //球の半径
    VECTOR* n, //追い出す方向ベクトル
    float* sq_distance, //線分から球の中心までの距離の２乗
    int* flag //食い込みフラッグ
)const
{
    VECTOR v;
    for( int i = 0; *flag==0 && i < 3; i++ ){
        v = p - P[ i ];
        float len = dot( v, V[ i ] );
        if( 0 <= len && len <= VLen[ i ] ){
            *n = v - V[ i ] * len;
            *sq_distance = (*n).sqMag();
            if( *sq_distance < radius * radius ){
                *flag = 1;
            }
        }
        else{
            *sq_distance = v.sqMag();
            if( *sq_distance < radius * radius ){
                *n = v;
                *flag = 1;
            }
        }
    }
}
void TRIANGLE::CylinderSegment(
    const VECTOR& p,//円柱の底面の中心座標 
    float radius, //半径
    float cylinderLen,//円柱の高さ
    VECTOR* n, //追い出す方向ベクトル
    float* sq_distance, //線分から円柱の中心線分までの距離の２乗
    int* flag //食い込みフラッグ
)const
{
    VECTOR v( 0, 1, 0 );//円柱中心軸の方向ベクトル
    for( int i = 0; *flag==0 && i < 3; i++ ){
        //平行ベクトルのとき
        VECTOR _cross = cross( v, V[ i ] );
        float len = _cross.sqMag();
        if( len < 0.000001f ) {
            VECTOR v_ = cross( P[ i ] - p, v );
            *sq_distance = v_.sqMag();
            *n = p - P[ i ];
            if( *sq_distance < radius * radius ){
                if( V[ i ].y > 0 ){
                    if( P[ i ].y + VLen[ i ] > p.y&& P[ i ].y < p.y + cylinderLen ){
                        *flag = 1;
                    }
                }
                else{
                    if( P[ i ].y > p.y&& P[ i ].y - VLen[ i ] < p.y + cylinderLen ){
                        *flag = 1;
                    }
                }
            }
        }
        else{
            //平行ベクトルでないとき
            float dv = dot( v, V[i] );
            float dA = dot( P[i] - p, v );
            float dB = dot( P[i] - p, V[i] );
            float lenA = (dA - dB * dv) / (1.0f - dv * dv);
            float lenB = (dB - dA * dv) / (dv * dv - 1.0f);
            VECTOR qA = p + v * lenA;
            VECTOR qB = P[i] + V[i] * lenB;
            *n = qA - qB;
            *sq_distance = (* n).sqMag();
            if( *sq_distance < radius * radius ){
                if( 0 <= lenA && lenA <= cylinderLen ){
                    if( 0 <= lenB && lenB <= VLen[ i ] ){
                        *flag = 1;
                    }
                }
            }
        }
    }
}

//このポリゴンに線分が交差していたら、線分の終点を交点にする
int TRIANGLE::intersection( VECTOR& sp, VECTOR* ep, float correct ){
    int flag = 0;
    //三角ポリゴンと線分の当たり判定
    // 線分(sp2ep)がP[0]を含む平面を貫通しているか
    VECTOR v1 = sp - P[ 0 ];
    VECTOR v2 = *ep - P[ 0 ];
    float d1 = dot( v1, N );
    float d2 = dot( v2, N );
    if( d1 * d2 <= 0.0f ){
        // 貫通している点の座標hpを求める
        d1 = absolute( d1 );
        d2 = absolute( d2 );
        float a = d1 / ( d1 + d2 );//貫通点までの内分比
        VECTOR v3 = v1 *  ( 1.0f - a ) + v2 * a;
        VECTOR hp = P[ 0 ] + v3;
        // hpがポリゴンの範囲内か判定
        float d;
        d = dot( cross( V[ 0 ], hp - P[ 0 ] ), N );
        if( d > 0 ){
            d = dot( cross( V[ 1 ], hp - P[ 1 ] ), N );
            if( d > 0 ){
                d = dot( cross( V[ 2 ], hp - P[ 2 ] ), N );
                if( d > 0 ){
                    // 当たっているので終点を交差点とする
                    flag = 1;
                    //補正
                    a *= correct;
                    v3 = v1 *  ( 1.0f - a ) + v2 * a;
                    hp = P[ 0 ] + v3;
                    *ep = hp;
                }
            }
        }
    }
    return flag;
}
