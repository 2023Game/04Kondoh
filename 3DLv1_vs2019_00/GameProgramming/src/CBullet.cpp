#include "CBullet.h"

//���Ɖ��s���̐ݒ�
//Set(��,���s��)
void CBullet::Set(float w, float d) {
	//�X�P�[���ݒ�
	mScale = CVector(1.0f, 1.0f, 1.0f);
	//�O�p�`�̒��_�ݒ�
	mT.Vertex(CVector(-w, 0.0f, 0.0f), CVector(w, 0.0f, 0.0f), CVector(0.0f, 0.0f, -d));
	//�O�p�`�̖@���ݒ�
	mT.Normal(CVector(0.0f, 1.0f, 0.0f));
}

//�X�V
void CBullet::Update() {
	CTransform::Update();
	if (mLife-- > 0) {
		CTransform::Update();
		//�ʒu�X�V�@�i�s������1�i��
		mPosition = mPosition + CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;
	}
	else {
		//�����ɂ���
		mEnabled = false;
	}
}

//�`��
void CBullet::Render() {
	mCollider.Render();
	//DIFFUSE���F�ݒ�
	float c[] = { 1.0f,1.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//�O�p�`�`��
	mT.Render(mMatrix);
	//CTransform::Update()
}

CBullet::CBullet()
	:mLife(50)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.1f)
{}