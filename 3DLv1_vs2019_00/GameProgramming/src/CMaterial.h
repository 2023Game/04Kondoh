#ifndef CMATERIAL_H
#define CMATERIAL_H
#define MATERIAL_NAME_LEN 64 //���O�̒���

#include "CTexture.h"

/*
�}�e���A���N���X
�}�e���A���̃f�[�^������
*/

class CMaterial {
public:
	//�e�N�X�`���擾
	CTexture* Texture();
	//�}�e���A���𖳌��ɂ���
	void Disabled();
	//�f�t�H���g�R���X�g���N�^
	CMaterial();
	//�}�A�e���A����L���ɂ���
	void Enabled();
	//�}�e���A���̖��O���擾
	char* Name();
	//�}�e���A���̖��O��ݒ�
	//Name(�܂����A���̖��O)
	void Name(char* name);
	//mDiffuse�z��̎擾
	float* Diffuse();

private:
	//�e�N�X�`��
	CTexture mTexture;
	//�}�e���A����
	char mName[MATERIAL_NAME_LEN + 1];
	//�g�U���̐FRGBA
	float mDiffuse[4];
};

#endif // !CMATERIAL_H
