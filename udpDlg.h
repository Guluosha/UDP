// udpDlg.h : header file
//

#if !defined(AFX_UDPDLG_H__41F6D40D_1E92_46A8_81BA_650339014C5B__INCLUDED_)
#define AFX_UDPDLG_H__41F6D40D_1E92_46A8_81BA_650339014C5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_RECVDATA WM_USER+1    //������Ϣ

/////////////////////////////////////////////////////////////////////////////
// CUdpDlg dialog
#include<Winsock2.h>
#pragma comment(lib,"ws2_32.lib")

//���崫����Ϣ���̵߳Ļ�����
struct recvparam				 
{
	SOCKET sock;	//�������ݵ��׽���
	HWND hwnd;		//�������ݴ���ľ��
};

class CUdpDlg : public CDialog
{
// Construction
public:
	CUdpDlg(CWnd* pParent = NULL);	// standard constructor
	void InitSocket();  //������ʼ���׽��ֺ���
	SOCKET m_socket;	//�������͵㵽����Ϣ���׽���
	SOCKET b_socket;	//�������͹㲥��Ϣ���׽���
	static DWORD WINAPI RecvProc(LPVOID IpPara);  //�����̺߳������

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
	//afx_msg void OnRecvData(WPARAM wParam,LPARAM IParam);  //��Ϣ��Ӧ����ԭ������
	afx_msg LRESULT OnRecvData(WPARAM wParam, LPARAM IParam);  //��Ϣ��Ӧ����ԭ������
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
