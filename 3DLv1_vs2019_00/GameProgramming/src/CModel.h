#ifndef CMODEL_H
#define CMODEL_H
/*
���f���N���X
���f���f�[�^�̓��͂�\��
*/
//vector�̃C���N���[�h
#include <vector>
#include "CTriangle.h"

class CModel {
public:
	//���f���t�@�C���̓���
	//Load(���f���t�@�C�����A�}�e���A���t�@�C����)
	void Load(char* obj, char* mtl);
	//�`��
	void Render();

private:
	//�O�p�`�̉ϒ��z��
	std::vector<CTriangle> mTriangles;
};

#endif // !CMODEL_H

