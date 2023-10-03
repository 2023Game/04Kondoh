#ifndef CMODEL_H
#define CMODEL_H
/*
モデルクラス
モデルデータの入力や表示
*/
//vectorのインクルード
#include <vector>
#include "CTriangle.h"

class CModel {
public:
	//モデルファイルの入力
	//Load(モデルファイル名、マテリアルファイル名)
	void Load(char* obj, char* mtl);
	//描画
	void Render();

private:
	//三角形の可変長配列
	std::vector<CTriangle> mTriangles;
};

#endif // !CMODEL_H

