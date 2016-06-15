// udp.h : main header file for the UDP application
//

#if !defined(AFX_UDP_H__E86293AB_5FFA_454B_AB09_C7232F812A25__INCLUDED_)
#define AFX_UDP_H__E86293AB_5FFA_454B_AB09_C7232F812A25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUdpApp:
// See udp.cpp for the implementation of this class
//

class CUdpApp : public CWinApp
{
public:
	CUdpApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUdpApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUdpApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDP_H__E86293AB_5FFA_454B_AB09_C7232F812A25__INCLUDED_)
