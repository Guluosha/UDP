// udpDlg.h : header file
//

#if !defined(AFX_UDPDLG_H__41F6D40D_1E92_46A8_81BA_650339014C5B__INCLUDED_)
#define AFX_UDPDLG_H__41F6D40D_1E92_46A8_81BA_650339014C5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_RECVDATA WM_USER+1    //定义消息

/////////////////////////////////////////////////////////////////////////////
// CUdpDlg dialog
#include<Winsock2.h>
#pragma comment(lib,"ws2_32.lib")

//定义传递信息给线程的机构体
struct recvparam				 
{
	SOCKET sock;	//接收数据的套接字
	HWND hwnd;		//接收数据窗体的句柄
};

class CUdpDlg : public CDialog
{
// Construction
public:
	CUdpDlg(CWnd* pParent = NULL);	// standard constructor
	void InitSocket();  //声明初始化套接字函数
	SOCKET m_socket;	//声明发送点到点信息的套接字
	SOCKET b_socket;	//声明发送广播信息的套接字
	static DWORD WINAPI RecvProc(LPVOID IpPara);  //声明线程函数入口

// Dialog Data
	//{{AFX_DATA(CUdpDlg)
	enum { IDD = IDD_UDP_DIALOG };
	CListBox	m_listip;
	CListBox	m_re;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUdpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUdpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void Onsend();
	//afx_msg void OnRecvData(WPARAM wParam,LPARAM IParam);  //消息响应函数原型声明
	afx_msg LRESULT OnRecvData(WPARAM wParam, LPARAM IParam);  //消息响应函数原型声明
	afx_msg void Onsend2();
	afx_msg void Onchange();
	afx_msg void Onrefresh();
	afx_msg void Onclear();
	afx_msg void OnDblclklistip();
	afx_msg void Onchange1();
	afx_msg void Onping();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPDLG_H__41F6D40D_1E92_46A8_81BA_650339014C5B__INCLUDED_)
