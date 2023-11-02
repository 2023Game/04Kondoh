#ifndef CMODEL_H
#define CMODEL_H
/*
���f���N���X
���f���f�[�^�̓��͂�\��
*/
//vector�̃C���N���[�h
#include <vector>
#include "CTriangle.h"
#include "CMaterial.h"
#include "CVertex.h"

class CModel {
public:
	//�`��
	//Render(�s��)
	void Render(const CMatrix &m);
	~CModel();
	//���f���t�@�C���̓���
	//Load(���f���t�@�C�����A�}�e���A���t�@�C����)
	void Load(char* obj, char* mtl);
	//�`��
	void Render();

private:
	//���_�̔z��
	CVertex* mpVertexes;
	void CreateVertexBuffer();
	//�O�p�`�̉ϒ��z��
	std::vector<CTriangle> mTriangles;
	//�}�e���A���|�C���^�̉ϒ��z��
	std::vector<CMaterial*> mpMaterials;
};

#endif // !CMODEL_H

