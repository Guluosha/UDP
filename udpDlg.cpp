// udpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "udp.h"
#include "udpDlg.h"
#include<Winsock2.h>    //sockͷ�ļ�
#pragma comment(lib,"ws2_32.lib")  //����Ҫ�ӵĿ�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int state=0;		//����״̬��0��ʾ���ߣ�1��ʾ����
CString ip[500];  //ip�б�	
int temp=0;		//ip�б��±�
int ping=0;		//����״̬��1��ʾ���Σ�0��ʾ������

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CUdpDlg dialog
CUdpDlg::CUdpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUdpDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CUdpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUdpDlg)
	DDX_Control(pDX, IDC_listip, m_listip);
	DDX_Control(pDX, IDC_recvtext, m_re);
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CUdpDlg, CDialog)
	//{{AFX_MSG_MAP(CUdpDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_send, Onsend)
	ON_MESSAGE(WM_RECVDATA,OnRecvData)   //�߳���Ϣ��Ӧӳ��
	ON_BN_CLICKED(IDC_send2, Onsend2)
	ON_BN_CLICKED(IDC_change, Onchange)
	ON_BN_CLICKED(IDC_refresh, Onrefresh)
	ON_BN_CLICKED(IDC_clear, Onclear)
	ON_LBN_DBLCLK(IDC_listip, OnDblclklistip)
	ON_BN_CLICKED(IDC_change1, Onchange1)
	ON_BN_CLICKED(IDC_ping, Onping)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CUdpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CUdpDlg::InitSocket();    //���ó�ʼ���׽��ֺ���

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	//��ʾ�������ƺ�ip��ַ
	char s[50];
	memset(s,0,sizeof(s));  
	DWORD i=50;  
	GetComputerName(s,&i); 
	SetDlgItemText(IDC_name,s);
	hostent *host;
	host=gethostbyname(s);
	CString sip;
	sip=inet_ntoa(*(struct in_addr*)host->h_addr_list[0]);
	SetDlgItemText(IDC_sip,sip);

	//��ʾ��ʼ״̬
	SetDlgItemText(IDC_stat,"����");
	SetDlgItemText(IDC_ping,"�տ�Ⱥ��Ϣ");
	
	//����һ�����ݸ��߳����ݵı�������ʼ��ֵ��Ȼ������̺߳���
	recvparam *pr=new recvparam;  
	pr->sock=m_socket;			
	pr->hwnd=m_hWnd; 		
	HANDLE hThread=CreateThread(NULL,0,RecvProc,(LPVOID)pr,0,NULL);	
	CloseHandle(hThread);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUdpDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUdpDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUdpDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//��ʼ���׽��ֺ���
void CUdpDlg::InitSocket()
{
	//��������ʼ���󶨷��͵�Ե���Ϣ���׽���
	//��������ʼ��һ����ַ��Ϣ
	sockaddr_in a;
	a.sin_family=AF_INET;
	a.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	a.sin_port=htons(5000);
	//�����׽���
	m_socket=socket(AF_INET,SOCK_DGRAM,0);
	if(INVALID_SOCKET==m_socket)
	{
		MessageBox("�׽��ִ���ʧ��!");
		return;
	}
	bool optval;
	optval=true;
	//ʹ�׽��־��й㲥����
	setsockopt(m_socket,SOL_SOCKET,SO_BROADCAST,(char FAR *)&optval,sizeof(optval));
	int i;
	//���׽���
	i=bind(m_socket,(sockaddr *)&a,sizeof(sockaddr_in));
	if(i==SOCKET_ERROR)
	{
		closesocket(m_socket);
		MessageBox("��ʧ��!");
		return;
	}

	//��ʼ�����󶨷��͹㲥��Ϣ���׽���
	//���˶˿������ĺ�����һ��
	sockaddr_in b;
	b.sin_family=AF_INET;
	b.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	b.sin_port=htons(4000);
	b_socket=socket(AF_INET,SOCK_DGRAM,0);
	if(INVALID_SOCKET==b_socket)
	{
		MessageBox("�׽��ִ���ʧ��!");
		return;
	}
	setsockopt(b_socket,SOL_SOCKET,SO_BROADCAST,(char FAR *)&optval,sizeof(optval));
	i=bind(b_socket,(sockaddr *)&b,sizeof(sockaddr_in));
	if(i==SOCKET_ERROR)
	{
		closesocket(m_socket);
		MessageBox("��ʧ��!");
		return;
	}
}

//�߳���ں�����������Ϣ
DWORD WINAPI CUdpDlg::RecvProc(LPVOID IpPara)
{
	//�����ں���������ֵ
	SOCKET sock=((recvparam*)IpPara)->sock; 
	HWND hwnd=((recvparam*)IpPara)->hwnd; 
	int len=sizeof(sockaddr);
	char recvbuf[200];   
	char tempbuf[300];
	int i;
	sockaddr_in addrfrom;  //����һ����ַ�����������洢�յ���Դ��ַ����Ϣ
	while(TRUE)
	{
		//������Ϣ
		i=recvfrom(sock,recvbuf,200,0,(sockaddr*)&addrfrom,&len); 
		if(i==SOCKET_ERROR)
		{
			break;
		}
		sprintf(tempbuf,"%s",recvbuf);
		//�㲥��Ϣ������룬���͹㲥��Ϣʱʹ�ð�4000�˿ڵ��׽���
		if(addrfrom.sin_port==htons(4000))
		{
			CString str;
			char s=tempbuf[0];
			int i,t=-1;
			str=inet_ntoa(addrfrom.sin_addr);
			//�ж���ϢԴ��ַ�Ƿ����ip�б���
			for(i=0;i<temp;i++)
				if(ip[i]==str)
				{t=i;break;}
			//����㲥��Ϣ��֪����Դ��ַ������Ϣ����ip�б���û�и�Դ��ַ
			if(s=='1'&&t==-1)
			{
				ip[temp]=str;
				temp++;
			}
			//����㲥��Ϣ��֪����Դ��ַ������Ϣ����ip�б����и�Դ��ַ
			else if(s=='0'&&t!=-1)
			{
				for(i=t;i<temp-1;i++)
				{
					ip[i]=ip[i+1];
				}
				temp--;
				ip[temp]="NULL";
			}
		}
		//˽����Ϣ��Ⱥ����Ϣ������룬������Ϣ��ʹ�ð�5000�˿ڵ��׽��ַ���
		else
		{
			//�����Ⱥ����Ϣ
			if(tempbuf[0]=='0')
			{
				if(ping)    //���״̬����Ϊ���Σ�����������ʾ��Ϣ������Ĳ���
					continue;
			}
		}
		//������Ϣ��ʽ�����ݴ����Ի���
		::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)tempbuf);
	}
	return 0;
}

//����Ⱥ�ĺ���
void CUdpDlg::Onsend() 
{
	// TODO: Add your control notification handler code here
	if(state)    //����״̬����������Ϣ
	{
		//��ȡ�û�����Ҫ���͵���Ϣ�Լ��������Ⱥ����Ϣ
		CString strsend,name,str;
		GetDlgItemText(IDC_sendtext,str);
		GetDlgItemText(IDC_name,name);
		strsend="0";   //��Ǹ���Ϣ��Ⱥ����Ϣ
		strsend=strsend+"������-"+"[ "+name+" ]"+"˵:";
		strsend=strsend+str;
		SetDlgItemText(IDC_sendtext,"");
		//����Ŀ�ĵ�ַ��Ϣ
		SOCKADDR_IN addrto;  
		addrto.sin_family=AF_INET;
		addrto.sin_port=htons(5000);
		//��ѯip��ַ���ͣ�ʹ�ð�5000�˿ڵ��׽��ַ���
		for(int i=0;i<temp;i++)
		{
			addrto.sin_addr.S_un.S_addr=inet_addr(ip[i]);  
			sendto(m_socket,strsend,strsend.GetLength()+1,0,(SOCKADDR*)&addrto,sizeof(SOCKADDR));
		}
	}
	else 
		AfxMessageBox("δ���ߣ����ܽ���ͨ�ţ���");
}

//��Ϣӳ�亯��
/*void CUdpDlg::OnRecvData(WPARAM wParam,LPARAM IParam)
{
	//��ʾ���ݵ�����
	CString str=(char*)IParam;
	int i=m_re.GetCount();    //��ȡ�ؼ���ǰ����
	str=str.Right(str.GetLength()-1);//����״̬��ʾλ
	m_re.InsertString(i,str); //�����һ�в�������
	m_re.SetCurSel(i);	//ѡ�����һ�У�����������Զ�������µ���Ϣ
}*/

//��Ϣӳ�亯��
LRESULT CUdpDlg::OnRecvData(WPARAM wParam, LPARAM IParam)
{
	//��ʾ���ݵ�����
	CString str = (char*)IParam;
	int i = m_re.GetCount();    //��ȡ�ؼ���ǰ����
	str = str.Right(str.GetLength() - 1);//����״̬��ʾλ
	m_re.InsertString(i, str); //�����һ�в�������
	m_re.SetCurSel(i);	//ѡ�����һ�У�����������Զ�������µ���Ϣ
	return 0;//LRESULT;
}

//����˽����Ϣ
void CUdpDlg::Onsend2() 
{
	// TODO: Add your control notification handler code here
	if(state)   //ֻ������״�����ܷ���
	{
		//ͬȺ�Ĳ��֣���ȡ������ip�б���ѡ�е�ip��ΪĿ�ĵ�ַ����
		SOCKADDR_IN addrto; 
		addrto.sin_family=AF_INET;
		addrto.sin_port=htons(5000); 
		CString str,ip,name,strsend;
		GetDlgItemText(IDC_onip,ip);
		GetDlgItemText(IDC_name,name);
		GetDlgItemText(IDC_sendtext2,str);
		strsend="1";   //��ʾ��˽����Ϣ
		strsend=strsend+"˽��-"+"[ "+name+" ]"+"˵:";
		strsend=strsend+str;
		addrto.sin_addr.S_un.S_addr=inet_addr(ip); 
		sendto(m_socket,strsend,strsend.GetLength()+1,0,(SOCKADDR*)&addrto,sizeof(SOCKADDR));
		//˽��ʱ��ʾһ����Ϣ��������
		int i=m_re.GetCount();
		m_re.InsertString(i,strsend); 
		m_re.SetCurSel(i);
		SetDlgItemText(IDC_sendtext2,"");
	}
	else
	{
		AfxMessageBox("δ���ߣ����ܽ���ͨ�ţ���");
	}
}

//���ߺ���
void CUdpDlg::Onchange() 
{
	// TODO: Add your control notification handler code here
	if(state==0)    //ֻ�д�������״̬���ܽ������߲���
	{
		//����Ŀ�ĵ�ַ��Ŀ�ĵ�ַ��ipΪ�㲥��ַ
		sockaddr_in addrto;
		int t=1;
		addrto.sin_family=AF_INET;
		addrto.sin_addr.S_un.S_addr=INADDR_BROADCAST;
		addrto.sin_port=htons(5000);
		CString strsend,name;
		GetDlgItemText(IDC_name,name);
		if(t)    //�������û��ʧ��
		{
				strsend="1";    //��������״̬λ
				strsend=strsend+"[ "+name+" ]"+"�����ˣ�������*^�n^*��";
				SetDlgItemText(IDC_stat,"����");
				state=1;   //�ı䵱ǰ״̬
		}
		int i;
		//ʹ�ð���4000�˿ڵ��׽��ַ��ͣ�b_socket��
		i=sendto(b_socket,strsend,strsend.GetLength()+1,0,(SOCKADDR*)&addrto,sizeof(SOCKADDR));
		if(i==SOCKET_ERROR)  //����ʧ��
		{
			AfxMessageBox("��������ʧ�ܣ�");
			t=0;			
			CUdpDlg::Onchange();  //���·�һ��
		}
	}
	else
		AfxMessageBox("�Ѿ�������״̬");
}

//���ߺ���
void CUdpDlg::Onchange1() 
{
	// TODO: Add your control notification handler code here
	if(state==1)     //ֻ�д�������״̬�Ĳ�������
	{
		//ͬ���ߺ���
		sockaddr_in addrto;
		addrto.sin_family=AF_INET;
		addrto.sin_addr.S_un.S_addr=INADDR_BROADCAST;
		addrto.sin_port=htons(5000);
		int t=1;
		CString strsend,name;
		GetDlgItemText(IDC_name,name);
		if(t)
		{
				strsend="0";   //��������״̬λ
				strsend=strsend+"[ "+name+" ]"+"�����ˣ�����";
				SetDlgItemText(IDC_stat,"����");
				state=0;    //�ı䵱ǰ״̬
		}
		int i;
		i=sendto(b_socket,strsend,strsend.GetLength()+1,0,(SOCKADDR*)&addrto,sizeof(SOCKADDR));
		if(i==SOCKET_ERROR)
		{
			AfxMessageBox("��������ʧ�ܣ�");
			t=0;
			CUdpDlg::Onchange();
		}
	}
	else
		AfxMessageBox("�Ѿ�������״̬");
}

//ˢ������ip�б�
void CUdpDlg::Onrefresh() 
{
	// TODO: Add your control notification handler code here
		m_listip.ResetContent();  //��տռ���Ϣ
		for (int i = 0;i < temp;i++) {
			m_listip.InsertString(m_listip.GetCount(), ip[i]);
			m_listip.SetCurSel(i);
		}
}

//�����Ϣ����
void CUdpDlg::Onclear() 
{
	// TODO: Add your control notification handler code here
	m_re.ResetContent();
}

//���˫������ip�б����Ϣ��Ӧ����
void CUdpDlg::OnDblclklistip() 
{
	// TODO: Add your control notification handler code here
	//�����ݴ���һ����̬�ı���
	CString str;
	int index=m_listip.GetCurSel();
	m_listip.GetText(index,str);
	SetDlgItemText(IDC_onip,str);
}

//�ı�����״̬��ping=1�������Σ�ping=0��������
void CUdpDlg::Onping() 
{
	// TODO: Add your control notification handler code here
	if(ping==0)
	{
		ping=1;
		SetDlgItemText(IDC_ping,"����Ⱥ��Ϣ");
	}
	else if(ping==1)
	{
		ping=0;
		SetDlgItemText(IDC_ping,"�տ�Ⱥ��Ϣ");
	}
}
