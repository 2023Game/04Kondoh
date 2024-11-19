#include "CNavNode.h"
#include "CNavManager.h"
#include "Primitive.h"

// ノードのY座標のオフセット値
#define NODE_OFFSET_Y 5.0f

// コンストラクタ
CNavNode::CNavNode(const CVector& pos, bool isDestNode)
	:mIsDestNode(isDestNode)
	, mPosition(pos)
	, mColor(0.0f, 1.0f, 0.0f, 1.0f)
{
	// 座標設定
	SetPos(mPosition);
}

// デストラクタ
CNavNode::~CNavNode()
{
}

// ノードの座標を取得
const CVector& CNavNode::GetPos() const
{
	return mPosition;
}

// 遮蔽物チェックや表示用に、少し上にあげたノードの座標を返す
CVector CNavNode::GetOffsetPos() const
{
	// 地面と重ならない様に、ノードの位置から少し上にあげた位置を返す
	return mPosition + CVector(0.0f, NODE_OFFSET_Y, 0.0f);
}

// ノードの座標を設定
void CNavNode::SetPos(const CVector& pos)
{
	// ノードの座標を更新
	mPosition = pos;

	// TODO: ノードの座標が変わったので、接続しているノードを調べ直す
}

// ノードの色設定
void CNavNode::SetColor(const CColor& color)
{
	mColor.Set(color.R(), color.G(), color.B());
}

// ノードを描画（デバッグ用）
void CNavNode::Render()
{
	// ノードの座標に球を描画
	CMatrix m;
	m.Translate(GetOffsetPos());
	Primitive::DrawWireSphere(m, 1.0f, mColor);
}
