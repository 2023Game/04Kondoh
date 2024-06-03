#include "CInput.h"
#include <stdio.h>
#include <Windows.h>

GLFWwindow* CInput::spWindow = nullptr;	//ウィンドウのポインタ

void CInput::Init(GLFWwindow* w) {
	spWindow = w;
}

void CInput::Window(GLFWwindow* pwindow)
{
	spWindow = pwindow;
}

void CInput::GetMousePos(int* px, int* py)
{
	double xpos, ypos;
	glfwGetCursorPos(spWindow, &xpos, &ypos);
	*px = xpos;
	*py = ypos;
	return;
}

void CInput::GetMousePosD(int* px, int* py)
{
	POINT point;
	//WindowsAPI
	GetCursorPos(&point);
	*px = point.x;
	*py = point.y;
}

void CInput::SetMousePosD(int x, int y)
{
	//WindowsAPI
	SetCursorPos(x, y);
}

CInput::CInput()
{
	printf("入力インスタンスが生まれました\n");
}

bool CInput::Key(char key)
{
	return GetAsyncKeyState(key) < 0;
}
