; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CUdpDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "udp.h"

ClassCount=3
Class1=CUdpApp
Class2=CUdpDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_UDP_DIALOG

[CLS:CUdpApp]
Type=0
HeaderFile=udp.h
ImplementationFile=udp.cpp
Filter=N

[CLS:CUdpDlg]
Type=0
HeaderFile=udpDlg.h
ImplementationFile=udpDlg.cpp
Filter=D
LastObject=IDC_ping
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=udpDlg.h
ImplementationFile=udpDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_UDP_DIALOG]
Type=1
Class=CUdpDlg
ControlCount=19
Control1=IDC_sendtext,edit,1350631552
Control2=IDC_send,button,1342242817
Control3=IDC_name,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_recvtext,listbox,1352728835
Control6=IDC_STATIC,static,1342308352
Control7=IDC_sendtext2,edit,1350631552
Control8=IDC_send2,button,1342242816
Control9=IDC_refresh,button,1342242816
Control10=IDC_change,button,1342242816
Control11=IDC_STATIC,static,1342308352
Control12=IDC_stat,static,1342308352
Control13=IDC_sip,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_listip,listbox,1352728835
Control16=IDC_clear,button,1342242816
Control17=IDC_onip,static,1342308352
Control18=IDC_change1,button,1342242816
Control19=IDC_ping,button,1342242816

