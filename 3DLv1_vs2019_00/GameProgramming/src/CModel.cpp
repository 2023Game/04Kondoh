#include "CModel.h"
//
#include <stdio.h>

//
//
void CModel::Load(char* obj, char* mtl)
{
	//�t�@�C���|�C���^�ϐ��̍쐬
	FILE* fp;
	//�t�@�C������f�[�^���쐬����
	//���̓G���A���쐬����
	char buf[256];


	//�t�@�C���̃I�[�v��
	//fopen(�t�@�C����,���[�h)
	//�I�[�v���ł��Ȃ�����NULL��Ԃ�
	fp = fopen(mtl, "r");
	//�t�@�C���I�[�v���G���[�̔��f
	//fp��NULL�̎��̓G���[
	if (fp == NULL)
	{
		//�R���\�[���ɃG���[�o�͂��Ė߂�
		printf("%s fill open error\n", mtl);
		return;
	}

	//�t�@�C������P�s����
	//fgets(���̓G���A,�G���A�T�C�Y,�t�@�C���|�C���^)
	//�t�@�C���̍Ō�ɂȂ��NULL��Ԃ�
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		//���͂����l���R���\�[���ɏo�͂���
		printf("%s", buf);
	}

	fclose(fp);

	fp = fopen(obj, "r");
	if (fp == NULL)
	{
		printf("%s fill open error\n", obj);
		return;
	}
	fgets(buf, sizeof(buf), fp);
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		printf("%s", buf);
	}

	//�t�@�C���̃N���[�Y
	fclose(fp);
}