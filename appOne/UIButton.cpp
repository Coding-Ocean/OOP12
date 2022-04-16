#include "UIButton.h"
#include "graphic.h"

int Button::mButtonOffImg = -1;
int Button::mButtonOnImg = -1;
VECTOR2 Button::mDimensions;
void Button::SetButtonOffImg(int img)
{
	mButtonOffImg = img;
	mDimensions = getTextureSize(img);
}
void Button::SetButtonOnImg(int img) 
{ 
	mButtonOnImg = img; 
}

Button::Button(const char* name, std::function<void()> onClick,
	const VECTOR2& pos)
	: mName(name)
	, mTextSize(30)
	, mPosition(pos)
	, mHighlighted(false)
	, mOnClick(onClick)
{
}

Button::~Button()
{
}

bool Button::ContainsPoint(const VECTOR2& pt)
{
	bool no =
		pt.x < (mPosition.x - mDimensions.x / 2.0f) ||
		pt.x >(mPosition.x + mDimensions.x / 2.0f) ||
		pt.y < (mPosition.y - mDimensions.y / 2.0f) ||
		pt.y >(mPosition.y + mDimensions.y / 2.0f);

	mHighlighted = !no;
	
	return !no;
}

void Button::OnClick()
{
	// �A�^�b�`���ꂽ�֐��n���h��������ΌĂяo���B
	if (mOnClick)
	{
		mOnClick();
	}
}

void Button::Draw()
{
	// �{�^���摜�\��
	int buttonImg = mHighlighted ? mButtonOnImg : mButtonOffImg;
	if(buttonImg>=0) image(buttonImg, mPosition.x, mPosition.y);
	// �{�^�������\��
	textSize(mTextSize);
	VECTOR2 pos;
	pos.x = mPosition.x - mName.length() * mTextSize / 4;//���p�����̂ݑΉ�
	pos.y = mPosition.y + mTextSize / 2 - 2;
	text(mName.c_str(), pos.x, pos.y);
}
