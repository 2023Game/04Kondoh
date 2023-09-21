#include "CVector.h"


//Set(XÀ•W,YÀ•W,ZÀ•W)
void CVector::Set(float x, float y, float z)
{
	mX = x;
	mY = y;
	mZ = z;
}

float CVector::X() const
{
	return mX;
}
float CVector::Y() const
{
	return mY;
}
float CVector::Z() const
{
	return mZ;
}