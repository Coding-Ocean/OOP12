#include "Game.h"
#include "framework.h"
#include "graphic.h"
#include "window.h"
#include "input.h"
#include "Renderer.h"
#include "UIPause.h"

#include "Actor.h"
#include "SpriteComponent.h"
#include "TreeMeshComponent.h"
#include "Player.h"
#include "COLLISION_MAP.h"

bool Game::Initialize()
{
    window(1920, 1080, full);
    mRenderer = new Renderer(this);
    LoadData();
    initDeltaTime();
    return true;
}

void Game::RunLoop()
{
    while (notQuit)
    {
        clear(64, 128, 255);
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown()
{
    while (!mActors.empty())
    {
        delete mActors.back();
    }
    while (!mUIStack.empty())
    {
        delete mUIStack.back();
        mUIStack.pop_back();
    }
    delete mCollisionMap;
    delete mRenderer;
}

void Game::AddActor(Actor* actor)
{
    if (mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    }
    else
    {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor* actor)
{
    //このactorがmActorsにあるか探す
    auto iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        //このActorとケツのActorを入れ替える(消去後コピー処理を避けるため)
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::PushUI(UIScreen* uiScreen)
{
    mUIStack.emplace_back(uiScreen);
}

void Game::ProcessInput()
{
    if (mGameState == EGameplay)
    {
        mUpdatingActors = true;
        for (auto actor : mActors)
        {
            actor->ProcessInput();
        }
        mUpdatingActors = false;

        //Pause
        if (isTrigger(KEY_SPACE))
        {
            new UIPause(this);
        }
    }
    else if (!mUIStack.empty())
    {
        mUIStack.back()->ProcessInput();
    }
}

void Game::UpdateGame()
{
    setDeltaTime();

    if (mGameState == EGameplay)
    {
        // mActors更新(更新中にnewされたActorはmPendingActorsに追加される)
        mUpdatingActors = true;
        for (auto actor : mActors)
        {
            actor->Update();
        }
        mUpdatingActors = false;

        // 追加を延期したActorをmActorsに追加する
        for (auto pending : mPendingActors)
        {
            mActors.emplace_back(pending);
        }
        mPendingActors.clear();

        // Dead状態のActorを直下のdeadActorsに抽出する
        std::vector<Actor*> deadActors;
        for (auto actor : mActors)
        {
            if (actor->GetState() == Actor::EDead)
            {
                deadActors.emplace_back(actor);
            }
        }
        // deadActorsを消去する(mActorsからも取り除かれる)
        for (auto actor : deadActors)
        {
            delete actor;
        }
    }

    // UI更新
    for (auto ui : mUIStack)
    {
        if (ui->GetState() == UIScreen::EActive)
        {
            ui->Update();
        }
    }
    // UIのdelete
    auto iter = mUIStack.begin();
    while (iter != mUIStack.end())
    {
        if ((*iter)->GetState() == UIScreen::EClosing)
        {
            delete *iter;
            iter = mUIStack.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    //ゲーム終了
    if (mGameState == EQuit)
    {
        closeWindow();
    }
}

void Game::GenerateOutput()
{
    mRenderer->Draw();
}

//このゲームに固有のロジック
void Game::LoadData()
{
    //背景
    auto a = new Actor(this);
    auto tc = new TreeMeshComponent(a);
    tc->SetTree("background");
    
    //Player
    new Player(this);

    mCollisionMap = new COLLISION_MAP(5,"assets\\collisionMap.txt");
}