#ifndef CMATRIX_H
#define CMATRIX_H
/*
}gNX
4s4ñÌsñf[^ðµ¢Ü·
*/

class CVector;
class CMatrix {
public:
	CVector VectorZ() const;  //Z²xNgÌæ¾
	CVector VectorX() const;  //X²xNgÌæ¾
	CVector VectorY() const;  //Y²xNgÌæ¾
	//tsñæ¾
	CMatrix Transpose() const;
	//sñÌæ¾
	float* M() const;
	//ZqÌI[o[[h
	//CMatrix * CMatrixÌZÊðÔ·
	const CMatrix operator*(const CMatrix& m)const;
	//sñlÌãü
	//M(s,ñ,l)
	void M(int row, int col, float value);
	//Ú®sñÌì¬
	//Translate(Ú®ÊX,Ú®ÊY,Ú®ÊZ)
	CMatrix Translate(float mx, float my, float mz);
	//ñ]sñ(X²)Ìì¬
	//RotateX(px)
	CMatrix RotateX(float degree);
	//ñ]sñ(Z²)Ìì¬
	//RotateZ(px)
	CMatrix RotateZ(float degree);
	//ñ]sñ
    //RotateY(px)
	CMatrix RotateY(float degree);
	//sñlÌæ¾
	//M(sAñ)
	//mM[s][ñ]ðæ¾
	float M(int r, int c) const;
	//gåk¬sñÌì¬
	//Scale({¦X,{¦Y,{¦Z)
	CMatrix Scale(float sx, float sy, float sz);
	//\¦mFp
	//S~SÌsñðæÊoÍ
	void Print();
	//ftHgRXgN^
	CMatrix();
	//PÊsñÌì¬
	CMatrix Identity();
private:
	//S~SÌsñf[^ðÝè
	float mM[4][4];


};


#endif // !CMATRIX_H

