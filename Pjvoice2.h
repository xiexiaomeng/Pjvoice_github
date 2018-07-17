// Pjvoice2.h : main header file for the PJVOICE2 application
//

#if !defined(AFX_PJVOICE2_H__542CC6FE_668C_4F7D_BB2B_9BE45792D001__INCLUDED_)
#define AFX_PJVOICE2_H__542CC6FE_668C_4F7D_BB2B_9BE45792D001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPjvoice2App:
// See Pjvoice2.cpp for the implementation of this class
//

class CPjvoice2App : public CWinApp
{
public:
	CPjvoice2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPjvoice2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPjvoice2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PJVOICE2_H__542CC6FE_668C_4F7D_BB2B_9BE45792D001__INCLUDED_)
