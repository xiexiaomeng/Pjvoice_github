// Pjvoice2Dlg.h : header file
//

#if !defined(AFX_PJVOICE2DLG_H__4AC120E1_BBA8_4946_89D6_AFC7120C8875__INCLUDED_)
#define AFX_PJVOICE2DLG_H__4AC120E1_BBA8_4946_89D6_AFC7120C8875__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include <windows.h>//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/////////////////////////////////////////////////////////////////////////////
// CPjvoice2Dlg dialog

class CPjvoice2Dlg : public CDialog
{
// Construction
public:
	BOOL m_rec;     //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	int m_framenum1;  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	short* m_vdata1;
	float m_energy1;    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	BOOL m_firstrecord;   //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	void StartRecord(); //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	void PrePareRecord();  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	BOOL OpenDevice();      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	CPjvoice2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPjvoice2Dlg)
	enum { IDD = IDD_PJVOICE2_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPjvoice2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	PBYTE* m_pBuffer;  //»º´æ//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	PWAVEHDR* m_pWaveHdr;  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	WAVEFORMATEX m_wfx;   //%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	HWAVEIN m_hWaveIn;  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	int m_iBufNum;  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPjvoice2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTrain();
	afx_msg void OnRecog();
	afx_msg void OnMashuxunlian();
	afx_msg void OnHmmInit();
	afx_msg void OnShiHua();
	afx_msg void OnTrainHMM();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnYongyushibie();	
	afx_msg void OnMM_WIM_OPEN(UINT wParam,LONG lParam);//%%%%%%%%%%%%%%%%
	afx_msg void OnMM_WIM_DATA(UINT wParam,LONG lParam);//%%%%%%%%%%%%%%%%
	afx_msg void OnMM_WIM_CLOSE(UINT wParam,LONG lParam);//%%%%%%%%%%%%%%%%
	afx_msg void OnButton10();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PJVOICE2DLG_H__4AC120E1_BBA8_4946_89D6_AFC7120C8875__INCLUDED_)
