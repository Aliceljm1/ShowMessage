﻿// WindowsProject1.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "WindowsProject1.h"
#include <string>
#include <queue>

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
#define MAX_MESSAGES 30
std::queue<std::wstring> messagesQueue; // 使用队列来存储消息


// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	// 注册窗口以接收触控消息
	if (!RegisterTouchWindow(hWnd, 0))
	{
		// 注册失败时的处理
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//

int msgIndex = 0;

void addMessage(std::wstring message)
{
	//给消息追加时间，以便区分不同的消息
	SYSTEMTIME st;
	GetLocalTime(&st);
	int index = msgIndex++;
	//message增加序号
	message= std::to_wstring(index)+L":" + message;
	message += L" " + std::to_wstring(st.wHour) + L":" + std::to_wstring(st.wMinute) + L":" + std::to_wstring(st.wSecond) + L":" + std::to_wstring(st.wMilliseconds) + L"\n";
	messagesQueue.push(message);
	//如果消息数量超过最大值，则删除最早的消息
	if (messagesQueue.size() > MAX_MESSAGES)
		messagesQueue.pop();
}

void cleanMessage()
{
	// 清空消息队列
	std::queue<std::wstring> empty;
	std::swap(messagesQueue, empty);
}

void findWndSendClickEvent() 
{
	// 定义点坐标
	POINT point;
	point.x = 500; // x坐标
	point.y = 500; // y坐标

	// 获取包含该点的窗口句柄
	HWND hwnd = WindowFromPoint(point);

	if (hwnd != NULL) {
		// 向该窗口发送鼠标左键按下消息
		SendMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(point.x, point.y));
		Sleep(100); // 等待100毫秒
		// 向该窗口发送鼠标左键抬起消息
		SendMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(point.x, point.y));
	}
	else {
		MessageBoxA(NULL, "未找到窗口", "提示", MB_OK);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static std::wstring messages; // 用于存储消息字符串
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			cleanMessage();
			InvalidateRect(hWnd, NULL, TRUE); // 请求重绘窗口	
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_32771:
			findWndSendClickEvent();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_KEYDOWN:
		if (wParam == VK_F1)
		{
			cleanMessage();
			InvalidateRect(hWnd, NULL, TRUE); // 请求重绘窗口	
		}
		break;
	case WM_LBUTTONDOWN:
		messages = L"WM_LBUTTONDOWN";
		addMessage(messages);
		InvalidateRect(hWnd, NULL, TRUE); // 请求重绘窗口
		break;
	case WM_LBUTTONUP:
		messages = L"WM_LBUTTONUP";
		addMessage(messages);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_MOUSEMOVE:
		messages = L"WM_MOUSEMOVE";
		addMessage(messages);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_TOUCH:
	{
		UINT cInputs = LOWORD(wParam); // 触控点的数量
		PTOUCHINPUT pInputs = new TOUCHINPUT[cInputs];
		if (pInputs)
		{
			if (GetTouchInputInfo((HTOUCHINPUT)lParam, cInputs, pInputs, sizeof(TOUCHINPUT)))
			{
				for (UINT i = 0; i < cInputs; ++i)
				{
					TOUCHINPUT ti = pInputs[i];
					if (ti.dwFlags & TOUCHEVENTF_DOWN)
						messages = L"TOUCHEVENTF_DOWN";
					if (ti.dwFlags & TOUCHEVENTF_MOVE)
						messages = L"TOUCHEVENTF_MOVE";
					if (ti.dwFlags & TOUCHEVENTF_UP)
						messages = L"TOUCHEVENTF_UP";
				}
			}
			delete[] pInputs;
		}
		addMessage(messages);
		InvalidateRect(hWnd, NULL, TRUE); // 请求重绘窗口
	
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		std::wstring showmsg;
		//循环打印messagesQueue中的消息
		std::queue<std::wstring> tempQueue = messagesQueue;
		while (!tempQueue.empty())
		{
			showmsg += tempQueue.front();
			tempQueue.pop();
		}
		DrawText(hdc, showmsg.c_str(), -1, &ps.rcPaint, DT_LEFT);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
