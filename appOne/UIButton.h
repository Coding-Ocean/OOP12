#pragma once
#include<functional>
#include"VECTOR2.h"
class Button
{
public:
	//���̃N���X���g�p�O�ɉ摜���Z�b�g���Ă���
	static void SetButtonOffImg(int img);
	static void SetButtonOnImg(int img);

	Button(const char* name,
		std::function<void()> onClick,
		const VECTOR2& pos);
	~Button();

	void Draw();

	// �}�E�X�|�C���^�����̃{�^�����ɂ��邩
	bool ContainsPoint(const VECTOR2& pt);
	// �N���b�N���ꂽ�Ƃ��ɌĂяo�����֐�
	void OnClick();
	// Getter
	const VECTOR2& GetDimensions() const { return mDimensions; }
private:
	static int mButtonOnImg;
	static int mButtonOffImg;
	static VECTOR2 mDimensions;//�{�^���̕��ƍ���

	std::string mName;
	float mTextSize;
	VECTOR2 mPosition;
	bool mHighlighted;
	
	std::function<void()> mOnClick;
};
