// MFCGamePlugin.cpp: 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "MFCGamePlugin.h"
#include "CMyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CMFCGamePluginApp

BEGIN_MESSAGE_MAP(CMFCGamePluginApp, CWinApp)
END_MESSAGE_MAP()


// CMFCGamePluginApp 构造

CMFCGamePluginApp::CMFCGamePluginApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CMFCGamePluginApp 对象

CMFCGamePluginApp theApp;

WNDPROC g_oldProc;



LRESULT CALLBACK WindowProc(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{

	if (Msg == WM_DATA1)
	{
		OutputDebugString(L"无限指南针");
		
		//0041DE4D | .  8B86 9404000 > MOV EAX, DWORD PTR DS : [ESI + 0x494]
		//0041DE53 | .  8D8E 9404000 > LEA ECX, DWORD PTR DS : [ESI + 0x494]
		//0041DE59 | .  52           PUSH EDX
		//0041DE5A | .  53           PUSH EBX
		//0041DE5B | .  53           PUSH EBX
		//0041DE5C | .FF50 28      CALL DWORD PTR DS : [EAX + 0x28];  使用指南针道具
		_asm 
		{
			mov ecx, 0x45DEBC
			mov ecx, [ecx]
			LEA ECX, DWORD PTR DS : [ecx + 0x494]
			PUSH 0xF0// 若炸弹，则F4
			PUSH 0
			PUSH 0
			mov eax, 0x0041E691
			call eax
		}
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	else if (Msg == WM_DATA2)
	{
		// 1 获取两个点坐标
		POINT pt1 = { 0 };
		POINT pt2={ 0 };

		// 小技巧，用于调试，当注入成功时，ctrl+s 搜索指令找到此dll地址
		//_asm
		//{
		//	mov eax,eax
		//	mov eax,eax
		//}

		//0041E75E > \8B8E F0190000 MOV ECX, DWORD PTR DS : [ESI + 0x19F0];  Case F0(BM_GETCHECK) of switch 0041E749
		//0041E764   .  8D45 D8       LEA EAX, DWORD PTR SS : [EBP - 0x28]
		//0041E767   .  50            PUSH EAX
		//0041E768   .  8D45 E0       LEA EAX, DWORD PTR SS : [EBP - 0x20]
		//0041E76B   .  50            PUSH EAX
		//0041E76C.E8 CEAA0000   CALL kyodai2.0042923F;  提示待连接的两个坐标
		_asm
		{
			mov ecx, 0x45DEBC
			mov ecx, [ecx]
			lea ecx, DWORD PTR DS : [ecx + 0x494]// 要加上此，原程序中，此函数是在call指南针内部call的
			mov ecx, DWORD PTR DS : [ecx + 0x19F0]// 即在前面的基础上调用的，因此ecx...
			lea eax, pt1.x
			push eax// 原程序，push的是栈地址
			lea eax, pt2.x
			push eax
			mov eax,0x0042923F
			call eax
		}
		CString strCode;
		strCode.Format(L"单次消除: 点1 x=%d,y=%d,点2 x=%d,y=%d", pt1.x, pt1.y, pt2.x, pt2.y);
		OutputDebugString(strCode.GetBuffer());

		// 循环消除中，判断是否停止
		if (pt1.x == 0 && pt1.y == 0)
		{
			return -1;
		}

		// 2 调用消除call

		//0041AB13 | > \57            PUSH EDI;  参数6:2（当前edi = 2
		//0041AB14 | .  8D45 F4       LEA EAX, [LOCAL.3]
		//0041AB17 | .  53            PUSH EBX;  参数5：坐标数组（ = 1A5DE18 = ？ + ？
		//0041AB18 | .  50            PUSH EAX;  参数4：点2坐标（eax来自local3，就是点坐标
		//0041AB19 | .  8D45 EC       LEA EAX, [LOCAL.5]
		//0041AB1C | .  8BCE          MOV ECX, ESI
		//0041AB1E | .  50            PUSH EAX;  参数3：点1坐标（eax来自local5，就是点坐标
		//0041AB1F | .  0FB645 08     MOVZX EAX, BYTE PTR SS : [EBP + 0x8];  eax = 0
		//0041AB23 | .  69C0 DC000000 IMUL EAX, EAX, 0xDC;  eax = 0
		//0041AB29 | .  8D8430 5C1900 > LEA EAX, DWORD PTR DS : [EAX + ESI + 0x195C]
		//0041AB30 | .  50            PUSH EAX;  参数2：连连看数组地址（ = 12BB50 = ？ + ？
		//0041AB31 | .FF75 08       PUSH[ARG.1];  参数1：0（栈中可得，arg1为0
		//0041AB34 | .E8 551B0000   CALL kyodai2.0041C68E;  6个参数，相当靠谱，就是他了
		_asm
		{
			// 传递ecx，尤其重要，基地址！！
			mov ecx, 0x45DEBC
			mov ecx, [ecx]
			// 第一个参数 固定值
			push 0x4
			// 第二个参数 坐标点数组
			lea eax, DWORD PTR DS : [ecx + 0x494]
			mov eax, DWORD PTR DS : [eax + 0x19F0]
			add eax, 0x40
			push eax
			// 第三个参数 坐标1
			lea eax, pt1.x
			push eax
			// 第四个参数  坐标2
			lea eax, pt2.x
			push eax
			// 第五个参数 数组地址
			lea eax, DWORD PTR DS : [ecx + 0x494]
			mov eax, DWORD PTR DS : [eax + 0x19F0]
			mov eax, DWORD PTR DS : [eax + 4]
			push eax
			// 第六个参数 0
			push 0
			// 调用函数
			mov eax,0x0041C68E
			call eax
		}

		return DefWindowProc(hWnd, Msg, wParam, lParam);// 要加此，否则运行完自动结束
	}
	else if (Msg == WM_DATA3)
	{
	OutputDebugString(L"无限炸弹");



	//0041DE4D | .  8B86 9404000 > MOV EAX, DWORD PTR DS : [ESI + 0x494]
	//0041DE53 | .  8D8E 9404000 > LEA ECX, DWORD PTR DS : [ESI + 0x494]
	//0041DE59 | .  52           PUSH EDX
	//0041DE5A | .  53           PUSH EBX
	//0041DE5B | .  53           PUSH EBX
	//0041DE5C | .FF50 28      CALL DWORD PTR DS : [EAX + 0x28];  使用指南针道具
	_asm
	{
		mov ecx, 0x45DEBC
		mov ecx, [ecx]
		LEA ECX, DWORD PTR DS : [ecx + 0x494]
		PUSH 0xF4// 若指南针，则F0
		PUSH 0
		PUSH 0
		mov eax, 0x0041E691
		call eax
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return CallWindowProc(g_oldProc,hWnd,Msg,wParam,lParam);
}


// 线程回调函数
unsigned __stdcall ThreadProc()
{
	CMyDlg dlg;
	dlg.DoModal();
	return 0;
}

// CMFCGamePluginApp 初始化

BOOL CMFCGamePluginApp::InitInstance()
{
	CWinApp::InitInstance();
	// 1 获取窗口句柄
	m_hWnd = ::FindWindow(NULL, L"QQ连连看");
	if (m_hWnd == NULL)
	{
		OutputDebugString(L"找游戏窗口：失败");
		return FALSE;
	}
	// 2 设置窗口回调函数
	g_oldProc = (WNDPROC)SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)WindowProc);
	if (g_oldProc == NULL)
	{
		OutputDebugString(L"设置回调函数：失败");
		return FALSE;
	}
	// 3 弹出对话框
	_beginthreadex(0, 0, (_beginthreadex_proc_type)ThreadProc,0,0,0);
	return TRUE;
}
