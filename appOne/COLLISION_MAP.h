#ifndef COLLISION_MAP_HD
#define COLLISION_MAP_HD

class COLLISION_MAP{
public:
    COLLISION_MAP( float len, const char* filename );
    ~COLLISION_MAP();
    //�����蔻��p�f�[�^�����[�h���O�p�`������Ă���
    void loadData( const char* fileName );
    //�L�����N�^���ރJ�v�Z���ƃX�e�[�W�̎O�p�`�Ƃ̂����蔻��
    int capsule_triangles(class VECTOR* pos, float* dirY, int* jumpFlag, float speed, float radius, float height, float degree);
    //�����ƃX�e�[�W�̎O�p�`�Ƃ̓����蔻��
    int segment_triangles(class VECTOR& sp, class VECTOR* ep, float correct=1.0f );
    int segment_trianglesEco(class VECTOR& sp, class VECTOR* ep, float correct=1.0f );
    //Getter
    float minX(){ return MinX; }
    float minY(){ return MinY; }
    float minZ(){ return MinZ; }
    float maxX(){ return MaxX; }
    float maxY(){ return MaxY; }
    float maxZ(){ return MaxZ; }
    //�f�o�b�O�p�@�|���S���\���֐�
    //void debugDraw( const VECTOR& actorPos, const VECTOR& camPos=VECTOR() );
    //class LINE* lineShader = 0;
    //class LINE_TRIANGLE* lineTriangle=0;
    //class LINE_CUBE* lineCube=0;
private:
    class CELL* SelectCell( const class VECTOR* pos );
    //�S�Z��
    class CELL*** Cells = 0;
    float Len = 0;
    int NumX=0, NumZ=0, NumY=0;

    //�S�O�p�|���S��
    class TRIANGLE* Triangles=0;
    int NumTriangles=0;
    //�|���S�����_���W�̍ŏ��l�A�ő�l
    float MinX = 1000;
    float MaxX = -1000;
    float MinZ = 1000;
    float MaxZ = -1000;
    float MinY = 1000;
    float MaxY = -1000;
};

#endif