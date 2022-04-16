#include "UIScreen.h"
#include "graphic.h"
#include "input.h"
#include "Game.h"
#include "UIButton.h"

UIScreen::UIScreen(Game* game)
	:mGame(game)
	, mState(EActive)
	, mBackImg(-1)
	, mTextSize(40)
	, mTitlePos(width / 2, 100.0f)
	, mButtonPos(width / 2, 170.0f)
{
	// Game��UIStack�ɒǉ�
	mGame->PushUI(this);
	// �{�^���摜
	Button::SetButtonOnImg(loadImage("Assets/ButtonYellow.png"));
	Button::SetButtonOffImg(loadImage("Assets/ButtonBlue.png"));
}

UIScreen::~UIScreen()
{
	for (auto b : mButtons)
	{
		delete b;
	}
	mButtons.clear();
}

void UIScreen::ProcessInput()
{
	// �{�^��������Ȃ�
	if (!mButtons.empty())
	{
		// �}�E�X�̈ʒu���擾
		VECTOR2 mousePos(mouseX, mouseY);
		// �S�{�^���̃`�F�b�N
		for (auto b : mButtons)
		{
			// ���[���I�[�o�[���Ă�����
			if (b->ContainsPoint(mousePos))
			{
				if (isTrigger(MOUSE_LBUTTON))
				{
					b->OnClick();
				}
			}
		}
	}
}

void UIScreen::Draw()
{
	rectMode(CENTER);
	// �w�i�摜�\���i������������j
	if (mBackImg >= 0)
	{
		image(mBackImg, mBackPos.x, mBackPos.y);
	}
	
	// �^�C�g���\���i������������j
	if (mTitle.c_str())
	{
		textSize(mTextSize);
		fill(mTitleColor);
		text(mTitle.c_str(),
			mTitlePos.x - mTitle.length() * mTextSize / 4,//���p�����̂ݑΉ�
			mTitlePos.y + mTextSize / 2
		);
	}

	// �{�^���\���i������������j
	for (auto b : mButtons)
	{
		b->Draw();
	}
}

void UIScreen::CloseMe()
{
	mState = EClosing;
}

//�{�^���͏c�ɕ���Ŕz�u�����
void UIScreen::AddButton(const char* name, std::function<void()> onClick)
{
	Button* b = new Button(name, onClick, mButtonPos);
	mButtons.emplace_back(b);

	// ���ɒǉ������{�^���̈ʒu���v�Z���Ă����i�摜�̍����{�Ԋu�j
	mButtonPos.y += b->GetDimensions().y + 20.0f;
}
