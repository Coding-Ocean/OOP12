#ifndef COLLISION_MAP_HD
#define COLLISION_MAP_HD

class COLLISION_MAP{
public:
    COLLISION_MAP( float len, const char* filename );
    ~COLLISION_MAP();
    //当たり判定用データをロードし三角形を作っていく
    void loadData( const char* fileName );
    //キャラクタを包むカプセルとステージの三角形とのあたり判定
    int capsule_triangles(class VECTOR* pos, float* dirY, int* jumpFlag, float speed, float radius, float height, float degree);
    //線分とステージの三角形との当たり判定
    int segment_triangles(class VECTOR& sp, class VECTOR* ep, float correct=1.0f );
    int segment_trianglesEco(class VECTOR& sp, class VECTOR* ep, float correct=1.0f );
    //Getter
    float minX(){ return MinX; }
    float minY(){ return MinY; }
    float minZ(){ return MinZ; }
    float maxX(){ return MaxX; }
    float maxY(){ return MaxY; }
    float maxZ(){ return MaxZ; }
    //デバッグ用　ポリゴン表示関数
    //void debugDraw( const VECTOR& actorPos, const VECTOR& camPos=VECTOR() );
    //class LINE* lineShader = 0;
    //class LINE_TRIANGLE* lineTriangle=0;
    //class LINE_CUBE* lineCube=0;
private:
    class CELL* SelectCell( const class VECTOR* pos );
    //全セル
    class CELL*** Cells = 0;
    float Len = 0;
    int NumX=0, NumZ=0, NumY=0;

    //全三角ポリゴン
    class TRIANGLE* Triangles=0;
    int NumTriangles=0;
    //ポリゴン頂点座標の最小値、最大値
    float MinX = 1000;
    float MaxX = -1000;
    float MinZ = 1000;
    float MaxZ = -1000;
    float MinY = 1000;
    float MaxY = -1000;
};

#endif