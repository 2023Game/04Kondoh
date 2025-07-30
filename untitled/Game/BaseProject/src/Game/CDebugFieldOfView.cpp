#include "CDebugFieldOfView.h"
#include "Primitive.h"
#include "glut.h"

// コンストラクタ
CDebugFieldOfView::CDebugFieldOfView(CObjectBase* owner,
	float fovAngle, float fovLength)
	: CObjectBase(ETag::eDebug,ETaskPriority::eDebugFov,0,ETaskPauseType::eGame)
	, mpOwner(owner)
	, mpMatrix(nullptr)
	, mFovAngle(fovAngle)
	, mFovLength(fovLength)
{
}

// デストラクタ
CDebugFieldOfView::~CDebugFieldOfView()
{
	// 表示対象が存在する場合は
	// 自身が削除されたことを表示対象に伝える
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// 視野範囲を表示する対象を設定
void CDebugFieldOfView::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// 視野範囲を設定
void CDebugFieldOfView::Set(float angle, float length)
{
	mFovAngle = angle;
	mFovLength = length;
}

// 視野範囲行列を設定
void CDebugFieldOfView::SetMatrix(CMatrix* mtx)
{
	mpMatrix = mtx;
}

// 更新処理
void CDebugFieldOfView::Update()
{
}

// 描画
void CDebugFieldOfView::Render()
{
	// 対象がいない場合は、描画処理を行わない
	if (mpOwner == nullptr) return;

	// デプステストをオフ
	glDisable(GL_DEPTH_TEST);

	// 強制的に半透明にする
	mColor.A(0.5f);

	CMatrix m = mpMatrix != nullptr ? *mpMatrix : mpOwner->Matrix();

	// 視野範囲の扇形を描画
	Primitive::DrawSector
	(
		m,      // 表示対象の行列
		-mFovAngle, mFovAngle,  // -視野角度～視野角度の設定
		mFovLength,             // 視野距離
		mColor                  // 扇形の色
	);
	// デプステストの状態を戻す
	glEnable(GL_DEPTH_TEST);

}
