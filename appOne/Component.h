#pragma once
class Component
{
public:
	// コンストラクタ
	// (updateOderが小さい程、先に更新される)
	Component(class Actor* owner, int updateOrder = 100);
	// デストラクタ
	virtual ~Component();
	// このコンポーネントの入力処理
	virtual void ProcessInput(){}
	// このコンポーネントの更新処理
	virtual void Update(){}

	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	// このComponentを所有するActor
	class Actor* mOwner;
	// 更新の順番
	int mUpdateOrder;
};

