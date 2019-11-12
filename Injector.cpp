#include <iostream>
#include <windows.h>
using namespace std;
//要加载的dll路径
WCHAR szDllPath[] = L"C:\\Users\\15pb-win7\\Desktop\\MFCGamePlugin.dll";

int main()
{
	//1.要注入，需要dll文件
	//2.找到要注入的进程PID
	DWORD dwPid=0;
	//HWND hwnd = FindWindow(NULL, L"new 1 - Notepad++");
	//GetWindowThreadProcessId(hwnd, &dwPid);
	printf("please input PID>> ");
	scanf_s("%d", &dwPid);
	//3.打开进程，获取进程句柄
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	//4.在目标进程中申请空间
	LPVOID pBuff = VirtualAllocEx(
		hProcess,
		0,
		sizeof(szDllPath),
		MEM_RESERVE | MEM_COMMIT,
		PAGE_EXECUTE_READWRITE
	);
	//5.将路径写入到目标进程中
	DWORD dwSize;
	WriteProcessMemory(
		hProcess,
		pBuff,			//在指申请的地址上
		szDllPath,		//写入的内容
		sizeof(szDllPath),//写入大小
		&dwSize
	);
	//6.使用关键函数加载目标dll
	// 利用远程创建线程函数，实现目标进程加载dll
	// 远程线程执行函数直接指向LoadLibaray函数，同时参数指向dll路径，完美实现加载dll
	HANDLE hThread = CreateRemoteThread(
		hProcess,
		NULL,
		NULL,
		(LPTHREAD_START_ROUTINE)LoadLibrary,		//线程执行地址指向LoadLibrary
		pBuff,										//线程的附加参数dll路径
		NULL, NULL
	);
	//7 释放句柄
	CloseHandle(hProcess);
	CloseHandle(hThread);

}