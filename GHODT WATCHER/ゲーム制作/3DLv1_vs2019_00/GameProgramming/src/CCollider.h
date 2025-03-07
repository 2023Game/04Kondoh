#ifndef CCOLLIDER_H
#define CCOLLIDER_H
//キャラクタークラスのインクルード
#include "CCharacter3.h"

/*
コライダクラス
衝突判定データ
*/
class CCollider : public CTransform, public CTask {
	friend CCollisionManager;
public:
	
	//優先度の変更
	virtual void ChangePriority();
	//優先度の変更
	void ChangePriority(int priority);

	//CalcCalcPointLineDist(点,始点,終点,線上の最短点,割合)
	//点と線(始点、終点を通る直線)の最短距離を求める
	//float CalcPointLineDist(const CVector& p, const CVector& s, const CVector e, CVector* mp, float* t);

	static bool CCollider::CollisionSphereLine(CCollider* s, CCollider* l, CVector* a);
	
	//CollisionTriangleSphere(三角コライダ,球コライダ,調整値)
	//retrun:true(衝突している)false(衝突していない)
	//調整値:衝突しない位置まで戻す値
	static bool CollisionTriangleSphere(CCollider* triangle, CCollider* sphere,
		CVector* adjust);

	//CollisionTriangleLine(三角コライダ,線分コライダ,調整値)
	//return:true(衝突している)false(衝突していない)
	//調整値：衝突しない位置まで戻す
	static bool CollisionTriangleLine(CCollider* triangle, CCollider* line, CVector* adjust);

	//コライダタイプ
	enum class EType {
		ESPHERE,   //球コライダ
		ETRIANGLE, //三角コライダ
		ELINE      //線分コライダ
	};
	//デフォルトコンストラクタ
	CCollider();

	//衝突判定
	//CCollider(コライダ１,コライダ２)
	//return:true(衝突している),false(衝突していない)
	static bool Collision(CCollider* m, CCollider* o);
	~CCollider();
	//コンストラクタ
	//CCollider(親、親行列、位置、半径)
	CCollider(CCharacter3* parent, CMatrix* matrix,
		const CVector& position, float radius);
	//親ポインタの取得
	CCharacter3* Parent();
	//描画
	void Render();
	EType Type();

protected:
	EType mType; //コライダタイプ
	//頂点
	CVector mV[3];
	CCharacter3* mpParent; //親
	CMatrix* mpMatrix; //親行列
	float mRadius;  //半径

};


#endif // !CCOLLIDER_H

