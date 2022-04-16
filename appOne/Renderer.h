#pragma once
#include<vector>
#include"MATRIX.h"
#include"VECTOR.h"
#include"COLOR.h"
class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();

	bool Initialize();
	void Shutdown();

	void Draw();

	void AddMesh(class MeshComponent* mesh);
	void RemoveMesh(class MeshComponent* mesh);
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	class SHADER* GetShader() { return mShader; }
	class CONTAINER* GetContainer() { return mContainer; }
private:
	class Game* mGame;
	class CONTAINER* mContainer;
	
	class SHADER* mShader;
	MATRIX mView;
	MATRIX mProj;
	VECTOR mLightPos;
	float mAmbient;
	COLOR mDiffuse;

	std::vector<class MeshComponent*>mMeshes;
	std::vector<class SpriteComponent*>mSprites;
};

