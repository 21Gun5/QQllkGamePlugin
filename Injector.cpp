#include <iostream>
#include <windows.h>
using namespace std;
//Ҫ���ص�dll·��
WCHAR szDllPath[] = L"C:\\Users\\15pb-win7\\Desktop\\MFCGamePlugin.dll";

int main()
{
	//1.Ҫע�룬��Ҫdll�ļ�
	//2.�ҵ�Ҫע��Ľ���PID
	DWORD dwPid=0;
	//HWND hwnd = FindWindow(NULL, L"new 1 - Notepad++");
	//GetWindowThreadProcessId(hwnd, &dwPid);
	printf("please input PID>> ");
	scanf_s("%d", &dwPid);
	//3.�򿪽��̣���ȡ���̾��
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	//4.��Ŀ�����������ռ�
	LPVOID pBuff = VirtualAllocEx(
		hProcess,
		0,
		sizeof(szDllPath),
		MEM_RESERVE | MEM_COMMIT,
		PAGE_EXECUTE_READWRITE
	);
	//5.��·��д�뵽Ŀ�������
	DWORD dwSize;
	WriteProcessMemory(
		hProcess,
		pBuff,			//��ָ����ĵ�ַ��
		szDllPath,		//д�������
		sizeof(szDllPath),//д���С
		&dwSize
	);
	//6.ʹ�ùؼ���������Ŀ��dll
	// ����Զ�̴����̺߳�����ʵ��Ŀ����̼���dll
	// Զ���߳�ִ�к���ֱ��ָ��LoadLibaray������ͬʱ����ָ��dll·��������ʵ�ּ���dll
	HANDLE hThread = CreateRemoteThread(
		hProcess,
		NULL,
		NULL,
		(LPTHREAD_START_ROUTINE)LoadLibrary,		//�߳�ִ�е�ַָ��LoadLibrary
		pBuff,										//�̵߳ĸ��Ӳ���dll·��
		NULL, NULL
	);
	//7 �ͷž��
	CloseHandle(hProcess);
	CloseHandle(hThread);

}