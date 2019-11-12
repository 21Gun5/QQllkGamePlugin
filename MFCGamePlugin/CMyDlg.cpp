// CMyDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCGamePlugin.h"
#include "CMyDlg.h"
#include "afxdialogex.h"

// CMyDlg 对话框

IMPLEMENT_DYNAMIC(CMyDlg, CDialogEx)

CMyDlg::CMyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CMyDlg::~CMyDlg()
{
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMyDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CMyDlg 消息处理程序


void CMyDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	CMFCGamePluginApp* pApp = (CMFCGamePluginApp*)AfxGetApp();
	::SendMessage(pApp->m_hWnd, WM_DATA1, 0, 0);
}

void CMyDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMFCGamePluginApp* pApp = (CMFCGamePluginApp*)AfxGetApp();
	::SendMessage(pApp->m_hWnd, WM_DATA2, 0, 0);
}


void CMyDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	CMFCGamePluginApp* pApp = (CMFCGamePluginApp*)AfxGetApp();
	// 循环消除
	for (int i = 0; i < 100; i++)
	{
		int nRet = ::SendMessage(pApp->m_hWnd, WM_DATA2, 0, 0);
		if (nRet == -1)
			break;
	}
}


void CMyDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CMFCGamePluginApp* pApp = (CMFCGamePluginApp*)AfxGetApp();
	::SendMessage(pApp->m_hWnd, WM_DATA3, 0, 0);
}


void CMyDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CMFCGamePluginApp* pApp = (CMFCGamePluginApp*)AfxGetApp();
	// 循环消除
	for (int i = 0; i < 60; i++)
	{
		::SendMessage(pApp->m_hWnd, WM_DATA3, 0, 0);
	}
}
