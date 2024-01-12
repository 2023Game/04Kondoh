#ifndef CCOLLIDERMESH_H
#define CCOLLIDERMESH_H
#include "CModel.h"
#include "CColliderTriangle.h"
/*
モデルデータから三角コライダの生成
*/

class CColliderMesh
{
public:
	//優先度の変更
	void ChangePriority();
	CColliderMesh();
	~CColliderMesh();
	//Set(親,親行列,モデル)
	//モデルから三角コライダの生成
	void Set(CCharacter3* parent, CMatrix* matrix, CModel* model);

private:
	//三角コライダの配列作成
	CColliderTriangle* mpColliderTriangles;
};

#endif // !CCOLLIDERMESH_H

