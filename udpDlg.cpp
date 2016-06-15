// udpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "udp.h"
#include "udpDlg.h"
#include<Winsock2.h>    //sock头文件
#pragma comment(lib,"ws2_32.lib")  //必须要加的库
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int state=0;		//在线状态，0表示下线，1表示上线
CString ip[500];  //ip列表	
int temp=0;		//ip列表下标
int ping=0;		//屏蔽状态，1表示屏蔽，0表示不屏蔽

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
	ON_MESSAGE(WM_RECVDATA,OnRecvData)   //线程消息响应映射
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
	CUdpDlg::InitSocket();    //调用初始化套接字函数

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
	
	//显示本机名称和ip地址
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

	//显示初始状态
	SetDlgItemText(IDC_stat,"下线");
	SetDlgItemText(IDC_ping,"收看群消息");
	
	//创建一个传递给线程数据的变量并初始化值，然后进入线程函数
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

//初始化套接字函数
void CUdpDlg::InitSocket()
{
	//创建并初始化绑定发送点对点信息的套接字
	//创建并初始化一个地址信息
	sockaddr_in a;
	a.sin_family=AF_INET;
	a.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	a.sin_port=htons(5000);
	//创建套接字
	m_socket=socket(AF_INET,SOCK_DGRAM,0);
	if(INVALID_SOCKET==m_socket)
	{
		MessageBox("套接字创建失败!");
		return;
	}
	bool optval;
	optval=true;
	//使套接字具有广播功能
	setsockopt(m_socket,SOL_SOCKET,SO_BROADCAST,(char FAR *)&optval,sizeof(optval));
	int i;
	//绑定套接字
	i=bind(m_socket,(sockaddr *)&a,sizeof(sockaddr_in));
	if(i==SOCKET_ERROR)
	{
		closesocket(m_socket);
		MessageBox("绑定失败!");
		return;
	}

	//初始化并绑定发送广播信息的套接字
	//除了端口其他的和上面一致
	sockaddr_in b;
	b.sin_family=AF_INET;
	b.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	b.sin_port=htons(4000);
	b_socket=socket(AF_INET,SOCK_DGRAM,0);
	if(INVALID_SOCKET==b_socket)
	{
		MessageBox("套接字创建失败!");
		return;
	}
	setsockopt(b_socket,SOL_SOCKET,SO_BROADCAST,(char FAR *)&optval,sizeof(optval));
	i=bind(b_socket,(sockaddr *)&b,sizeof(sockaddr_in));
	if(i==SOCKET_ERROR)
	{
		closesocket(m_socket);
		MessageBox("绑定失败!");
		return;
	}
}

//线程入口函数，监听信息
DWORD WINAPI CUdpDlg::RecvProc(LPVOID IpPara)
{
	//获得入口函数参数的值
	SOCKET sock=((recvparam*)IpPara)->sock; 
	HWND hwnd=((recvparam*)IpPara)->hwnd; 
	int len=sizeof(sockaddr);
	char recvbuf[200];   
	char tempbuf[300];
	int i;
	sockaddr_in addrfrom;  //创建一个地址变量，用来存储收到的源地址的信息
	while(TRUE)
	{
		//接收信息
		i=recvfrom(sock,recvbuf,200,0,(sockaddr*)&addrfrom,&len); 
		if(i==SOCKET_ERROR)
		{
			break;
		}
		sprintf(tempbuf,"%s",recvbuf);
		//广播信息处理代码，发送广播信息时使用绑定4000端口的套接字
		if(addrfrom.sin_port==htons(4000))
		{
			CString str;
			char s=tempbuf[0];
			int i,t=-1;
			str=inet_ntoa(addrfrom.sin_addr);
			//判断信息源地址是否存在ip列表中
			for(i=0;i<temp;i++)
				if(ip[i]==str)
				{t=i;break;}
			//如果广播信息告知是其源地址上线信息并且ip列表中没有该源地址
			if(s=='1'&&t==-1)
			{
				ip[temp]=str;
				temp++;
			}
			//如果广播信息告知是其源地址下线信息并且ip列表中有该源地址
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
		//私聊信息、群聊信息处理代码，发送信息是使用绑定5000端口的套接字发送
		else
		{
			//如果是群聊信息
			if(tempbuf[0]=='0')
			{
				if(ping)    //如果状态设置为屏蔽，跳过下面显示信息到界面的操作
					continue;
			}
		}
		//发送信息方式把数据传给对话框
		::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)tempbuf);
	}
	return 0;
}

//发送群聊函数
void CUdpDlg::Onsend() 
{
	// TODO: Add your control notification handler code here
	if(state)    //上线状态才允许发送信息
	{
		//获取用户名、要发送的信息以及标记这是群聊信息
		CString strsend,name,str;
		GetDlgItemText(IDC_sendtext,str);
		GetDlgItemText(IDC_name,name);
		strsend="0";   //标记该信息是群聊信息
		strsend=strsend+"聊天室-"+"[ "+name+" ]"+"说:";
		strsend=strsend+str;
		SetDlgItemText(IDC_sendtext,"");
		//定义目的地址信息
		SOCKADDR_IN addrto;  
		addrto.sin_family=AF_INET;
		addrto.sin_port=htons(5000);
		//轮询ip地址表发送，使用绑定5000端口的套接字发送
		for(int i=0;i<temp;i++)
		{
			addrto.sin_addr.S_un.S_addr=inet_addr(ip[i]);  
			sendto(m_socket,strsend,strsend.GetLength()+1,0,(SOCKADDR*)&addrto,sizeof(SOCKADDR));
		}
	}
	else 
		AfxMessageBox("未上线，不能进行通信！！");
}

//消息映射函数
/*void CUdpDlg::OnRecvData(WPARAM wParam,LPARAM IParam)
{
	//显示数据到界面
	CString str=(char*)IParam;
	int i=m_re.GetCount();    //获取控件当前行数
	str=str.Right(str.GetLength()-1);//消除状态表示位
	m_re.InsertString(i,str); //在最后一行插入数据
	m_re.SetCurSel(i);	//选择最后一行，这样可以永远看到最新的信息
}*/

//消息映射函数
LRESULT CUdpDlg::OnRecvData(WPARAM wParam, LPARAM IParam)
{
	//显示数据到界面
	CString str = (char*)IParam;
	int i = m_re.GetCount();    //获取控件当前行数
	str = str.Right(str.GetLength() - 1);//消除状态表示位
	m_re.InsertString(i, str); //在最后一行插入数据
	m_re.SetCurSel(i);	//选择最后一行，这样可以永远看到最新的信息
	return 0;//LRESULT;
}

//发送私聊信息
void CUdpDlg::Onsend2() 
{
	// TODO: Add your control notification handler code here
	if(state)   //只有在线状况才能发送
	{
		//同群聊部分，获取在在线ip列表中选中的ip作为目的地址发送
		SOCKADDR_IN addrto; 
		addrto.sin_family=AF_INET;
		addrto.sin_port=htons(5000); 
		CString str,ip,name,strsend;
		GetDlgItemText(IDC_onip,ip);
		GetDlgItemText(IDC_name,name);
		GetDlgItemText(IDC_sendtext2,str);
		strsend="1";   //表示是私聊信息
		strsend=strsend+"私聊-"+"[ "+name+" ]"+"说:";
		strsend=strsend+str;
		addrto.sin_addr.S_un.S_addr=inet_addr(ip); 
		sendto(m_socket,strsend,strsend.GetLength()+1,0,(SOCKADDR*)&addrto,sizeof(SOCKADDR));
		//私聊时显示一份信息到界面上
		int i=m_re.GetCount();
		m_re.InsertString(i,strsend); 
		m_re.SetCurSel(i);
		SetDlgItemText(IDC_sendtext2,"");
	}
	else
	{
		AfxMessageBox("未上线，不能进行通信！！");
	}
}

//上线函数
void CUdpDlg::Onchange() 
{
	// TODO: Add your control notification handler code here
	if(state==0)    //只有处于下线状态才能进行上线操作
	{
		//定义目的地址，目的地址的ip为广播地址
		sockaddr_in addrto;
		int t=1;
		addrto.sin_family=AF_INET;
		addrto.sin_addr.S_un.S_addr=INADDR_BROADCAST;
		addrto.sin_port=htons(5000);
		CString strsend,name;
		GetDlgItemText(IDC_name,name);
		if(t)    //如果操作没有失败
		{
				strsend="1";    //设置上线状态位
				strsend=strsend+"[ "+name+" ]"+"上线了！！！（*^﹏^*）";
				SetDlgItemText(IDC_stat,"上线");
				state=1;   //改变当前状态
		}
		int i;
		//使用绑定了4000端口的套接字发送（b_socket）
		i=sendto(b_socket,strsend,strsend.GetLength()+1,0,(SOCKADDR*)&addrto,sizeof(SOCKADDR));
		if(i==SOCKET_ERROR)  //操作失败
		{
			AfxMessageBox("发送请求失败！");
			t=0;			
			CUdpDlg::Onchange();  //重新发一次
		}
	}
	else
		AfxMessageBox("已经是上线状态");
}

//下线函数
void CUdpDlg::Onchange1() 
{
	// TODO: Add your control notification handler code here
	if(state==1)     //只有处于上线状态的才能下线
	{
		//同上线函数
		sockaddr_in addrto;
		addrto.sin_family=AF_INET;
		addrto.sin_addr.S_un.S_addr=INADDR_BROADCAST;
		addrto.sin_port=htons(5000);
		int t=1;
		CString strsend,name;
		GetDlgItemText(IDC_name,name);
		if(t)
		{
				strsend="0";   //设置下线状态位
				strsend=strsend+"[ "+name+" ]"+"下线了！！！";
				SetDlgItemText(IDC_stat,"下线");
				state=0;    //改变当前状态
		}
		int i;
		i=sendto(b_socket,strsend,strsend.GetLength()+1,0,(SOCKADDR*)&addrto,sizeof(SOCKADDR));
		if(i==SOCKET_ERROR)
		{
			AfxMessageBox("发送请求失败！");
			t=0;
			CUdpDlg::Onchange();
		}
	}
	else
		AfxMessageBox("已经是下线状态");
}

//刷新在线ip列表
void CUdpDlg::Onrefresh() 
{
	// TODO: Add your control notification handler code here
		m_listip.ResetContent();  //清空空间信息
		for (int i = 0;i < temp;i++) {
			m_listip.InsertString(m_listip.GetCount(), ip[i]);
			m_listip.SetCurSel(i);
		}
}

//清空消息界面
void CUdpDlg::Onclear() 
{
	// TODO: Add your control notification handler code here
	m_re.ResetContent();
}

//左键双击在线ip列表框消息响应函数
void CUdpDlg::OnDblclklistip() 
{
	// TODO: Add your control notification handler code here
	//把数据传到一个静态文本框
	CString str;
	int index=m_listip.GetCurSel();
	m_listip.GetText(index,str);
	SetDlgItemText(IDC_onip,str);
}

//改变屏蔽状态，ping=1代表屏蔽，ping=0代表不屏蔽
void CUdpDlg::Onping() 
{
	// TODO: Add your control notification handler code here
	if(ping==0)
	{
		ping=1;
		SetDlgItemText(IDC_ping,"屏蔽群消息");
	}
	else if(ping==1)
	{
		ping=0;
		SetDlgItemText(IDC_ping,"收看群消息");
	}
}
