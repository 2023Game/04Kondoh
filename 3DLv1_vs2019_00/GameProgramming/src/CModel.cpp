#include "CModel.h"
//
#include <stdio.h>

//
//
void CModel::Load(char* obj, char* mtl)
{
	//ファイルポインタ変数の作成
	FILE* fp;
	//ファイルからデータを作成する
	//入力エリアを作成する
	char buf[256];


	//ファイルのオープン
	//fopen(ファイル名,モード)
	//オープンできない時はNULLを返す
	fp = fopen(mtl, "r");
	//ファイルオープンエラーの判断
	//fpがNULLの時はエラー
	if (fp == NULL)
	{
		//コンソールにエラー出力して戻る
		printf("%s fill open error\n", mtl);
		return;
	}

	//ファイルから１行入力
	//fgets(入力エリア,エリアサイズ,ファイルポインタ)
	//ファイルの最後になるとNULLを返す
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		//入力した値をコンソールに出力する
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

	//ファイルのクローズ
	fclose(fp);
}