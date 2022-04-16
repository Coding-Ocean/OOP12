#pragma once
#include<functional>
#include"COLOR.h"
#include"VECTOR2.h"
//���[�U�[�C���^�[�t�F�C�X���N���X
class UIScreen
{
public:
	UIScreen(class Game* game);
	virtual ~UIScreen();
	// �h���N���X�ŃI�[�o�[���C�h�\
	virtual void ProcessInput();
	virtual void Update(){};
	virtual void Draw();
	// UI�X�e�[�g�B�A�N�e�B�u���A���邩
	enum UIState
	{
		EActive,
		EClosing
	};
	// UI�X�e�[�g��Closing�ɐݒ�
	void CloseMe();
	// UI�X�e�[�g���擾
	UIState GetState() const { return mState; }
	// UI�^�C�g����ݒ�
	void SetTitle(const char* text) { mTitle = text; }
	// �{�^����ǉ��i�c�ɔz�u����Ă����j
	void AddButton(const char* name, std::function<void()> onClick);
protected:
	class Game* mGame;

	// �X�e�[�g
	UIState mState;

	//�w�i�摜
	int mBackImg;
	VECTOR2 mBackPos;

	//�^�C�g�������Ƃ��̑���
	std::string mTitle;
	COLOR mTitleColor;
	VECTOR2 mTitlePos;
	float mTextSize;

	//�{�^��
	std::vector<class Button*> mButtons;
	VECTOR2 mButtonPos;
};

