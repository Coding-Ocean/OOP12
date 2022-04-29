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

//�R�̒��_����O�p�`�f�[�^�����
void TRIANGLE::create( const VECTOR& p1, const VECTOR& p2, const VECTOR& p3 ){
    //���_
    P[ 0 ] = p1;
    P[ 1 ] = p2;
    P[ 2 ] = p3;
    //�R�ӂ̃x�N�g��
    V[ 0 ] = p2 - p1;
    V[ 1 ] = p3 - p2;
    V[ 2 ] = p1 - p3;
    //�@���x�N�g��
    N = cross( V[ 0 ], V[ 1 ] );
    N = normalize( N );
    //
    for( int i = 0; i < 3; i++ ){
        //�x�N�g���̑傫��
        VLen[ i ] = V[ i ].mag();
        //���K��
        V[ i ] /= VLen[ i ];
        //�ӂ̃x�N�g���Ɩ@���Ƃ̊O�σx�N�g��
        C[ i ] = cross( V[ i ], N );
    }
    //���ʂS�Ԗڂ̗v�f
    Nw = -( N.x * p1.x + N.y * p1.y + N.z * p1.z );
    //�������Ƃ��ɃW�����v�ɂȂ�Ȃ��悤�ȍő嗎�������߂邽�߂̏���
    TanTheta = tan( acos( N.y - 0.01f ) );//0.01�͒����l�B�]�v�ɌX�����Ă����B
}

//pos��XZ���W���^�ォ��݂Ă��̃|���S�����ɂ��邩�ǂ�������
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
//pos������ׂ�����Y��Ԃ�
float TRIANGLE::getY( const VECTOR& pos ) const{
    return ( N.x * pos.x + N.z * pos.z + Nw ) / -N.y;
}

//���̃|���S���ɃJ�v�Z�����H������ł�����ǂ��o��-----------------------------
int TRIANGLE::wallAndCeilingPolygon( 
    VECTOR* pos, float* dirY, int jumpFlag, 
    float radius, float height, float ny 
) const{
    int flag = 0;
    if( N.y <= ny ){
        VECTOR p = *pos;
        if( N.y >= 0.0f ){//�ǃ|���S��
            p.y += radius;//�������̒��S
        }
        else{//�V��|���S��
            p.y += height - radius;//�㔼���̒��S
        }
        //�|���S�����܂ޕ��ʂ܂ł̋���
        VECTOR v;
        v = p - P[ 0 ];
        float distance = dot( N, v );
        if( 0.0f <= distance  && distance < radius ){
            //�|���S���̖@��������p���܂܂�Ă��邩�`�F�b�N
            if( dot( v, C[ 0 ] ) <= 0.0f ){
                v = p - P[ 1 ];
                if( dot( v, C[ 1 ] ) <= 0.0f ){
                    v = p - P[ 2 ];
                    if( dot( v, C[ 2 ] ) <= 0.0f ){
                        //�|���S���ɐH������ł���-----------------------------------
                        flag = 1;
                        float d = radius - distance;
                        VECTOR n = N * d;
                        if( N.y >= 0.0f ){//��
                            if( !jumpFlag ){
                                n.y = 0.0f;//�΂߂ɂȂ��Ă���ǂ�o��Ȃ�����
                            }
                        }
                        else{//�V��
                            if( jumpFlag ){
                                *dirY = -0.05f;//�V��ɓ��������炷������
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

//�����i�Ӂj�ɃJ�v�Z�����H������ł�����ǂ��o��
int TRIANGLE::wallAndCeilingSegment( 
    VECTOR* pos, float* dirY, int jumpFlag, 
    float radius, float height, float ny 
) const{
    int flag = 0;
    if(N.y <= ny ){//�ǂƓV��ɑ���������̂ݔ���ΏۂƂ���
        VECTOR p = *pos;
        VECTOR n;
        float sq_distance;
        //�u�J�v�Z���̉������v�Ɓu�����v�̓����蔻��
        p.y = pos->y + radius;
        SphereSegment( p, radius, &n, &sq_distance, &flag );
        if( flag == 0 ){
            //�u�J�v�Z���̏㔼���v�Ɓu�����v�̓����蔻��
            p.y = pos->y + height - radius;
            SphereSegment( p, radius, &n, &sq_distance, &flag );
            if( flag == 0 ){
                //�u�J�v�Z���̉~�������v�Ɓu�����v�̓����蔻��
                p.y = pos->y + radius;
                float cylinderLen = height - radius * 2.0f;
                CylinderSegment( p, radius, cylinderLen, &n, &sq_distance, &flag );
            }
        }
        //�ǂ��o��
        if( flag ){
            float d = radius - sqrt( sq_distance );
            n = normalize( n );
            if( jumpFlag && n.y < -0.99f ){//���������̉��ɐH�����񂾂Ƃ��A���ɗ�����̂��������̂ł���ŏC��
                *dirY = -0.05f;
            }
            *pos +=  n * d;
        }
    }
    return flag;
}
void TRIANGLE::SphereSegment(
    const VECTOR& p, //���̒��S
    float radius, //���̔��a
    VECTOR* n, //�ǂ��o�������x�N�g��
    float* sq_distance, //�������狅�̒��S�܂ł̋����̂Q��
    int* flag //�H�����݃t���b�O
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
    const VECTOR& p,//�~���̒�ʂ̒��S���W 
    float radius, //���a
    float cylinderLen,//�~���̍���
    VECTOR* n, //�ǂ��o�������x�N�g��
    float* sq_distance, //��������~���̒��S�����܂ł̋����̂Q��
    int* flag //�H�����݃t���b�O
)const
{
    VECTOR v( 0, 1, 0 );//�~�����S���̕����x�N�g��
    for( int i = 0; *flag==0 && i < 3; i++ ){
        //���s�x�N�g���̂Ƃ�
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
            //���s�x�N�g���łȂ��Ƃ�
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

//���̃|���S���ɐ������������Ă�����A�����̏I�_����_�ɂ���
int TRIANGLE::intersection( VECTOR& sp, VECTOR* ep, float correct ){
    int flag = 0;
    //�O�p�|���S���Ɛ����̓����蔻��
    // ����(sp2ep)��P[0]���܂ޕ��ʂ��ђʂ��Ă��邩
    VECTOR v1 = sp - P[ 0 ];
    VECTOR v2 = *ep - P[ 0 ];
    float d1 = dot( v1, N );
    float d2 = dot( v2, N );
    if( d1 * d2 <= 0.0f ){
        // �ђʂ��Ă���_�̍��Whp�����߂�
        d1 = absolute( d1 );
        d2 = absolute( d2 );
        float a = d1 / ( d1 + d2 );//�ђʓ_�܂ł̓�����
        VECTOR v3 = v1 *  ( 1.0f - a ) + v2 * a;
        VECTOR hp = P[ 0 ] + v3;
        // hp���|���S���͈͓̔�������
        float d;
        d = dot( cross( V[ 0 ], hp - P[ 0 ] ), N );
        if( d > 0 ){
            d = dot( cross( V[ 1 ], hp - P[ 1 ] ), N );
            if( d > 0 ){
                d = dot( cross( V[ 2 ], hp - P[ 2 ] ), N );
                if( d > 0 ){
                    // �������Ă���̂ŏI�_�������_�Ƃ���
                    flag = 1;
                    //�␳
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
