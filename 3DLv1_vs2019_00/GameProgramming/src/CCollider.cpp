#include "CCollider.h"
#include "CCollisionManager.h"
#include "CColliderLine.h"

CCollider::CCollider(CCharacter3* parent, CMatrix* matrix,
	const CVector& position, float radius, ETag tag)
	:CCollider()
{
	mTag = tag; //タグの設定
	//親設定
	mpParent = parent;
	//親行列設定
	mpMatrix = matrix;
	//CTransform設定
	mPosition = position; //位置
	//半径設定
	mRadius = radius;
}

CCharacter3* CCollider::Parent()
{
	return mpParent;
}

void CCollider::Render() {
	glPushMatrix();
	//コライダの中心座標を計算
	//自分の座標×親の変換行列を掛ける
	CVector pos = mPosition * *mpMatrix;
	//中心座標へ移動
	glMultMatrixf(CMatrix().Translate(pos.X(), pos.Y(), pos.Z()).M());
	//DIFFUSE赤色設定
	float c[] = { 1.0f,0.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//球描画
	glutWireSphere(mRadius, 16, 16);
	glPopMatrix();
}

CCollider::CCollider()
	:mpParent(nullptr)
	,mpMatrix(&mMatrix)
	,mType(EType::ESPHERE)
	,mRadius(0)
{
	//コリジョンマネージャに追加
	CCollisionManager::Instance()->Add(this);
}

CCollider::~CCollider() {
	//コリジョンリストから削除
	CCollisionManager::Instance()->Remove(this);
}

//衝突判定
//CCollision(コライダ１,コライダ２)
//return:true(衝突している),false(衝突していない)
bool CCollider::Collision(CCollider* m, CCollider* o) {
	//各コライダの中心座標を求める
	//原点×コライダの変換行列×親の変換行列
	CVector mpos = m->mPosition * *m->mpMatrix;
	CVector opos = o->mPosition * *o->mpMatrix;
	//中心から中心へのベクトルを求める
	mpos = mpos - opos;
	//中心の距離が半径の合計より小さいと衝突
	if (m->mRadius + o->mRadius > mpos.Length()) {
		//衝突している
		return true;
	}
	//衝突していない
	return false;
}

bool CCollider::CollisionTriangleLine(CCollider* t, CCollider* l, CVector* a) {
	CVector v[3], sv, ev;
	//各コライダの頂点をワールド座標へ変換
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	sv = l->mV[0] * *l->mpMatrix;
	ev = l->mV[1] * *l->mpMatrix;
	//面の法線を、外積を正規化して求める
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Normalize();
	//三角の頂点から線分始点へのベクトルを求める
	CVector v0sv = sv - v[0];
	//三角形の頂点から線分終点へのベクトルを求める
	CVector v0ev = ev - v[0];

	//線分が面と交差しているか内積で確認する
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//プラスは交差してない
	if (dots * dote >= 0.0f) {
		//衝突していない(調整不要)
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//線分は面と交差している
	

	//面と線分の交点を求める
	//交点の計算
	CVector cross = sv + (ev - sv) * (abs(dots) / (abs(dots) + abs(dote)));

	//交点が三角形内なら衝突している
	//頂点１頂点２ベクトルと頂点１交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((v[1] - v[0]).Cross(cross - v[0]).Dot(normal) < 0.0f) {
		//衝突しない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//頂点２頂点３ベクトルと頂点２交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形外
	if ((v[2] - v[1]).Cross(cross - v[1]).Dot(normal) < 0.0f) {
		//
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//頂点３頂点１ベクトルと頂点３交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((v[0] - v[2]).Cross(cross - v[2]).Dot(normal) < 0.0f) {
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	 
	//調整値計算(衝突しない位置まで戻す)
	if (dots < 0.0f) {
		//始点が裏面
		*a = normal * -dots;
	}
	else {
		//終点が裏面
		*a = normal * -dote;
	}
	return true;
}

//CollisionTriangleSphere(三角コライダ,球コライダ,調整値)
//retrun:true(衝突している)false(衝突していない)
//調整値:衝突していない位置まで戻す
bool CCollider::CollisionTriangleSphere(CCollider* t, CCollider* s, CVector* a)
{
	CVector v[3], sv, ev;
	//
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	//
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Normalize();
	//
	sv = s->mPosition * *s->mpMatrix + normal * s->mRadius;
	ev = s->mPosition * *s->mpMatrix - normal * s->mRadius;
	CColliderLine line(nullptr, nullptr, sv, ev);
	//
	return CollisionTriangleLine(t, &line, a);
}

CCollider::EType CCollider::Type()
{
	return mType;
}


void CCollider::ChangePriority(int priority)
{
	mPriority = priority;
	CCollisionManager::Instance()->Remove(this); //一旦削除
	CCollisionManager::Instance()->Add(this); //追加
}

//優先度の変更
void CCollider::ChangePriority() 
{
	//自分の座標×親の変換行列を掛けてワールド座標を求める
	CVector pos = mPosition * *mpMatrix;
	//ベクトルの長さが優先度
	CCollider::ChangePriority(pos.Length());
}

void CCollider::Matrix(CMatrix* m)
{
	mpMatrix = m;
}

CCollider::ETag CCollider::Tag() {
	return mTag;
}
