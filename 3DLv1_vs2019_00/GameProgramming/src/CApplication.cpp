#include "CApplication.h"
#include "CRectangle.h"
//OpenGL
#include "glut.h"
#include "CVector.h"
#include "CTriangle.h"

//クラスのstatic変数
CTexture CApplication::mTexture;
CCharacterManager CApplication::mCharacterManager;

#define SOUND_BGM "res\\mario.wav" //BGM音声ファイル
#define SOUND_OVER "res\\mdai.wav" //ゲームオーバー音声ファイル

CCharacterManager* CApplication::CharacterManager()
{
	return &mCharacterManager;
}

CTexture* CApplication::Texture()
{
	return &mTexture;
}

void CApplication::Start()
{
	mEye = CVector(1.0f, 2.0f, 3.0f);
}

void CApplication::Update()
{
	//頂点１,頂点２,頂点３,法線データの作成
	CVector v0, v1, v2, n;
	//法線を上向きで設定する
	n.Set(0.0f, 1.0f, 0.0f);
	//頂点１の座標を設定する
	v0.Set(0.0f, 0.0f, 0.5f);
	//頂点２の座標を設定する
	v1.Set(1.0f, 0.0f, 0.0f);
	//頂点３の座標を設定する
	v2.Set(0.0f, 0.0f, -0.5f);


	//視点の設置
	//gluLookAt(視点X,視点Y,視点Z,中心X,中心Y,中心Z,上向X,上向Y,上向Z)
	gluLookAt(mEye.X(), mEye.Y(), mEye.Z(), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	if (mInput.Key('J'))
	{
		mEye = mEye - CVector(0.1f, 0.0f, 0.0f);
	}
	if (mInput.Key('L'))
	{
		mEye = mEye + CVector(0.1f, 0.0f, 0.0f);
	}

	if (mInput.Key('I'))
	{
		mEye = mEye - CVector(0.0f, 0.0f, 0.1f);
	}
	if (mInput.Key('K'))
	{
		mEye = mEye + CVector(0.0f, 0.0f, 0.1f);
	}

	if (mInput.Key('M'))
	{
		mEye = mEye - CVector(0.0f, 0.1f, 0.0f);
	}
	if (mInput.Key('O'))
	{
		mEye = mEye + CVector(0.0f, 0.1f, 0.0f);
	}


	//描画開始
	//glBigin(形)
	//GL_TRIANGLES:三角形
	glBegin(GL_TRIANGLES);

	//法線(面の向き)の設定
	glNormal3f(n.X(), n.Y(), n.Z());
	//頂点座標の設置
	glVertex3f(v0.X(), v0.Y(), v0.Z());
	glVertex3f(v1.X(), v1.Y(), v1.Z());
	glVertex3f(v2.X(), v2.Y(), v2.Z());

	//法線と頂点の設定
	n.Set(0.0f, 0.0f, 1.0f);
	v0.Set(0.5f, 0.0f, 0.0f);
	v1.Set(0.0f, 1.0f, 0.0f);
	v2.Set(-0.5f, 0.0f, 0.0f);
	//法線(面の向き)の設定
	glNormal3f(n.X(), n.Y(), n.Z());
	//三角形２の描画
	glVertex3f(v0.X(), v0.Y(), v0.Z());
	glVertex3f(v1.X(), v1.Y(), v1.Z());
	glVertex3f(v2.X(), v2.Y(), v2.Z());

	//法線と頂点の設定
	n.Set(1.0f, 0.0f, 0.0f);
	v0.Set(0.0f, 0.5f, 0.0f);
	v1.Set(0.0f, 0.0f, 1.0f);
	v2.Set(0.0f, -0.5f, 0.0f);
	//法線(面の向き)の設定
	glNormal3f(n.X(), n.Y(), n.Z());
	//三角形３の描画
	glVertex3f(v0.X(), v0.Y(), v0.Z());
	glVertex3f(v1.X(), v1.Y(), v1.Z());
	glVertex3f(v2.X(), v2.Y(), v2.Z());

	//描画終了
	glEnd();

	//三角形クラスのインスタンス作成
	CTriangle t0;
	//法線と頂点の設定
	t0.Vertex(CVector(1.0f, 0.0f, 0.5f), CVector(2.0f, 0.0f, 0.0f), CVector(1.0f, 0.0f, -0.5f));
	t0.Normal(CVector(0.0f, 1.0f, 0.0f));
	//三角形の描写
	t0.Render();

	CTriangle t1;
	t1.Vertex(CVector(0.0f, 0.5f, 1.0f), CVector(0.0f, 0.0f, 2.0f), CVector(0.0f, -0.5f, 1.0f));
	t1.Normal(CVector(1.0f, 0.0f, 0.0f));
	t1.Render();

	CTriangle t2;
	t2.Vertex(CVector(-0.5f, 1.0f, 0.0f), CVector(0.5f, 1.0f, 0.0f), CVector(0.0f, 2.0f, 0.0f));
	t2.Normal(CVector(0.0f, 0.0f, 1.0f));
	t2.Render();
}
