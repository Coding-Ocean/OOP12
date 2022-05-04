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
    VECTOR camPos(0, -5.0f, 3.0f), lookat(0, -5.0f, 0), up(0, 1, 0);
    mView.camera(camPos, lookat, up);
    mProj.pers(3.14f/4, width / height, 0.05f, 170);

    mLightPos.set(0, 0.f, 1);
    mAmbient = 0.3f;
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
    mShader->setProjView(mProj * mView);
    mShader->setLightPosition(mLightPos);
    mShader->setLightAmbient(mAmbient);
    mShader->setDiffuse(mDiffuse);
    for (auto mesh : mMeshes)
    {
        setTextureSamplerWrap();

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
    // ƒ\[ƒgÏ‚İ”z—ñ‚Ì‘}“üêŠ‚ğ’T‚·
    // (©•ª‚æ‚è‡”Ô‚Ì‘å‚«‚¢Å‰‚Ì—v‘f‚ğ’T‚·)
    int myDrawOrder = sprite->GetDrawOrder();
    auto iter = mSprites.begin();
    for (; iter != mSprites.end(); ++iter)
    {
        if (myDrawOrder < (*iter)->GetDrawOrder())
        {
            break;
        }
    }

    // ’T‚µo‚µ‚½—v‘f‚Ì‘O‚É©•ª‚ğ‘}“ü
    mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
    // swap‚µ‚Äpop_back‚Å‚«‚È‚¢Bswap‚µ‚Ä‚µ‚Ü‚¤‚Æ‡”Ô‚ª•ö‚ê‚é‚½‚ß
    auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
    mSprites.erase(iter);
}

void Renderer::AddMesh(MeshComponent* mesh)
{
    mMeshes.emplace_back(mesh);
}

void Renderer::RemoveMesh(MeshComponent* mesh)
{
    // swap‚µ‚Äpop_back‚Å‚«‚È‚¢Bswap‚µ‚Ä‚µ‚Ü‚¤‚Æ‡”Ô‚ª•ö‚ê‚é‚½‚ß
    auto iter = std::find(mMeshes.begin(), mMeshes.end(), mesh);
    mMeshes.erase(iter);
}
