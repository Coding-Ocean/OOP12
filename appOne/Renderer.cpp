#include "Renderer.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "UIScreen.h"
#include "Game.h"
#include "SHADER/LAMBERT.h"
#include "CONTAINER/CONTAINER.h"

Renderer::Renderer(Game* game)
    :mGame(game)
{
    Initialize();
}

Renderer::~Renderer()
{
    Shutdown();
}

bool Renderer::Initialize()
{
    mShader = new LAMBERT;
    setRasterizerCullBack();
    VECTOR camPos(0, 0.0f, 6.0f), lookat(0, 1.0f, 0), up(0, 1, 0);
    mView.camera(camPos, lookat, up);
    mProj.pers(3.14f / 4, width / height, 1, 20);
    mLightPos.set(0, 1, 1);
    mAmbient = 0.8f;
    mDiffuse.set(1, 1, 1, 1);

    mContainer = new CONTAINER("Assets\\Assets.txt");

    return false;
}

void Renderer::Shutdown()
{
    delete mShader;
    delete mContainer;
}

void Renderer::Draw()
{
    clear(60);

    mShader->setProjView(mProj * mView);
    mShader->setLightPosition(mLightPos);
    mShader->setLightAmbient(mAmbient);
    mShader->setDiffuse(mDiffuse);
    for (auto mesh : mMeshes)
    {
        mesh->Draw();
    }

    for (auto sprite : mSprites)
    {
        sprite->Draw();
    }

    for (auto ui : mGame->GetUIStack())
    {
        ui->Draw();
    }
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
    // ソート済み配列の挿入場所を探す
    // (自分より順番の大きい最初の要素を探す)
    int myDrawOrder = sprite->GetDrawOrder();
    auto iter = mSprites.begin();
    for (; iter != mSprites.end(); ++iter)
    {
        if (myDrawOrder < (*iter)->GetDrawOrder())
        {
            break;
        }
    }

    // 探し出した要素の前に自分を挿入
    mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
    // swapしてpop_backできない。swapしてしまうと順番が崩れるため
    auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
    mSprites.erase(iter);
}

void Renderer::AddMesh(MeshComponent* mesh)
{
    mMeshes.emplace_back(mesh);
}

void Renderer::RemoveMesh(MeshComponent* mesh)
{
    // swapしてpop_backできない。swapしてしまうと順番が崩れるため
    auto iter = std::find(mMeshes.begin(), mMeshes.end(), mesh);
    mMeshes.erase(iter);
}
