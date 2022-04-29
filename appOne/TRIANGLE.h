#include "VECTOR.h"

class TRIANGLE{
public:
    TRIANGLE();
    ~TRIANGLE();
    //�R�̒��_����O�p�`�f�[�^�����
    void create( const VECTOR& p1, const VECTOR& p2, const VECTOR& p3 );
    //pos��XZ���W���^�ォ��݂Ă��̃|���S�����ɂ��邩�ǂ�������
    int insideMe( const VECTOR& pos, float ny ) const;
    //XZ���W����Y���W���Z�o
    float getY( const VECTOR& pos ) const;
    //���̃|���S���ɃJ�v�Z�����H������ł�����ǂ��o��
    int wallAndCeilingPolygon( VECTOR* pos, float* dirY, int jumpFlag, float radius, float height, float ny ) const;
    //�����ɃJ�v�Z�����H������ł�����ǂ��o��
    int wallAndCeilingSegment( VECTOR* pos, float* dirY, int jumpFlag, float radius, float height, float ny ) const;
    //���̃|���S���ɐ������������Ă�����A�����̏I�_����_�ɂ���
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
    //����
    VECTOR N;
    float Nw=0;
    //�ӂ̃x�N�g���Ɩ@���Ƃ̊O�σx�N�g��
    VECTOR C[ 3 ];
    float TanTheta=0;
};
