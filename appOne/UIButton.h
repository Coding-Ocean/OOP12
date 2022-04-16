#pragma once
#include<functional>
#include"VECTOR2.h"
class Button
{
public:
	//このクラスを使用前に画像をセットしておく
	static void SetButtonOffImg(int img);
	static void SetButtonOnImg(int img);

	Button(const char* name,
		std::function<void()> onClick,
		const VECTOR2& pos);
	~Button();

	void Draw();

	// マウスポインタがこのボタン内にあるか
	bool ContainsPoint(const VECTOR2& pt);
	// クリックされたときに呼び出される関数
	void OnClick();
	// Getter
	const VECTOR2& GetDimensions() const { return mDimensions; }
private:
	static int mButtonOnImg;
	static int mButtonOffImg;
	static VECTOR2 mDimensions;//ボタンの幅と高さ

	std::string mName;
	float mTextSize;
	VECTOR2 mPosition;
	bool mHighlighted;
	
	std::function<void()> mOnClick;
};
