#ifndef CVECTOR_H
#define CVECTOR_H
/*
ベクトルクラス
ベクトルデータを扱います
*/
#include "CMatrix.h"

class CVector {
public:
	//正規化
	//大きさ１ベクトルを返す
	CVector Normalize() const;
	//外積
	//Cross
	CVector Cross(const CVector& v)const;
	//*演算子のオーバーロード
	//CVector * float の演算結果を返す
	CVector operator*(const float& f)const;
	//内積
	//Dot(ベクトル)
	float Dot(const CVector& v) const;
	//ベクトルの長さを返す
	float Length() const;
	//CVector * CMatrixの結果をCVectorで返す
	CVector operator*(const CMatrix& m);

	//+演算子のオーバーロード
	//CVector + CVector の演算結果を返す
	CVector operator+(const CVector& v) const;

	//-演算子のオーバーロード
	//CVector - CVector の演算結果を返す
	CVector operator-(const CVector& v) const;

	//デフォルトコンストラクタ
	CVector();
	//コンストラクタ
	//CVector(X座標,Y座標,Z座標)
	CVector(float x, float y, float z);
	//各軸での値の設定
	//Set(X座標,Y座標,Z座標)
	void Set(float x, float y, float z);
	//Xの値を得る
	float X() const;
	//Yの値を得る
	float Y() const;
	//Zの値を得る
	float Z() const;

private:
	//3D各軸での値を設定
	float mX, mY, mZ;

};

#endif // !CVECTOR_H

