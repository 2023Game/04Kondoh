#ifndef CMATERIAL_H
#define CMATERIAL_H
#define MATERIAL_NAME_LEN 64 //名前の長さ

#include "CTexture.h"

/*
マテリアルクラス
マテリアルのデータを扱う
*/

class CMaterial {
public:
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
	//テクスチャ
	CTexture mTexture;
	//マテリアル名
	char mName[MATERIAL_NAME_LEN + 1];
	//拡散光の色RGBA
	float mDiffuse[4];
};

#endif // !CMATERIAL_H

