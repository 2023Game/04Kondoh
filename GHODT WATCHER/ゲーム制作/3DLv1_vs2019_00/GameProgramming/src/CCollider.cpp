#include "CCollider.h"
#include "CCollisionManager.h"
#include "CColliderLine.h"

CCollider::CCollider(CCharacter3* parent, CMatrix* matrix,
	const CVector& position, float radius) 
	:CCollider()
{
	//�e�ݒ�
	mpParent = parent;
	//�e�s��ݒ�
	mpMatrix = matrix;
	//CTransform�ݒ�
	mPosition = position; //�ʒu
	//���a�ݒ�
	mRadius = radius;
}

CCharacter3* CCollider::Parent()
{
	return mpParent;
}

void CCollider::Render() {
	glPushMatrix();
	//�R���C�_�̒��S���W���v�Z
	//�����̍��W�~�e�̕ϊ��s����|����
	CVector pos = mPosition * *mpMatrix;
	//���S���W�ֈړ�
	glMultMatrixf(CMatrix().Translate(pos.X(), pos.Y(), pos.Z()).M());
	//DIFFUSE�ԐF�ݒ�
	float c[] = { 1.0f,0.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//���`��
	glutWireSphere(mRadius, 16, 16);
	glPopMatrix();
}

CCollider::CCollider()
	:mpParent(nullptr)
	,mpMatrix(&mMatrix)
	,mType(EType::ESPHERE)
	,mRadius(0)
{
	//�R���W�����}�l�[�W���ɒǉ�
	CCollisionManager::Instance()->Add(this);
}

CCollider::~CCollider() {
	//�R���W�������X�g����폜
	CCollisionManager::Instance()->Remove(this);
}

//�Փ˔���
//CCollision(�R���C�_�P,�R���C�_�Q)
//return:true(�Փ˂��Ă���),false(�Փ˂��Ă��Ȃ�)
bool CCollider::Collision(CCollider* m, CCollider* o) {
	//�e�R���C�_�̒��S���W�����߂�
	//���_�~�R���C�_�̕ϊ��s��~�e�̕ϊ��s��
	CVector mpos = m->mPosition * *m->mpMatrix;
	CVector opos = o->mPosition * *o->mpMatrix;
	//���S���璆�S�ւ̃x�N�g�������߂�
	mpos = mpos - opos;
	//���S�̋��������a�̍��v��菬�����ƏՓ�
	if (m->mRadius + o->mRadius > mpos.Length()) {
		//�Փ˂��Ă���
		return true;
	}
	//�Փ˂��Ă��Ȃ�
	return false;
}

bool CCollider::CollisionTriangleLine(CCollider* t, CCollider* l, CVector* a) {
	CVector v[3], sv, ev;
	//�e�R���C�_�̒��_�����[���h���W�֕ϊ�
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	sv = l->mV[0] * *l->mpMatrix;
	ev = l->mV[1] * *l->mpMatrix;
	//�ʂ̖@�����A�O�ς𐳋K�����ċ��߂�
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Normalize();
	//�O�p�̒��_��������n�_�ւ̃x�N�g�������߂�
	CVector v0sv = sv - v[0];
	//�O�p�`�̒��_��������I�_�ւ̃x�N�g�������߂�
	CVector v0ev = ev - v[0];

	//�������ʂƌ������Ă��邩���ςŊm�F����
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//�v���X�͌������ĂȂ�
	if (dots * dote >= 0.0f) {
		//�Փ˂��Ă��Ȃ�(�����s�v)
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//�����͖ʂƌ������Ă���
	

	//�ʂƐ����̌�_�����߂�
	//��_�̌v�Z
	CVector cross = sv + (ev - sv) * (abs(dots) / (abs(dots) + abs(dote)));

	//��_���O�p�`���Ȃ�Փ˂��Ă���
	//���_�P���_�Q�x�N�g���ƒ��_�P��_�x�N�g���Ƃ̊O�ς����߁A
	//�@���Ƃ̓��ς��}�C�i�X�Ȃ�A�O�p�`�̊O
	if ((v[1] - v[0]).Cross(cross - v[0]).Dot(normal) < 0.0f) {
		//�Փ˂��Ȃ�
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//���_�Q���_�R�x�N�g���ƒ��_�Q��_�x�N�g���Ƃ̊O�ς����߁A
	//�@���Ƃ̓��ς��}�C�i�X�Ȃ�A�O�p�`�O
	if ((v[2] - v[1]).Cross(cross - v[1]).Dot(normal) < 0.0f) {
		//
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//���_�R���_�P�x�N�g���ƒ��_�R��_�x�N�g���Ƃ̊O�ς����߁A
	//�@���Ƃ̓��ς��}�C�i�X�Ȃ�A�O�p�`�̊O
	if ((v[0] - v[2]).Cross(cross - v[2]).Dot(normal) < 0.0f) {
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	 
	//�����l�v�Z(�Փ˂��Ȃ��ʒu�܂Ŗ߂�)
	if (dots < 0.0f) {
		//�n�_������
		*a = normal * -dots;
	}
	else {
		//�I�_������
		*a = normal * -dote;
	}
	return true;
}

//CollisionTriangleSphere(�O�p�R���C�_,���R���C�_,�����l)
//retrun:true(�Փ˂��Ă���)false(�Փ˂��Ă��Ȃ�)
//�����l:�Փ˂��Ă��Ȃ��ʒu�܂Ŗ߂�
bool CCollider::CollisionTriangleSphere(CCollider* t, CCollider* s, CVector* a)
{
	CVector v[3], sv, ev;
	//�e�R���C�_�̒��_�����[���h���W�ɕϊ�
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	//�ʂ̖@�����A�O�ς𐳋K�����ċ��߂�
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Normalize();
	//�O�p�R���C�_�Ɛ��R���C�_�̏Փˏ���
	sv = s->mPosition * *s->mpMatrix + normal * s->mRadius;
	ev = s->mPosition * *s->mpMatrix - normal * s->mRadius;

	//�ʂ̖@�����A�O�ς𐳋K�����ċ��߂�
	//�O�p�̒��_��������n�_�ւ̃x�N�g�������߂�
	CVector v0sv = sv - v[0];
	//�O�p�`�̒��_��������I�_�ւ̃x�N�g�������߂�
	CVector v0ev = ev - v[0];

	//�������ʂƌ������Ă��邩���ςŊm�F����
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//�v���X�͌������ĂȂ�
	if (dots * dote >= 0.0f) {
		//�Փ˂��Ă��Ȃ�(�����s�v)
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//�����͖ʂƌ������Ă���


	//�ʂƐ����̌�_�����߂�
	//��_�̌v�Z
	CVector cross = sv + (ev - sv) * (abs(dots) / (abs(dots) + abs(dote)));

	//��_���O�p�`���Ȃ�Փ˂��Ă���
	//���_�P���_�Q�x�N�g���ƒ��_�P��_�x�N�g���Ƃ̊O�ς����߁A
	//�@���Ƃ̓��ς��}�C�i�X�Ȃ�A�O�p�`�̊O
	if ((v[1] - v[0]).Cross(cross - v[0]).Dot(normal) < 0.0f) {
		//�Փ˂��Ȃ�
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//���_�Q���_�R�x�N�g���ƒ��_�Q��_�x�N�g���Ƃ̊O�ς����߁A
	//�@���Ƃ̓��ς��}�C�i�X�Ȃ�A�O�p�`�O
	if ((v[2] - v[1]).Cross(cross - v[1]).Dot(normal) < 0.0f) {
		//
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//���_�R���_�P�x�N�g���ƒ��_�R��_�x�N�g���Ƃ̊O�ς����߁A
	//�@���Ƃ̓��ς��}�C�i�X�Ȃ�A�O�p�`�̊O
	if ((v[0] - v[2]).Cross(cross - v[2]).Dot(normal) < 0.0f) {
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}


	//�����l�v�Z(�Փ˂��Ȃ��ʒu�܂Ŗ߂�)
	if (dots < 0.0f) {
		//�n�_������
		*a = normal * -dots;
	}
	else {
		//�I�_������
		*a = normal * -dote;
	}
	return true;

	//CColliderLine line(nullptr, nullptr, sv, ev);
	////
	//return CollisionTriangleLine(t, &line, a);
}

//CalcCalcPointLineDist(�_,�n�_,�I�_,����̍ŒZ�_,����)
	//�_�Ɛ�(�n�_�A�I�_��ʂ钼��)�̍ŒZ���������߂�
float CalcPointLineDist(const CVector& p, const CVector& s, const CVector e, CVector* mp, float* t) 
{

	*t = 0.0f;  //�����̏�����
	CVector v = e - s;  //�n�_����I�_�ւ̃x�N�g�������߂�
	float dvv = v.Dot(v);  //�x�N�g���̒����̂Q������߂�
	if (dvv > 0.0f) {
		*t = v.Dot(p - s) / dvv;  //����̐����ƂȂ銄�������߂�
		//��̎��̐���
		//dot(v,p-sp)��|v| |p-sp|con��
		//dvv��|v��2��|
		//��̌v�Z�ŁAt��|p-sp|cos�� / |v|�ƂȂ�
		//�܂�t�́udot�œ��e����������v�̒����v�Ƃ��������ɂȂ�
	}
	*mp = s + v * *t;  //����̐����ƂȂ�_�����߂�
	return (p - *mp).Length();  //�����̒�����Ԃ�

}

bool CCollider::CollisionSphereLine(CCollider* s, CCollider* l, CVector* a) 
{
	CVector v[1], sv, ev;
	CVector mp;
	float t;
	v[0] = s->mV[0] * *s->mpMatrix;
	sv = l->mV[0] * *l->mpMatrix;
	ev = l->mV[1] * *l->mpMatrix;

	float dist = CalcPointLineDist(v[0], sv, ev, &mp, &t);

	if (s->mRadius > dist && 0.0f <= t && t <= 1.0f) {
		return true;
	}
	return false;
}

CCollider::EType CCollider::Type()
{
	return mType;
}


void CCollider::ChangePriority(int priority)
{
	mPriority = priority;
	CCollisionManager::Instance()->Remove(this); //��U�폜
	CCollisionManager::Instance()->Add(this); //�ǉ�
}

//�D��x�̕ύX
void CCollider::ChangePriority() 
{
	//�����̍��W�~�e�̕ϊ��s����|���ă��[���h���W�����߂�
	CVector pos = mPosition * *mpMatrix;
	//�x�N�g���̒������D��x
	CCollider::ChangePriority(pos.Length());
}
