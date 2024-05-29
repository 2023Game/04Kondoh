#ifndef CMATERIAL_H
#define CMATERIAL_H
#define MATERIAL_NAME_LEN 64 //名前の長さ

#include "CTexture.h"
#include "CModelX.h"

/*
マテリアルクラス
マテリアルのデータを扱う
*/

class CMaterial {
public:
	CMaterial(CModelX* model);
	~CMaterial();
	//頂点数の設定
	//VertexNum(頂点数)
	void VertexNum(int num);
	//頂点数の取得
	int VertexNum();
	//テクスチャ取得
	CTexture* Texture();
	//マテリアルを無効にする
	void Disabled();
	//デフォルトコンストラクタ
	CMaterial();
	//マアテリアルを有効にする
	void Enabled();
	//マテリアルの名前を取得
	char* Name();
	//マテリアルの名前を設定
	//Name(またリアルの名前)
	void Name(char* name);
	//mDiffuse配列の取得
	float* Diffuse();

private:
	float mPower;
	float mSpecular[3];
	float mEmissive[3];
	//テクスチャファイル名
	char* mpTextureFilename;
	//マテリアル毎の頂点
	int mVertexNum;
	//テクスチャ
	CTexture mTexture;
	//マテリアル名
	char mName[MATERIAL_NAME_LEN + 1];
	//拡散光の色RGBA
	float mDiffuse[4];
};

#endif // !CMATERIAL_H

