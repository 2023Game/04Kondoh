#ifndef CTRIANGLE_H
#define CTRIANGLE_H

 #include "CVector.h"
/*
�O�p�`�N���X
*/
class CTriangle {
public:
	//�`��
	//Render(�s��)
	void Render(const CMatrix& m);
	//UV�ݒ�
	void UV(const CVector& v0, const CVector& v1, const CVector& v2);
	//�}�e���A���ԍ��̎擾
	int MaterialIdx();
	//�}�e���A���ԍ��̐ݒ�
	//Material(�}�e���A���ԍ�)
	void MaterialIdx(int idx);
	//���_���W�ݒ�
	//Vertex(���_�P,���_�Q,���_�R)
	void Vertex(const CVector &v0, const CVector &v1, const CVector &v2);
	//�@���ݒ�
	//Normal(�@���x�N�g��)
	void Normal(const CVector &n);
	//�`��
	void Render();
	//Normal(�@���x�N�g���P,�@���x�N�g���Q,�@���x�N�g���R)
	void Normal(const CVector& v0, const CVector& v1, const CVector& v2);

private:
	CVector mUv[3]; //�e�N�X�`���}�b�s���O
	int mMaterialIdx;
	CVector mV[3]; //���_���W
	CVector mN[3]; //�@��
}; 

#endif // !CTRIANGLE_H
