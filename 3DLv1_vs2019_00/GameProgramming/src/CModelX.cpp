#include <stdio.h>
#include <string.h>
#include "CModelX.h"
#include "glut.h"
#include "cmodelx.h"

CModelX::CModelX()
	:mpPointer(nullptr)
{
	//mTokenを初期化
	memset(mToken, 0, sizeof(mToken));
}

void CModelX::Load(char* file) {
	//
	//ファイルサイズを取得する
	//
	FILE* fp;      //ファイルポインタ変数の作成
	fp = fopen(file, "rb"); //ファイルをオープン
	if (fp == NULL) {       //エラーチェック
		printf("fopen error:%s\n", file);
		return;
	}
	//ファイルの最後へ移動
	fseek(fp, 0L, SEEK_END);
	//ファイルサイズの取得
	int size = ftell(fp);
	//ファイルサイズ＋１バイト分の領域を確保
	char* buf = mpPointer = new char[size + 1];
	//
	//ファイルから3Dモデルのデータを読み込む
	//
	//ファイルの先頭へ移動
	fseek(fp, 0L, SEEK_SET);
	//確保した領域にファイルサイズ分データを読み込む
	fread(buf, size, 1, fp);
	//最後に¥0を設定する(文字列の終端)
	buf[size] = '\0';
	printf("%s\n", mpPointer);

	fclose(fp);    //ファイルをクローズする

	SAFE_DELETE_ARRAY(buf);   //確保した領域を開放する

}

bool CModelX::IsDelimiter(char c)
{
	return false;
}

/*
GetToken
文字列データから、単語を１つ取得する
*/
char* CModelX::GetToken() {
	char* p = mpPointer;
	char* q = mToken;
	//タブ(\t)空白()改行(\r)(\n),;"の区切り文字以外になるまで読み飛ばす
	while (*p != '\0' && IsDelimiter(*p)) p++;
	if (*p == '{' || *p == '}') {
		//{または}ならmTokenに代入し次の文字へ
		*q++ = *p++;
	}
	else
	{
		//タブ(\t)空白()改行(\r)(\n),;"の区切り文字
		//または、}の文字になるまでmTokenに代入する
		while (*p != '\0' && !IsDelimiter(*p) && *p != '}')
			*q++ = *p++;
	}
	
	*q = '\0';      //mTokenの最後に\0を代入
	mpPointer = p;  //次の読み込むポイントを更新する

	//もしmTokenが//の場合は、コメントなので改行まで読み飛ばす
	/*
	strcmp(文字列1,文字列2)
	
	
	*/
	if (!strcmp("//", mToken)) {
		//
		while (*p != '\0' && !strchr("\r\n", *p)) p++;
		//
		mpPointer = p;
		//
		return GetToken();
	}
	return mToken;
 }
