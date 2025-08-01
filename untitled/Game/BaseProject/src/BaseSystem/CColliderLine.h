#pragma once
#include "CCollider.h"

/// <summary>
/// 線分コライダー
/// </summary>
class CColliderLine : public CCollider
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コライダーの持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="v0">線分の始点</param>
	/// <param name="v1">線分の終点</param>
	/// <param name="isKinematic">trueならば、衝突時に押し戻しの影響を受けない</param>
	/// <param name="weight">コライダーの重量</param>
	CColliderLine(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1,
		bool isKinematic = false, float weight = 1.0f);

	/// <summary>
	/// 線分コライダーの設定
	/// </summary>
	/// <param name="owner">コライダーの持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="v0">線分の始点</param>
	/// <param name="v1">線分の終点</param>
	void Set(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1);

	/// <summary>
	/// 線分の始点と終点を取得
	/// </summary>
	/// <param name="v0">始点格納用</param>
	/// <param name="v1">終点格納用</param>
	void Get(CVector* v0, CVector* v1) const;

	float Radius() const;

#if _DEBUG
	// コライダー描画
	void Render() override;
#endif

protected:
	// コライダーの情報を更新
	void UpdateCol(bool isInit = false) override;

private:
	CVector mV[2];	// 線分の頂点の配列
	CVector mWV[2];	// 線分の頂点のワールド座標
	float mRadius;
};
