#ifndef CCHARACTER3_H
#define CCHARACTER3_H
//�ϐ��s��N���X�̃C���N���[�h
#include "CTransform.h"
//���f���N���X�̃C���N���[�h
#include "CModel.h"
/*
�L�����N�^�[�N���X
�Q�[���L�����N�^�̊�{�I�ȋ@�\���`����
*/

class CCharacter3 : public CTransform {
public:
	//���f���ݒ�
	//Model(���f���N���X�̃|�C���g)
	void Model(CModel* m);
	//�`�揈��
	void Render();
protected:
	CModel* mpModel; //���f���̃|�C���^

};

#endif // !CCHARACTER3_H

