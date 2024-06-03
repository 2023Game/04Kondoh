#ifndef CINPUT_H
#define CINPUT_H
#include <Windows.h>
#include "GLFW/glfw3.h"

class CInput
{
public:
	/*
	初期化
	Init(ウィンドウポインタ)
	*/
	static void Init(GLFWwindow* w);
	/*
	GetMousePos(X座標ポインタ,Y座標ポインタ)
	ウィンドウの左上を原点とした、
	マウスカーソルの座標を、引数に設定
	*/
	static void GetMousePos(int* px, int* py);

	/*
	マウスカーソルの座標を取得する
	GetMousePos(X座標ポインタ,Y座標ポインタ)
	ディスプレイの左上を原点(0,0)とした、
	マウスカーソルの座標を、引数に設定する
	画面右方向がX軸プラス
	画面下方向がY軸プラス
	*/
	static void GetMousePosD(int* px, int* py);

	/*
	マウスカーソルの座標を設定する
	SetCursorPosD(X座標,Y座標)
	ディスプレイの左上が原点(0,0)
	*/
	static void SetMousePosD(int x, int y);

	static void Window(GLFWwindow* pwindow);
	CInput();
	//bool Key(文字)
	//戻り値
	//true:文字のキーが押されている
	//false:文字のキーが押されていない
	bool Key(char key);
private:
	static GLFWwindow* spWindow;	//ウィンドウのポインタ
};


#endif // !CINPUT_H