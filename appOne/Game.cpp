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

bool Game::Initialize()
{
    window(1024, 768, full);
    mRenderer = new Renderer(this);

    Actor* a = new Actor(this);
    a->SetPosition(VECTOR(0,0));
    auto sc = new SpriteComponent(a);
    sc->SetImage(loadImage("assets\\enemy01.png"));
    sc->SetRectMode(CORNER);

    a = new Actor(this);
    a->SetPosition(VECTOR(0, 0, 0));
    auto tc = new TreeMeshComponent(a);
    tc->SetTree("explorer");

    initDeltaTime();
    return true;
}

void Game::RunLoop()
{
    while (notQuit)
    {
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

    delete mRenderer;
}

void Game::AddActor(Actor* actor)
{
    //Update���Ȃ�A�ǉ���Update��ɉ�������
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
    //����actor��mActors�ɂ��邩�T��
    auto iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        //����Actor�ƃP�c��Actor�����ւ���(������R�s�[����������邽��)
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
        // mActors�X�V(�X�V����new���ꂽActor��mPendingActors�ɒǉ������)
        mUpdatingActors = true;
        for (auto actor : mActors)
        {
            actor->Update();
        }
        mUpdatingActors = false;

        // �ǉ�����������Actor��mActors�ɒǉ�����
        for (auto pending : mPendingActors)
        {
            mActors.emplace_back(pending);
        }
        mPendingActors.clear();

        // Dead��Ԃ�Actor�𒼉���deadActors�ɒ��o����
        std::vector<Actor*> deadActors;
        for (auto actor : mActors)
        {
            if (actor->GetState() == Actor::EDead)
            {
                deadActors.emplace_back(actor);
            }
        }
        // deadActors����������(mActors�������菜�����)
        for (auto actor : deadActors)
        {
            delete actor;
        }
    }

    // UI�X�V
    for (auto ui : mUIStack)
    {
        if (ui->GetState() == UIScreen::EActive)
        {
            ui->Update();
        }
    }
    // UI��delete
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

    //�Q�[���I��
    if (mGameState == EQuit)
    {
        closeWindow();
    }
}

void Game::GenerateOutput()
{
    mRenderer->Draw();
}

//���̃Q�[���ɌŗL�̃��W�b�N
void Game::AddUfo(Ufo* ufo)
{
    mUfos.emplace_back(ufo);
}

void Game::RemoveUfo(Ufo* ufo)
{
    auto iter = std::find(mUfos.begin(), mUfos.end(), ufo);
    if (iter != mUfos.end())
    {
        std::iter_swap(iter, mUfos.end() - 1);
        mUfos.pop_back();
    }
}
