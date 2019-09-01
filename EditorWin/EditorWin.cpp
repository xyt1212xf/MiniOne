// EditorWin.cpp : 锟斤拷锟斤拷应锟矫筹拷锟斤拷锟斤拷锟节点。
//

#include "stdafx.h"
#include "EditorWin.h"
#include "GuiPlugin.h"
#include "SceneMenuUI.h"
#include "ToolWnd.h"
#include "LeftWnd.h"
#include "RightWnd.h"
#include "BottomWnd.h"
#include "ResourceMenu.h"
#include "FileMenu.h"
#include "EditMenu.h"

#include <windows.h>

HINSTANCE hInst;                                // 
std::string szWindowClass = "MiniOneEngine";            // the main window class name
std::string szTitle = "MiniOneEditor";
const INT32 WINDOW_WIDTH = 1280;
const INT32 WINDOW_HEIGHT = 768;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		bool bRunResult = (CResourceMenu::wndProcess(hWnd, message, wParam, lParam) | CFileMenu::wndProcess(hWnd, message, wParam, lParam) |
						   CEditMenu::wndProcess(hWnd, message, wParam, lParam));
		if (!bRunResult)
		{	
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	}
	break;
	case WM_CONTEXTMENU:
	{
		RECT rect;
		POINT pt;
		// 获取鼠标右击是的坐标  
		pt.x = (short)LOWORD(lParam);
		pt.y = (short)HIWORD(lParam);
		//获取客户区域大小  
		GetClientRect((HWND)wParam, &rect);
		//把屏幕坐标转为客户区坐标  
		ScreenToClient((HWND)wParam, &pt);
		//判断点是否位于客户区域内  
		//if (PtInRect(&rect, pt))
		if (false)
		{
			//加载菜单资源  
			HMENU hroot = LoadMenu((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDR_RESMENU));
			if (hroot)
			{
				// 获取第一个弹出菜单  
				HMENU hpop = GetSubMenu(hroot, 0);
				// 把客户区坐标还原为屏幕坐标  
				ClientToScreen((HWND)wParam, &pt);
				//显示快捷菜单  
				TrackPopupMenu(hpop,
					TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON,
					pt.x,
					pt.y,
					0,
					(HWND)wParam,
					NULL);
				// 用完后要销毁菜单资源  
				DestroyMenu(hroot);
			}
		}
		else
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return	mini::CEngine::engineMsgProcess(hWnd, message, wParam, lParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance, DWORD nIcon, DWORD nStyle)
{
	WNDCLASSEXA wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(nIcon));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_EDITORWIN);
	wcex.lpszClassName = szWindowClass.c_str();
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(nIcon));

	return RegisterClassExA(&wcex);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	RECT rc;
	SetRect(&rc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW&(~WS_MAXIMIZEBOX)&(~WS_SIZEBOX), TRUE);
	MyRegisterClass(hInstance, IDI_EDITORWIN, CS_HREDRAW | CS_VREDRAW);
	HWND hWnd = CreateWindow(szWindowClass.c_str(), szTitle.c_str(), WS_OVERLAPPEDWINDOW&(~WS_SIZEBOX),
		(GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH) / 2, (GetSystemMetrics(SM_CYSCREEN) - WINDOW_HEIGHT) / 2, 
		(rc.right - rc.left), (rc.bottom - rc.top),nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, TRUE);
	UpdateWindow(hWnd);

	mini::CEngine* pEngine = mini::createEngine(hWnd);
	if (pEngine != nullptr)
	{
		pEngine->onStart();
		mini::ui::CGuiPlugin* pGuiPlguin = pEngine->getPlugin<mini::ui::CGuiPlugin>(mini::PluginType::GUI);
		
		/*
		CSceneMenuUI sceneMenu;
		pGuiPlguin->initWnd(&sceneMenu, "resource/Editor/Gui/SceneMenu.xml");
		pGuiPlguin->addWnd(&sceneMenu);
		sceneMenu.bind();
		*/
		mini::GEngine->stopRender();
		

		CLeftWnd leftWnd;
		leftWnd.initWnd();

		CRightWnd rightWnd;
		rightWnd.initWnd();

		CBottomWnd bottomWnd;
		bottomWnd.initWnd();


		CToolWnd toolWnd;
		toolWnd.initWnd();

		mini::GEngine->recoverRender();

		pEngine->onRun();
		
	}
	return 0;// (int)msg.wParam;
}