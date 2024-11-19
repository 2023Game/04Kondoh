#pragma once

// 経路探索ノードクラス
class CNavNode
{
public:
	// コンストラクタ
	CNavNode(const CVector& pos, bool isDestNode = false);
	// デストラクタ
	~CNavNode();

	// ノードの座標を取得
	const CVector& GetPos() const;
	// 遮蔽物チェックや表示用に、少し上にあげたノードの座標を返す
	CVector GetOffsetPos() const;
	// ノードの座標を設定
	void SetPos(const CVector& pos);

	// ノードの色設定
	void SetColor(const CColor& color);
	// ノードを描画（デバッグ用）
	void Render();

private:
	// 経路探索時に経由できない目的地専用のノードかどうか
	bool mIsDestNode;
	CVector mPosition;  // ノードの座標

	// デバック関連
	CColor mColor;      // ノードの色
};