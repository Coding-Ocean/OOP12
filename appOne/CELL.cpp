#include"COLOR.h"
#include"TRIANGLE.h"
#include"CELL.h"
#include"mathUtil.h"

void CELL::setLen_Pos( float len, float x, float y, float z ) 
{
    //������x�͍��A���͉��A���͉��̍��W
    Len = len;  X = x; Y = y; Z = z;

    //�E�A��A��O�̍��W�����߂�
    float r = x + len;//right�E
    float t = y + len;//top��
    float fr = z + len;//front��O

    P[ 0 ] = VECTOR( x, t, fr );//�����O
    P[ 1 ] = VECTOR( r, t, fr );//�E���O
    P[ 2 ] = VECTOR( x, y, fr );//������O
    P[ 3 ] = VECTOR( r, y, fr );//�E����O

    P[ 4 ] = VECTOR( x, t, z );//���㉜
    P[ 5 ] = VECTOR( r, t, z );//�E�㉜
    P[ 6 ] = VECTOR( x, y, z );//������
    P[ 7 ] = VECTOR( r, y, z );//�E����
}
void CELL::regist(class TRIANGLE& t )
{
    //TRIANGLE�̂R���_
    VECTOR* tp = t.p();

    //�O�p�`�̑S���_��AABB�̊e�ʂ��㉺�E���O��ɑ��݂��遁�������Ă��Ȃ�
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

    //�u�O�p�`�̂����ꂩ�̒��_�v���uAAB�v�̒��ɓ����Ă��邩
    for( int i = 0; hitSw == 0 && i < 3; i++ ){
        if( P[ 0 ].x <= tp[ i ].x && tp[ i ].x <= P[ 1 ].x &&
            P[ 2 ].y <= tp[ i ].y && tp[ i ].y <= P[ 0 ].y &&
            P[ 4 ].z <= tp[ i ].z && tp[ i ].z <= P[ 0 ].z )
        {
            hitSw = 1;
        }
    }
    
    //�u�O�p�`�̂����ꂩ�̕Ӂv���uAAB�̂����ꂩ�̖ʁv�Ɍ������Ă��邩
    if( hitSw == 0 )
    {
        //�O�p�`�̕ӂ̎n�_sp �I�_ep
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
                    //�ђʂ��Ă���_�̍��Whp�����߂�
                    d1 = absolute( d1 );
                    d2 = absolute( d2 );
                    float a = d1 / (d1 + d2);//�ђʓ_�܂ł̓�����
                    VECTOR v3 = v1 * (1.0f - a) + v2 * a;
                    VECTOR hp = P[ Idx[ j ][ 0 ] ] + v3;//�q�b�g�|�C���g
                    //hp���S�p�|���S���͈͓̔�������
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
    
    //�uAAB�̑Ίp���̂����ꂩ�v���u�O�p�`�v�ƌ������Ă��邩
    if( hitSw == 0 )
    {
        for( int j = 0; hitSw == 0 && j < 4; j++ ){
            VECTOR ep = P[ 7 - j ];
            if( t.intersection( P[ j ], &ep ) ){
                hitSw = 3;
            }
        }
    }

    //�o�^
    if( hitSw ){
        Triangles.push_back( &t );
        NumTriangles++;
    }
}

//�u�J�v�Z���v�Ɓu�ǂ܂��͓V��v�Ƃ̓����蔻��
int CELL::capsule_triangles( VECTOR* pos, float* dirY, int* jumpFlag, 
    //float speed, 
    float radius, float height, float degree, int* step ) const 
{
    //�|���S���̖@���x�N�g����Y���W������ny��菬�������̂�ǂƂ���
    float ny = cosf( 3.141592f * degree / 180.0f );
    //�J�v�Z�����ǂƓV��̃|���S���ɐH�����񂾕�����pos��ǂ��o��
    int cnt = 0;
    for( int i = 0; cnt < 2 && i < NumTriangles; i++ ){
        if( Triangles[ i ]->wallAndCeilingPolygon( pos, dirY, *jumpFlag, radius, height, ny ) ){
            cnt++;
        }
    }
    if( cnt )return cnt;
    //�J�v�Z�����ǂƓV��̐����ɐH�����񂾕�����pos��ǂ��o��
    for( int i = 0; cnt < 3 && i < NumTriangles; i++ ){
        if( Triangles[ i ]->wallAndCeilingSegment( pos, dirY, *jumpFlag, radius, height, ny ) ){
            cnt++;
            *step = 1;
        }
    }
    return cnt;
}
//�u�J�v�Z���v�Ɓu���v�Ƃ̓����蔻��
int CELL::capsule_floor_triangles( VECTOR* pos, float* dirY, int* jumpFlag, float speed, 
    float radius, float height, float degree, int step ) const 
{
    //�|���S���̖@���x�N�g����Y���W������ny��菬�������̂�ǂƂ���
    float ny = cosf( 3.141592f * degree / 180.0f );
    //���ɗ�������B���łȂ���Η���
    float floorY = -100.0f;//���̍Œ�l�ȉ������Ă���
    float tanTheta = 0.0f;//�⓹������Ƃ��W�����v�ɂȂ�Ȃ����������߂邽�߂Ɏg��
    GetNearestFloorY( pos, 1.0f, ny, &tanTheta, &floorY );
    if( *jumpFlag == 0 ){
        float diff = pos->y - floorY;
        if( step && 0.0f < diff && diff < radius / 1.0f ){//�K�i����点��
            ;
        }
        else if(  diff <= speed * tanTheta ){//speed*tanTheta�ō⓹������Ƃ��̍ő嗎�������߂���
            pos->y = floorY;
        }
        else{//����
            *jumpFlag = 1;
            *dirY = 0;
        }
    }
    else{
        if( pos->y <= floorY ){//���n
            pos->y = floorY;
            *jumpFlag = 0;
        }
    }

    return 0;
}
//�I�u�W�F�N�g���Ⴍ�āA�����Ƃ��߂����̂x���W���擾
int CELL::GetNearestFloorY(
    const VECTOR* pos, 
    float offsetY, //�������x���������āA����˂�������ꍇ�̒����l 
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

//�J�����̈ʒu���I�u�W�F�N�g�̑O�Ɉړ�
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
    //�n�_�ƏI�_�Ƃ�AABB�̊e�ʂ��㉺�E���O��ɑ��݂��遁�������Ă��Ȃ�
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

    //�u�����v���uAABB�̂����ꂩ�̖ʁv�Ɍ������Ă��邩
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
                //�ђʂ��Ă���_�̍��Whp�����߂�
                d1 = absolute( d1 );
                d2 = absolute( d2 );
                float a = d1 / (d1 + d2);//�ђʓ_�܂ł̓�����
                VECTOR v3 = v1 * (1.0f - a) + v2 * a;
                VECTOR hp = P[ Idx[ j ][ 0 ] ] + v3;//�q�b�g�|�C���g
                //hp���S�p�|���S���͈͓̔�������
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
