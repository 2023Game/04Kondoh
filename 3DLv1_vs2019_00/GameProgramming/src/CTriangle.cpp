#include "CTriangle.h"
#include "glut.h"

//���_���W�ݒ�
//Vertex(���_�P,���_�Q,���_�R)
void CTriangle::Vertex(const CVector &v0, const CVector &v1, const CVector &v2) {
	mV[0] = v0;
	mV[1] = v1;
	mV[2] = v2;
}

//�@���ݒ�
//Normal(�@���x�N�g��)
void CTriangle::Normal(const CVector &n) {
	mN = n;
}

//�`��
void CTriangle::Render() {
	glBegin(GL_TRIANGLES);
	glNormal3f(mN.X(), mN.Y(), mN.Z());
	glVertex3f(mV[0].X(), mV[0].Y(), mV[0].Z());
	glVertex3f(mV[1].X(), mV[1].Y(), mV[1].Z());
	glVertex3f(mV[2].X(), mV[2].Y(), mV[2].Z());
	glEnd();
}