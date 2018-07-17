// Pjvoice2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Pjvoice2.h"
#include "Pjvoice2Dlg.h"
#include "jiegouti.h"
#include "rec.h"
#include "Lbg1.h"
#include<iostream.h>
#include<fstream.h>
#include<iomanip.h>
#include "hmm.h"
#include <stdlib.h>

#include <stdio.h> 
#include "nrutil.h"
#include <math.h>
#include <vector>

#include<string>
#include<cstdio>
#include<cmath>
#include <complex> 
#include <bitset> 
#include <conio.h> 

#include <mmsystem.h>//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define RECORD_BUFFER_SIZE 3904 //����Ļ���������//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define RECORD_BUFFER_SIZE 3904 //2015.6.9 ����Ļ���������//
//#define FRAME_BUFFER_SIZE 320                     //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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
		// No message handle
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPjvoice2Dlg dialog

CPjvoice2Dlg::CPjvoice2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPjvoice2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPjvoice2Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPjvoice2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPjvoice2Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPjvoice2Dlg, CDialog)
	//{{AFX_MSG_MAP(CPjvoice2Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnTrain)
	ON_BN_CLICKED(IDC_BUTTON2, OnRecog)
	ON_BN_CLICKED(IDC_BUTTON3, OnMashuxunlian)
	ON_BN_CLICKED(IDC_BUTTON4, OnHmmInit)
	ON_BN_CLICKED(IDC_BUTTON5, OnShiHua)
	ON_BN_CLICKED(IDC_BUTTON6, OnTrainHMM)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnYongyushibie)	
	ON_MESSAGE(MM_WIM_OPEN,OnMM_WIM_OPEN)//%%%%%%%%%%%%%%%%
	ON_MESSAGE(MM_WIM_DATA,OnMM_WIM_DATA)//%%%%%%%%%%%%%%%%
	ON_MESSAGE(MM_WIM_CLOSE,OnMM_WIM_CLOSE)//%%%%%%%%%%%%%%%%
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPjvoice2Dlg message handlers

BOOL CPjvoice2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	m_vdata1= (short*)calloc(512*1000,sizeof(short)); //xieyanyi
	//¼����ʼ��
	m_iBufNum = 2;//¼������������������//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_pBuffer = new PBYTE[m_iBufNum];//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_pWaveHdr = new PWAVEHDR[m_iBufNum];//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	for (int i = 0; i < m_iBufNum; i++)//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	{
		m_pBuffer[i]=(PBYTE)calloc(1,RECORD_BUFFER_SIZE);//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		m_pWaveHdr[i]=reinterpret_cast<PWAVEHDR>(calloc(1,sizeof(WAVEHDR)));//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

		//m_pBuffer[i]=(PBYTE)malloc(RECORD_BUFFER_SIZE);//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		//m_pWaveHdr[i]=reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		if (!m_pBuffer[i] || !m_pWaveHdr[i])//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		{
			if (m_pBuffer[i]) free(m_pBuffer[i]);//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			if (m_pWaveHdr[i]) free(m_pWaveHdr[i]);//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			MessageBeep(MB_ICONEXCLAMATION);//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			AfxMessageBox(_T("Memory error!"));//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			return FALSE;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		}
	}

	if(!OpenDevice())//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	{
		MessageBox(_T("��¼���豸����"), _T("����"),MB_OK);//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		return FALSE;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	}
	//���б���в�������

	m_firstrecord=TRUE;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_framenum1=0;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_rec=FALSE;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPjvoice2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPjvoice2Dlg::OnPaint() 
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
HCURSOR CPjvoice2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CPjvoice2Dlg::OpenDevice()//����һЩ����//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
{
	m_wfx.wFormatTag = WAVE_FORMAT_PCM;//¼����ʽ//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_wfx.nChannels =1;//������//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_wfx.nSamplesPerSec = 32000;   //������//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_wfx.wBitsPerSample = 16;//����λ//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_wfx.nAvgBytesPerSec = 32000*2;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_wfx.nBlockAlign = 2;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_wfx.cbSize = 0;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	if(waveInOpen(&m_hWaveIn,WAVE_MAPPER,&m_wfx,(DWORD)this->m_hWnd,NULL,CALLBACK_WINDOW))//�򿪲��������豸Ϊ¼��//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	{
		MessageBeep(MB_ICONEXCLAMATION);//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		AfxMessageBox(_T("Audio can not be open!"));//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		return FALSE;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	}
    return TRUE;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%<<
}

void CPjvoice2Dlg::PrePareRecord()//��������
{
	for (int i = 0; i < m_iBufNum; i++)//���ӻ��棬m_iBufNum==2�����Է�������������
	{
		m_pWaveHdr[i]->lpData = (char*)m_pBuffer[i];//�������ݻ�����ͷָ�루lpData��
		m_pWaveHdr[i]->dwBufferLength = RECORD_BUFFER_SIZE;//����������
		m_pWaveHdr[i]->dwBytesRecorded = 0;//��������ǰ�Ѿ�ռ�õĳ���
		m_pWaveHdr[i]->dwFlags = 0;
		m_pWaveHdr[i]->dwLoops = 1;
		m_pWaveHdr[i]->dwUser = 0;
		m_pWaveHdr[i]->lpNext = NULL;
		m_pWaveHdr[i]->reserved = 0;
		waveInPrepareHeader(m_hWaveIn,m_pWaveHdr[i],sizeof(WAVEHDR));//����������Ϣ�������豸�����
		waveInAddBuffer(m_hWaveIn,m_pWaveHdr[i],sizeof(WAVEHDR));//����������ַ�������豸�����
	}
}

void CPjvoice2Dlg::StartRecord()
{
     MMRESULT mmResult = waveInStart(m_hWaveIn);//�򿪲��������豸
}
void CPjvoice2Dlg::OnMM_WIM_CLOSE(UINT wParam, LONG lParam)//¼���ر�ʱ����Ϣ��Ӧ��������¼���ر�ʱ��ִ�иú������ú���Ϊ�գ�
{
	//AfxMessageBox("��������");
}

void CPjvoice2Dlg::OnMM_WIM_OPEN(UINT wParam,LONG lParam)//��ʼ¼��ʱ����Ϣ��Ӧ��������¼����ʼʱ��ִ�иú������ú���Ϊ�գ�
{
    ;
}

void CPjvoice2Dlg::OnMM_WIM_DATA(UINT wParam, LONG lParam)//���ô˺���,¼�����̴�����Ϣ��Ӧ��������һ��¼������������ʱ���Զ����øú�����
{
//AfxMessageBox("��������");

	void PreEmphasise (short *s, float k);//Ԥ��������

	//������
    float twopi;
	int hi;
	float Hamming[FrmLen];
	twopi=8.0F*atan(1.0F);
	for( hi=0;hi<FrmLen;hi++)
	{
		Hamming[hi]=(float)(0.54-0.46*cos((float)hi*twopi/(float)(FrmLen-1)));
	} 
		

	//m_vdata1= (short*)calloc(512*200,sizeof(short)); //xieyanyi
	
	short *p;
	short *data,*q;
	float *energy;
	float* mfcc=(float *)calloc((400)*36,sizeof(float));
	int i,j,h;
	static int numl=0,num1l=0;//%%%%%%%%%%%%%%%%%%%%%%%%%%%
	static short kl=0;//%%%%%%%%%%%%%%%%%%%%%%%%%%
    static short  ss=0;//����ƽ��������־����ss=2ʱ����ƽ������
	if (((PWAVEHDR) lParam)->dwBytesRecorded < RECORD_BUFFER_SIZE)//�жϻ������Ƿ�������δ���ú������أ���ֹδ����ִ�����������
	{
		TRACE(_T("No useful data\n"));
		return;
	}
       ss++;
	if (m_firstrecord&&ss==2)//��ss==2ʱ��ƽ��������ֻ��һ�Σ�����Ҫ�ж�energy[i]<m_energy1���Բ�����в�����ȡ�Ȳ���
	{
		//m_energy1=18622.00;//����һ֡�հ׵������õ���ƽ������������õ��ľ���ֵ
		p=(short*)((PWAVEHDR) lParam)->lpData;//���������ݻ�����ͷָ�븳��p
		m_energy1=0.0;

		/*//////////)))))))))))))))))))))
	    for (i=50;i<50+512*3;i++)//ȡ�û�������50����50+512*3������������ƽ������
		{
			//PreEmphasise(q,(float)0.97);
			//m_energy1+=(float)fabs((double)p[i]/2000);//ƽ�����Ⱦ���ֵ�ۼӣ�����2000��������Ե���Ӧ���Ǹ���ʶ��Ч�������˵�
			m_energy1+=(float)fabs((double)(p[i]-p[i-1])*0.97/2000);//2015.6.2 ƽ�����Ⱦ���ֵ�ۼӣ�����2000��������Ե���Ӧ���Ǹ���ʶ��Ч�������˵�
			//m_energy1+=(float)fabs((double)p[i]);//ƽ�����Ⱦ���ֵ�ۼӣ�����2000��������Ե���Ӧ���Ǹ���ʶ��Ч�������˵�
		}
		m_energy1=(float)(1.0*m_energy1/(float)3); //2015.6.10ƽ����������ʵ����ƽ�����ȣ�*/
		
		//m_energy1=���;
    	//////////)))))))))))))))))))))
			m_energy1=1.1;

		m_firstrecord=FALSE;//�������üٱ����ٴμ���ƽ������
		return;
	}
	//��Ϊ����������16λ�ģ�������Ĵ洢�ռ���char�͵�����Ҫ����2������ÿ���ƶ����ǲ�������һ�����������֡��Ҫ����steplen����h=7.46875����Ϊ7֡��
	h=(RECORD_BUFFER_SIZE/2-(FRAMELEN-STEPLEN))/STEPLEN;
	
	//data=(short*)malloc(sizeof(short)*h*FRAMELEN);//Ϊ7֡�������ݷ����µĴ洢�ռ�dataΪͷָ��һ��7*512=3584���ռ�
	//energy=(float*)malloc(sizeof(float)*h);//Ϊÿ֡ƽ����������洢�ռ䣬һ��h֡

	data=(short*)calloc(h*FRAMELEN,sizeof(short));//�洢���źſ��ǲ���
	//data_q=(short*)calloc(h*FRAMELEN,sizeof(short));/////////////////////////////////////////////////
	energy=(float*)calloc(h,sizeof(float));//Ϊÿ֡ƽ����������洢�ռ䣬һ��h֡
	q=data;//��dataͷָ�븳��q
	p=(short*)((PWAVEHDR) lParam)->lpData;//�ѻ������������ݸ���p
	for (i=0;i<h;i++)//���7֡��������ÿ֡�Ķ�ʱ����
	{
		energy[i]=0.0;//ÿ֡�Ķ�ʱ����
		//PreEmphasise(p,(float)0.97);
		for (j=0;j<FRAMELEN;j++)//FRAMELENΪ����֡����������512����ÿ֡��ƽ�����ȣ�������������energy������
		{
			//short data_q=(short)p[j]/(float)2000;
			q[j]=(short)p[j]/(float)1;//2015.6.9
		}
		PreEmphasise(q,(float)0.97);
		//PreEmphasise(q,(float)0.97);
		//PreEmphasise(q,(float)0.97);
		for (j=0;j<FRAMELEN;j++)//FRAMELENΪ����֡����������512����ÿ֡��ƽ�����ȣ�������������energy������
		{
			//short data_q=(short)p[j]/(float)2000;
			q[j]=q[j]*Hamming[j];
			energy[i]=energy[i]+(float)fabs((short)q[j]/(float)2000);//2015.6.9
		}
		q=q+FRAMELEN;//�洢����ָ��ƫ������һ֡�Ĳ���������512��
		p=p+STEPLEN;
		//��������������ָ��ƫ��һ��������STEPLEN=160����Ҳ����˵ÿ��ֻ�ƶ�160����ʵ�ʴ洢��512�������Է����洢��һЩ�������ݡ�
	} 
	q=data;//�ٴδ�ͷ��ַ��ʼ��������������Ч֡


			//CString huhu;
			//huhu.Format("%f",m_energy1);
			//AfxMessageBox(huhu);                   //xjfopajgopajgoiahjgoiahhjkkokopkp


	for (i=0;i<h;i++) //��ȡ��Ч����֡
	{
		switch(kl) //k�ĳ�ʼֵΪ0����̬������
		{
	        case 0:
				{
					if (energy[i]>m_energy1) //�����֡ƽ����������m_energy����֡Ϊ��Ч��������
					{
					    numl++;//��̬������ʼֵΪ0����������Ч֡���м���
						for (j=0;j<FRAMELEN;j++)//�Ѹ�֡���ݸ���m_vdata1
						{
							m_vdata1[m_framenum1*FRAMELEN+j]=q[j];//����200֡��¼��ʱ��Ϊ6.4S
						}
						m_framenum1++;//��ʼֵΪ0����������Ч֡���м��������Ǿ�̬������
					    kl=1;//�ı�k��ִֵ����һ��������Ѱ����Ч����֡��
					}
		            break;
				}
				
			case 1:
				{
					if (energy[i]>m_energy1) 
					{
						numl++;
						for (j=0;j<FRAMELEN;j++)
						{
							m_vdata1[m_framenum1*FRAMELEN+j]=q[j];
						}
						m_framenum1++;
					}
					else//�������һ֡С��ƽ������������Ч֡���㣨��������Щ�������ݣ�
					{
						numl=0;
						m_framenum1=0;
						kl=0;
					}
					if (numl>6)//���������10֡����ʵ���ǲ�������������ƽ����������е�case 2
					{
						kl=2;
					}
					break;
				}
			case 2://����10��������Ч��������ʱ��0.1sʱ�䣩һ������ʱ��Ϊ0.01s,������ֵ����������Ǵ���ƽ����������С��ƽ������������Ч����
				{
					if (energy[i]>m_energy1) 
					{
						numl++;
						for (j=0;j<FRAMELEN;j++)
						{
							m_vdata1[m_framenum1*FRAMELEN+j]=q[j];
						}
						m_framenum1++;
						if (m_framenum1>200)//������Ч����ʱ�䳬��2S��������Щ�������ݣ�
						{
							m_framenum1=0;
							kl=0;
							numl=0;
							num1l=0;
						}
					}
					else//��������и�����С��ƽ��������ִ��case 3���ò�������Ϊ��Ч��������
					{
						numl++;
						num1l++;//�ж�����������־��1
						for (j=0;j<FRAMELEN;j++)
						{
							m_vdata1[m_framenum1*FRAMELEN+j]=q[j];
						}
						m_framenum1++;
						kl=3;
						if (m_framenum1>198)//֡����������Ч����ʱ�䳬��1.98S��������Щ�������ݣ�
						{
							m_framenum1=0;
							kl=0;
							numl=0;
							num1l=0;
						}
					}
				break;
				}
			case 3:
				{
					if (energy[i]<m_energy1)  //�ж���������
					{
						num1l++;
						if (num1l>10)//������������10֡����ʱȷ��Ϊ��Ч�����յ㣬��ʱ���Խ��в�����ȡ��ƥ���ˣ������������������0.1s
						{
							num1l=0;
							kl=4;
							m_rec=TRUE;
						}
					}
					else//�˴��������������㣬����ִ��case 2
					{
						numl++;
						num1l=0;//���ж�����������־����
						for (j=0;j<FRAMELEN;j++)
						{
							m_vdata1[m_framenum1*FRAMELEN+j]=q[j];
						}
						m_framenum1++;
						kl=2;
						if (m_framenum1>198)
						{
							m_framenum1=0;
							kl=0;
							numl=0;
							num1l=0;
						}
					}
				break;
				}
	        default:
				break;
		}
		q=q+FRAMELEN;//һ����������һ֡�������ݣ�������������Ҫƫ��һ��֡��
	}

	//MessageBox("��bu��");
	if (m_rec==TRUE)//¼����ʼ��ʹ��ΪFALSE,m_recΪ��һ���������Խ���MFCC������ȡ��DTWƥ��ģ���־
	{

		kl=0;
		numl=0;
		num1l=0;
		m_framenum1--;	
		if (m_framenum1<20||m_framenum1>300)//��������Ч����ʱ����0.35S��35*160/16000����1.5Sʱ�Ž��в�����ȡ��ƥ��
		{
			m_framenum1=0;
			m_rec=FALSE;
			MessageBox("����˵");
		}
		else
		{	
			waveInReset(m_hWaveIn); //ֹͣ¼��
	        waveInUnprepareHeader(m_hWaveIn,*m_pWaveHdr,sizeof(WAVEHDR));//�������

			/////////////////////////////////////
			CString wo;
	    	wo.Format("%d��FRAMELEN %d",m_framenum1,FRAMELEN);
		    //MessageBox("����"+wo);

			ofstream outfile1("aaaa.txt"); 


			short m_vdatamax1=fabs(m_vdata1[1]);//2015.11.16
			for(int aaa=0;aaa<m_framenum1*FRAMELEN;aaa++)//�����
			{	
				if(fabs(m_vdata1[aaa])>m_vdatamax1)
					m_vdatamax1=fabs(m_vdata1[aaa]);
			}

	//CString pMaxStr;
	//pMaxStr.Format("%d",m_vdatamax1);
	//AfxMessageBox(pMaxStr);

			for(aaa=0;aaa<m_framenum1*FRAMELEN;aaa++)
			{	
				m_vdata1[aaa]=10000*m_vdata1[aaa]/m_vdatamax1;
				outfile1<<m_vdata1[aaa]<<' ';//10000Ϊ��һ��
			}

            //AfxMessageBox("shsihsihi");

			/*for(int aaa=0;aaa<m_framenum1;aaa++)
			{
				for(int bbb=0;bbb<FRAMELEN;bbb++)
					outfile1<<m_vdata1[FRAMELEN*aaa+bbb]<<' ';
				//outfile1<<endl;
			}*/
			outfile1.close();


			////////////////////////////////////
			//mfcc=(float*)malloc(sizeof(float)*m_framenum1*MFCCNUM);//ΪMFCC��������洢�ռ䣬mfccΪͷָ��
			//m=mfcc;//��mfccͷָ�븳��m

			int mindex=mfccf(mfcc,m_vdata1,m_framenum1*FRAMELEN);	
            //AfxMessageBox("shsihsihi");

			////////////////ѹ��
            ifstream infileCB("cb.txt");

			ofstream outfileYs("aaaaѹ��¼��.txt");
			int VoiceLen=0;
			//infileYy>>VoiceLen;//��֡��
			VoiceLen=mindex;
			VoiceLen=(int)(VoiceLen*MFCCNUM/VEC_DEM);//��֡Ϊ��������������ת��Ϊ��ά���������������ȣ�VEC_DEM=36ά
			int* Code=(int*)calloc(VoiceLen,sizeof(int));//����֮�������
			double* Dispar=(double*)calloc(CB_SIZE,sizeof(double));
			int CodeBL=0;
			int i,j,vi;
			double CodeBook1[CB_SIZE][VEC_DEM];//����

			/////////////������/////////////
			for(i=0;i<CB_SIZE;i++)
			{
				for(j=0;j<VEC_DEM;j++)
				{
					infileCB>>CodeBook1[i][j];				
				}
			}
			infileCB.close();

			//��ʾ�����ĳ��ȣ���ά��������
			CString string;
			string.Format("%d",VoiceLen); 
			//MessageBox(string);

			//////////////���벻������ά��������������/////////////
			double** VoiceData=(double**)calloc(VoiceLen,sizeof(double*));
			for(int yi=0;yi<VoiceLen;yi++)
				 VoiceData[yi]=(double*)calloc(VEC_DEM,sizeof(double));

			///////////////////������������/////////////////
			for(i=0;i<VoiceLen;i++)
				for(j=0;j<VEC_DEM;j++)
				{
					//infileYy>>VoiceData[i][j];
					VoiceData[i][j]=(double)mfcc[i*VEC_DEM+j];
					//VoiceData[i][j]=m_vdata1[i*VEC_DEM+j];
				
				}		
			//infileYy.close();

			////////////////////��������////////////////
			for(i=0;i<VoiceLen;i++)
			{
				CodeBL=0;
				for(j=0;j<CB_SIZE;j++)
				{
					for (vi=0; vi<VEC_DEM; vi++)
						Dispar[j] += (VoiceData[i][vi] - CodeBook1[j][vi]) * (VoiceData[i][vi] - CodeBook1[j][vi]);
					
					if(j>0)
					{
						if(Dispar[j]<Dispar[CodeBL])
						{
							CodeBL=j;
						}	
					}
				}
				for(j=0;j<CB_SIZE;j++)
					Dispar[j]=0;
				Code[i]=CodeBL;
			}
			//¼���������������
			outfileYs<<VoiceLen<<' ';
			for(i=0;i<VoiceLen;i++)
				outfileYs<<1+Code[i]<<' ';
			outfileYs.close();

			/////////////////�ͷ��ڴ�///////////////////

			for(i=0;i<VoiceLen;i++)
			{
				free(VoiceData[i]);
				VoiceData[i]=NULL;
			}
			free(VoiceData);
			VoiceData=NULL;    
			free(Dispar);
			Dispar=NULL;
			///////////////ѹ��

			/////////////////////ʶ��
			HMM hmm[11];
			//��HMM
			FILE *fpHMM1;
			fpHMM1=fopen("���˹�һ_HMM.txt", "r");
			ReadHMM(fpHMM1,&hmm[1]);//2015.11.2
			fclose(fpHMM1);
			
			FILE *fpHMM2;
			fpHMM2=fopen("������һ_HMM.txt", "r");
			ReadHMM(fpHMM2,&hmm[5]);//2015.11.2
			fclose(fpHMM2);
			
			FILE *fpHMM3;
			fpHMM3=fopen("ƽ�ɹ�һ_HMM.txt", "r");
			ReadHMM(fpHMM3,&hmm[6]);//2015.11.2
			fclose(fpHMM3);
			
			FILE *fpHMM4;
			fpHMM4=fopen("ǰ����һ_HMM.txt", "r");
			ReadHMM(fpHMM4,&hmm[0]);//2015.11.2
			fclose(fpHMM4);

			FILE *fpHMM5;
			fpHMM5=fopen("ǰ���һ_HMM.txt", "r");
			ReadHMM(fpHMM5,&hmm[7]);//2015.11.2
			fclose(fpHMM5);

			FILE *fpHMM6;
			fpHMM6=fopen("������һ_HMM.txt", "r");
			ReadHMM(fpHMM6,&hmm[8]);//2015.11.2
			fclose(fpHMM6);
			
			FILE *fpHMM7;
			fpHMM7=fopen("ֹͣ��һ_HMM.txt", "r");
			ReadHMM(fpHMM7,&hmm[4]);//2015.11.2
			fclose(fpHMM7);
			
			FILE *fpHMM8;
			fpHMM8=fopen("�½���һ_HMM.txt", "r");
			ReadHMM(fpHMM8,&hmm[9]);//2015.11.2
			fclose(fpHMM8);
			
			FILE *fpHMM9;
			fpHMM9=fopen("��ת��һ_HMM.txt", "r");
			ReadHMM(fpHMM9,&hmm[3]);//2015.11.2
			fclose(fpHMM9);

			FILE *fpHMM10;
			fpHMM10=fopen("վ����һ_HMM.txt", "r");
			ReadHMM(fpHMM10,&hmm[10]);//2015.11.2
			fclose(fpHMM10);

			FILE *fpHMM11;
			fpHMM11=fopen("��ת��һ_HMM.txt", "r");
			ReadHMM(fpHMM11,&hmm[2]);//2015.11.2
			fclose(fpHMM11);
			
			int T;	
			//int i;
			int *O;	
			T=mindex*36/9;

		//	CString huhu;
		//	huhu.Format("%d",T);
		//	AfxMessageBox(huhu);

			O=(int *)calloc(T+1,sizeof(int));
		//	::ofstream  outfileCode("aaaaaaaaa.txt");////////////////////////////
        //    outfileCode<<T;///////////////////////////////////////
			for(i=1;i<=T;i++)
			{
				O[i]=1+Code[i-1];
		//		outfileCode<<O[i]<<' ';///////////////////////////
			}

			//�����ڴ�
			double **delta=(double **)calloc(T+1,sizeof(double *));
			for(i=0;i<=T;i++)
			{
				delta[i]=(double *)calloc(hmm[1].N+1,sizeof(double));
			}

			int **psi=(int **)calloc(T+1,sizeof(int *));
			for(i=0;i<=T;i++)
			{
				psi[i]=(int *)calloc(hmm[1].N+1,sizeof(int));
			}

			int *qViterbi = (int *)calloc(T+1,sizeof(int));

			double pViterbi[11];
			double pMax=-6000;
			int result;

			//Viterbi
			for(i=0;i<5;i++)//2015.11.2
			{
				ViterbiLog(&hmm[i], T, O, delta, psi, qViterbi, &pViterbi[i]);

			}

			for(i=0;i<5;i++)//2015.11.2
			{
				if(pViterbi[i]>pMax)
				{
					pMax=pViterbi[i];
					result=i;
				}
			}

			CString pMaxStr;
			pMaxStr.Format("%lf, %d",pMax,m_framenum1);
			AfxMessageBox(pMaxStr);

			switch(result)
			{
			case 0:	AfxMessageBox(pMaxStr+"  ǰ��");//2015.11.2
				break;
			case 1:	AfxMessageBox(pMaxStr+"  ����");//2015.11.2
				break;
			case 2:	AfxMessageBox(pMaxStr+"  ��ת");//2015.11.2
				break;
			case 3:	AfxMessageBox(pMaxStr+"  ��ת");//2015.11.2
				break;
			case 4:	AfxMessageBox(pMaxStr+"  ֹͣ");//2015.11.2
				break;
			case 5:	AfxMessageBox(pMaxStr+"  ����");//2015.11.2
				break;
			case 6:	AfxMessageBox(pMaxStr+"  ƽ��");//2015.11.2
				break;
			case 7:	AfxMessageBox(pMaxStr+"  ǰ��");//2015.11.2
				break;
			case 8:	AfxMessageBox(pMaxStr+"  ����");//2015.11.2
				break;
			case 9:	AfxMessageBox(pMaxStr+"  �½�");//2015.11.2
				break;
			case 10: AfxMessageBox(pMaxStr+"  վ��");//2015.11.2
				break;
			default: AfxMessageBox(pMaxStr+"  ʶ�����");
				break;
			}

			//�ͷ��ڴ�
			for(i=0;i<=T;i++)
			{
				free(delta[i]);
				delta[i]=NULL;
			}
			free(delta);
			delta=NULL;  

			for(i=0;i<=T;i++)
			{
				free(psi[i]);
				psi[i]=NULL;
			}
			free(psi);
			psi=NULL;  

			free(qViterbi);
			qViterbi=NULL;

			free(O);
			/////////////////////ʶ��
			free(Code);
			Code=NULL;
			
			free(mfcc);//�ͷ��ڴ�
			mfcc=NULL;
			PrePareRecord();//׼��¼��
	        StartRecord();//��ʼ¼��
		}
	}
	
	if (m_rec==FALSE)
	{
		waveInAddBuffer (m_hWaveIn, (PWAVEHDR) lParam, sizeof (WAVEHDR));//�������豸����һ������ 
	}
	m_rec=FALSE;

//	free(m_vdata1); //xieyanyi
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%>>


void CPjvoice2Dlg::OnTrain() 
{
	// TODO: Add your control notification handler code here
	void PreEmphasise (short *s, float k);

	float twopi;
	int hi;
	float Hamming[FrmLen];
	twopi=8.0F*atan(1.0F);
	for( hi=0;hi<FrmLen;hi++)
	{
		Hamming[hi]=(float)(0.54-0.46*cos((float)hi*twopi/(float)(FrmLen-1)));
	} 

	int codetrain=0;//��������ѵ����
	ofstream  outfileCode("����ѵ��.txt");
	for(int order=1;order<=ORDERNUM;order++)//���δ���11�����������
	{
		CString ordername;
		switch(order)
        {
		case 1:
			ordername="����";break;
		case 2:
			ordername="����";break;
		case 3:
			ordername="ƽ��";break;  
		case 4:
			ordername="ǰ��";break;
		case 5:
			ordername="ǰ��";break;
		case 6:
			ordername="����";break;
		case 7:
			ordername="ֹͣ";break;
		case 8:
			ordername="�½�";break;
		case 9:
			ordername="��ת";break;
		case 10:
			ordername="վ��";break;
		case 11:
			ordername="��ת";break;
        default:
            MessageBox("û�д�����");break;
        }

		for(int wj=1;wj<=WENJIAN;wj++)//����ÿ�������е�WENJIAN�������ļ�
		{  
			CString nameofwj;
			nameofwj.Format(ordername+"%d",wj);//��.wav�ļ�

			CString voicedata;
			voicedata.Format(ordername+"��������%d",wj);//��ʼ����

			TWavHeader waveheader;//��ȡ.wav�����ļ���ͷ��Ϣ
			int jc=0;                //����ͳ��.wav�ʼ������ĳ���
			int i;
			int m_framenum=0;         //�˵����������֡����֡��512�ģ�
			short buffer[FrmLen];      //�洢ÿ�δ�.wav�ʼ��ж�ȡ������
			FILE *sourcefile;           //��.wav�ļ�

			ofstream outfile1(voicedata+".txt");           //���˵��������ݶ��롰����.txt��
			outfile1.precision(4);

			sourcefile=fopen(nameofwj+".wav","rb");                     //��.wav�ļ�

			fread(&waveheader,sizeof(struct _TWavHeader),1,sourcefile);           //��ȡ����ͷ��Ϣ
			while(fread(buffer,sizeof(short),FrmLen,sourcefile)==FrmLen)           //��ȡ�����źŵĳ���
			{
				jc++;          //ͳ�������źŵĳ���
			}

			int cishu=0;        //����ͳ�ƶ�ȡ�ڼ��Σ������ȵ�ʱ���źţ�
			short *pi=(short *)calloc(jc*FrmLen,sizeof(short));    //Ϊ��ȡ�������źŷ����ڴ�
			short *p=pi;    

			fseek(sourcefile,sizeof(struct _TWavHeader),SEEK_SET);   //�ƶ��ļ�ָ��
			//fread(&waveheader,sizeof(struct _TWavHeader),1,sourcefile);
			while(fread(buffer,sizeof(short),FrmLen,sourcefile)==FrmLen)    //ʵ�ʶ�ȡ�������ݡ�һ֡����
			{
				for(int i=0;i<FrmLen;++i)
				{
					p[FrmLen*cishu+i]=buffer[i];                     //��buffer���������ȫ���ŵ�pָ��Ĵ洢�ռ�
				}			
				PreEmphasise((p+(FrmLen*cishu)),(float)0.97);
		        //PreEmphasise((p+(FrmLen*cishu)),(float)0.97);
				//PreEmphasise((p+(FrmLen*cishu)),(float)0.97);
				cishu++;
			}

		//	for(int shii=0;shii<cishu;shii++)
		//	{
		//		PreEmphasise((p+(FrmLen*(shii-1))),(float)0.97);
		//	}


		////////�˵���///////
			
			short *data,*q;  //�洢���źſ��ǲ���
			short *m_vdata; //�˵���֮�������
			float *energy;   //���ǲ���֮��ÿһ֡���ݵ�ƽ������
			int j,h;          
			int num=0,num1=0;      //���ڶ˵��⣬ͳ����Ч֡����Ч֡
			short kz=0;              //�˵����״̬
			float m_energy=0.0;    //ƽ������
			
			/*for (i=0;i<512*20;i++)//ȡ�û�������50����50+512*3������������ƽ������
			{
				m_energy+=(float)fabs((float)p[i]/120);//ƽ�����Ⱦ���ֵ�ۼӣ�����2000��������Ե���Ӧ���Ǹ���ʶ��Ч�������˵ģ�������Ϊʲô����
			}
			m_energy=(float)(m_energy/(float)50); //ƽ����������ʵ����ƽ�����ȣ�*/


			//m_energy=2333.00;//����һ֡�հ׵������õ���ƽ������������õ��ľ���ֵ
			m_energy=2333.00;

			//h=(int)(jc*FrmLen/2-(FRAMELEN-STEPLEN))/STEPLEN;  //���ǲ���֮���֡�����˴������⣨��Ӧ�ó���2��2015.4.2
			h=(int)(jc*FrmLen-(FRAMELEN-STEPLEN))/STEPLEN;  //2015.4.2

			CString ing3;//////�м����//////
			ing3.Format("initial %d",(int)(jc*FrmLen/FRAMELEN));
//			MessageBox(ing3);

			CString ing;//////�м����//////
			ing.Format("h %d",h);
//			MessageBox(ing);

			data=(short*)calloc(h*FRAMELEN,sizeof(short));//�洢���źſ��ǲ���
			energy=(float*)calloc(h,sizeof(float));//Ϊÿ֡ƽ����������洢�ռ䣬һ��h֡
			m_vdata=(short*)calloc(200*FRAMELEN,sizeof(short)); //���ڴ洢�˵���֮���������ÿ֡512

			q=data;//��dataͷָ�븳��q

			for (i=0;i<h;i++)//��֡�����h֡��������ÿ֡�Ķ�ʱ����
			{
				energy[i]=0.0;//ÿ֡�Ķ�ʱ����
				//HammingWindow(p);
				for (j=0;j<FRAMELEN;j++)//FRAMELENΪ����֡����������512����ÿ֡��ƽ�����ȣ�������������energy������
				{
					q[j]=(short)p[j];
					q[j]=q[j]*Hamming[j];
					energy[i]=energy[i]+(float)fabs(q[j]);
				}
				q=q+FRAMELEN;//�洢����ָ��ƫ������һ֡�Ĳ���������512��
				p=p+STEPLEN;
				//��������������ָ��ƫ��һ��������STEPLEN=160����Ҳ����˵ÿ��ֻ�ƶ�160����ʵ�ʴ洢��512�������Է����洢��һЩ�������ݡ�

			}
			q=data;//�ٴδ�ͷ��ַ��ʼ��������������Ч֡
			
			for (i=0;i<h;i++) //��ȡ��Ч����֡
			{

				switch(kz) //kz�ĳ�ʼֵΪ0����̬������
				{
					case 0:
						{
							if (energy[i]>m_energy) //�����֡ƽ����������m_energy����֡Ϊ��Ч��������
							{
								num++;//������ʼֵΪ0����������Ч֡���м���
								for (j=0;j<FRAMELEN;j++)//�Ѹ�֡���ݸ���m_vdata
								{
									m_vdata[m_framenum*FRAMELEN+j]=q[j];//����200֡��¼��ʱ��Ϊ6.4S
								}
								//ing4.Format("%d",i);
								//MessageBox(ing4);
								m_framenum++;//��ʼֵΪ0����������Ч֡���м��������Ǿ�̬������
								kz=1;//�ı�kz��ִֵ����һ��������Ѱ����Ч����֡��
							}
							break;
						}
						
					case 1:
						{
							if (energy[i]>m_energy) 
							{
								num++;
								for (j=0;j<FRAMELEN;j++)
								{
									m_vdata[m_framenum*FRAMELEN+j]=q[j];
								}
								//ing4.Format("%d",i);
								//MessageBox(ing4);
								m_framenum++;
							}
							else//�������һ֡С��ƽ������������Ч֡���㣨��������Щ�������ݣ�
							{
								num=0;
								m_framenum=0;
								kz=0;
							}
							if (num>10)//���������10֡����ʵ���ǲ�������������ƽ����������е�case 2
							{
								kz=2;
							}
							break;
						}
					case 2://����10��������Ч��������ʱ��0.1sʱ�䣩һ������ʱ��Ϊ0.01s,������ֵ����������Ǵ���ƽ����������С��ƽ������������Ч����
						{
							if (energy[i]>m_energy) 
							{
								num++;
								for (j=0;j<FRAMELEN;j++)
								{
									m_vdata[m_framenum*FRAMELEN+j]=q[j];
								}
								//ing4.Format("%d",i);
								//MessageBox(ing4);
								m_framenum++;
								if (num>200)//������Ч����ʱ�䳬��2S��������Щ�������ݣ�
								{
									m_framenum=0;
									kz=0;
									num=0;
									num1=0;
								}
							}
							else//��������и�����С��ƽ��������ִ��case 3���ò�������Ϊ��Ч��������
							{
								num++;
								num1++;//�ж�����������־��1
								for (j=0;j<FRAMELEN;j++)
								{
									m_vdata[m_framenum*FRAMELEN+j]=q[j];
								}
								//ing4.Format("%d",i);
								//MessageBox(ing4);
								m_framenum++;
								kz=3;
								if (num>198)//֡����������Ч����ʱ�䳬��1.98S��������Щ�������ݣ�
								{
									m_framenum=0;
									kz=0;
									num=0;
									num1=0;
								}
							}
						break;
						}
					case 3:
						{
							if (energy[i]<m_energy)  //�ж���������
							{
								num1++;
								if (num1>10)//������������10֡����ʱȷ��Ϊ��Ч�����յ㣬��ʱ���Խ��в�����ȡ��ƥ���ˣ������������������0.1s
								{
									num1=0;
									kz=4;
								}
							}
							else//�˴��������������㣬����ִ��case 2
							{
								num++;
								num1=0;//���ж�����������־����
								for (j=0;j<FRAMELEN;j++)
								{
									m_vdata[m_framenum*FRAMELEN+j]=q[j];
								}
								//ing4.Format("%d",i);
								//MessageBox(ing4);
								m_framenum++;
								kz=2;
								if (m_framenum>198)
								{
									m_framenum=0;
									kz=0;
									num=0;
									num1=0;
								}
							}
						break;
						}
					default:
						break;
				}
				q=q+FRAMELEN;//һ����������һ֡�������ݣ�������������Ҫƫ��һ��֡��

			}

			CString ingh;//////�м����//////
			ingh.Format("m_framenum %d",m_framenum);
//			MessageBox(ingh);
			/////////�˵������////////

			/*short m_vdatamax=fabs(m_vdata[1]);
			for(i=0;i<m_framenum*FRAMELEN;i++)//�����
			{	
				if(fabs(m_vdata[i])>m_vdatamax)
					m_vdatamax=fabs(m_vdata[i]);
			}

			for(i=0;i<m_framenum*FRAMELEN;i++)
			{	
				outfile1<<10000*m_vdata[i]/m_vdatamax<<' ';//10000Ϊ��һ��
			}*/

			short sum=0;
			for(i=0;i<m_framenum*FRAMELEN;i++)//�����
			{	
				sum+=m_vdata[i]*m_vdata[i];
			}

			for(i=0;i<m_framenum*FRAMELEN;i++)
			{	
				outfile1<<short(1000*m_vdata[i]/sqrt(sum))<<' ';//10000Ϊ��һ��
			}


			/*outfile1<<"�����ź���֡���� "<<m_framenum;
			  
			outfile1<<"��Ƶ���ݵĴ�С: "<<waveheader.wSampleLength<<endl; 
			outfile1<<"��־����RIFF��: "<<waveheader.rId<<endl;    
			outfile1<<"���ݴ�С,��������ͷ�Ĵ�С����Ƶ�ļ��Ĵ�С: "<<waveheader.rLen<<endl;    //���ݴ�С,��������ͷ�Ĵ�С����Ƶ�ļ��Ĵ�С
			outfile1<<"��ʽ���ͣ�WAVE��: "<<waveheader.wId<<endl;       //��ʽ���ͣ�"WAVE"��
			outfile1<<"fmt: "<<waveheader.fId<<endl; 
			outfile1<<"Sizeof(WAVEFORMATEX): "<<waveheader.fLen<<endl; 
			outfile1<<"�����ʽ������WAVE_FORMAT_PCM��WAVEFORMAT_ADPCM��: "<<waveheader.wFormatTag<<endl;   
			outfile1<<"��������������Ϊ1��˫����Ϊ2: "<<waveheader.nChannels<<endl;   
			outfile1<<"����Ƶ��: "<<waveheader.nSamplesPerSec<<endl; 
   			outfile1<<"ÿ���������: "<<waveheader.nAvgBytesPerSec<<endl;      
			outfile1<<"�����: "<<waveheader.nBlockAlign<<endl;  
			outfile1<<"WAVE�ļ��Ĳ�����С: "<<waveheader.wBitsPerSample<<endl; 
  			outfile1<<"data: "<<waveheader.dId<<endl;   
			*/
			outfile1.close();
			fclose(sourcefile);

			CString mfccData;
			mfccData.Format(ordername+"mfcc%d",wj);
			ofstream outfileMfcc(mfccData+".txt");  //�洢mfcc����
			ifstream ifile(voicedata+".txt");     //������������

			codetrain+=mfccd(outfileMfcc,ifile,outfileCode);      //������ȡ

			free(pi);pi=NULL;
			free(data);data=NULL;
			free(energy);energy=NULL;
			free(m_vdata);m_vdata=NULL;

			ifile.close();
			outfileMfcc.close();
			}
	}
	outfileCode.seekp(0, ios::beg); //��������ѵ������
	outfileCode<<codetrain<<' ';
	outfileCode.close();

	CString ing;
	ing.Format("xulian %d",codetrain);
	MessageBox(ing);
}

void CPjvoice2Dlg::OnRecog() 
{
	// TODO: Add your control notification handler code here
	HMM hmm[11];

	//��HMM
	FILE *fpHMM1;
	fpHMM1=fopen("���˹�һ_HMM.txt", "r");
	ReadHMM(fpHMM1,&hmm[0]);
	fclose(fpHMM1);
	
	FILE *fpHMM2;
	fpHMM2=fopen("������һ_HMM.txt", "r");
	ReadHMM(fpHMM2,&hmm[1]);
	fclose(fpHMM2);
	
	FILE *fpHMM3;
	fpHMM3=fopen("ƽ�ɹ�һ_HMM.txt", "r");
	ReadHMM(fpHMM3,&hmm[2]);
	fclose(fpHMM3);
	
	FILE *fpHMM4;
	fpHMM4=fopen("ǰ����һ_HMM.txt", "r");
	ReadHMM(fpHMM4,&hmm[3]);
	fclose(fpHMM4);

	FILE *fpHMM5;
	fpHMM5=fopen("ǰ���һ_HMM.txt", "r");
	ReadHMM(fpHMM5,&hmm[4]);
	fclose(fpHMM5);

	FILE *fpHMM6;
	fpHMM6=fopen("������һ_HMM.txt", "r");
	ReadHMM(fpHMM6,&hmm[5]);
	fclose(fpHMM6);
	
	FILE *fpHMM7;
	fpHMM7=fopen("ֹͣ��һ_HMM.txt", "r");
	ReadHMM(fpHMM7,&hmm[6]);
	fclose(fpHMM7);
	
	FILE *fpHMM8;
	fpHMM8=fopen("�½���һ_HMM.txt", "r");
	ReadHMM(fpHMM8,&hmm[7]);
	fclose(fpHMM8);
	
	FILE *fpHMM9;
	fpHMM9=fopen("��ת��һ_HMM.txt", "r");
	ReadHMM(fpHMM9,&hmm[8]);
	fclose(fpHMM9);

	FILE *fpHMM10;
	fpHMM10=fopen("վ����һ_HMM.txt", "r");
	ReadHMM(fpHMM10,&hmm[9]);
	fclose(fpHMM10);

	FILE *fpHMM11;
	fpHMM11=fopen("��ת��һ_HMM.txt", "r");
	ReadHMM(fpHMM11,&hmm[10]);
	fclose(fpHMM11);
	
	//������
	FILE *fVoice;	
	//fVoice=fopen("���1ǰ��ѹ������.txt","r");		
	//fVoice=fopen("���1����ѹ������.txt","r");
	//fVoice=fopen("���1����ѹ������.txt","r");	
	//fVoice=fopen("���1ƽ��ѹ������.txt","r");	
	//fVoice=fopen("���1ǰ��ѹ������.txt","r");	
	fVoice=fopen("���1����ѹ������.txt","r");	
	//fVoice=fopen("���1ֹͣѹ������.txt","r");	
	//fVoice=fopen("���1�½�ѹ������.txt","r");
	//fVoice=fopen("���1��תѹ������.txt","r");	
	//fVoice=fopen("���1վ��ѹ������.txt","r");	
	//fVoice=fopen("���1��תѹ������.txt","r");	


	//"����";"����";"ƽ��";"ǰ��";"ǰ��";"����";"ֹͣ";"�½�";"��ת""վ��";"��ת"
	int T;	
	int i;
	int *O;	
	fscanf(fVoice,"%d",&(T));
	O=(int *)calloc(T+1,sizeof(int));
	for(i=1;i<=T;i++)
	{
		fscanf(fVoice,"%d",&(O[i]));
	}
	fclose(fVoice);

	//�����ڴ�
	double **delta=(double **)calloc(T+1,sizeof(double *));
	for(i=0;i<=T;i++)
	{
		delta[i]=(double *)calloc(hmm[1].N+1,sizeof(double));
	}

	int **psi=(int **)calloc(T+1,sizeof(int *));
	for(i=0;i<=T;i++)
	{
		psi[i]=(int *)calloc(hmm[1].N+1,sizeof(int));
	}

	int *qViterbi = (int *)calloc(T+1,sizeof(int));

	double pViterbi[11];
	double pMax=-6000;
	int result;

	//Viterbi
	for(i=0;i<11;i++)
	{
		ViterbiLog(&hmm[i], T, O, delta, psi, qViterbi, &pViterbi[i]);

	}

	for(i=0;i<11;i++)
	{
		if(pViterbi[i]>pMax)
		{
			pMax=pViterbi[i];
			result=i;
		}
	}

	CString pMaxStr;
	pMaxStr.Format("%lf",pMax);
	AfxMessageBox(pMaxStr);

	switch(result)
	{
	case 0:	AfxMessageBox("����");
		break;
	case 1:	AfxMessageBox("����");
		break;
	case 2:	AfxMessageBox("ƽ��");
		break;
	case 3:	AfxMessageBox("ǰ��");
		break;
	case 4:	AfxMessageBox("ǰ��");
		break;
	case 5:	AfxMessageBox("����");
		break;
	case 6:	AfxMessageBox("ֹͣ");
		break;
	case 7:	AfxMessageBox("�½�");
		break;
	case 8:	AfxMessageBox("��ת");
		break;
	case 9:	AfxMessageBox("վ��");
		break;
	case 10: AfxMessageBox("��ת");
		break;
	default: AfxMessageBox("ʶ�����");
		break;
	}


	//�ͷ��ڴ�
	for(i=0;i<=T;i++)
	{
		free(delta[i]);
		delta[i]=NULL;
	}
	free(delta);
	delta=NULL;  

	for(i=0;i<=T;i++)
	{
		free(psi[i]);
		psi[i]=NULL;
	}
	free(psi);
	psi=NULL;  

	free(qViterbi);
	qViterbi=NULL;

	free(O);
}

void CPjvoice2Dlg::OnMashuxunlian() 
{
	// TODO: Add your control notification handler code here

	Lbg();
}

void CPjvoice2Dlg::OnHmmInit() 
{
	// TODO: Add your control notification handler code here
	HMM hmm1;
	int N=5;
	int M=128;
	int seed=100;
	float wucha=0;
	InitHMM(&hmm1,N,M,seed);

	AfxMessageBox("ok");
}

void CPjvoice2Dlg::OnShiHua() 
{
	// TODO: Add your control notification handler code here
	for(int order=1;order<=ORDERNUM;order++)//���δ���11�����������
	{
		CString ordername;
		switch(order)
        {
		case 1:
			ordername="����";break;
		case 2:
			ordername="����";break;
		case 3:
			ordername="ƽ��";break;  
		case 4:
			ordername="ǰ��";break;
		case 5:
			ordername="ǰ��";break;
		case 6:
			ordername="����";break;
		case 7:
			ordername="ֹͣ";break;
		case 8:
			ordername="�½�";break;
		case 9:
			ordername="��ת";break;
		case 10:
			ordername="վ��";break;
		case 11:
			ordername="��ת";break;
        default:
            MessageBox("û�д�����");break;
        }

		for(int wj=1;wj<=WENJIAN;wj++)//����ÿ�������е�WENJIAN�������ļ�
		{	
			ifstream infileCB("cb.txt");

			CString nameMfcc;
			nameMfcc.Format("mfcc%d",wj);
			ifstream infileYy(ordername+nameMfcc+".txt");	

			CString nameYasuo;
			nameYasuo.Format("ѹ������%d",wj);

			ofstream outfileYs(ordername+nameYasuo+".txt");
			int VoiceLen=0;
			infileYy>>VoiceLen;//��֡��
			VoiceLen=(int)(VoiceLen*MFCCNUM/VEC_DEM);//��֡Ϊ��������������ת��Ϊ��ά���������������ȣ�VEC_DEM=36ά
			int* Code=(int*)calloc(VoiceLen,sizeof(int));//����֮�������
			double* Dispar=(double*)calloc(CB_SIZE,sizeof(double));
			int CodeBL=0;
			int i,j,vi;
			double CodeBook1[CB_SIZE][VEC_DEM];//����

			/***************������***************/
			for(i=0;i<CB_SIZE;i++)
			{
				for(j=0;j<VEC_DEM;j++)
				{
					infileCB>>CodeBook1[i][j];				
				}
			}
			infileCB.close();

			//��ʾ�����ĳ��ȣ���ά��������
			CString string;
			string.Format("%d",VoiceLen); 
			//MessageBox(string);

			/*********���벻������ά��������������**********/
			double** VoiceData=(double**)calloc(VoiceLen,sizeof(double*));
			for(int yi=0;yi<VoiceLen;yi++)
				 VoiceData[yi]=(double*)calloc(VEC_DEM,sizeof(double));

			/*********�������ļ��ж�����������**********/
			for(i=0;i<VoiceLen;i++)
				for(j=0;j<VEC_DEM;j++)
				{
					infileYy>>VoiceData[i][j];
				}		
			infileYy.close();

			/*********��������**********/
			for(i=0;i<VoiceLen;i++)
			{
				CodeBL=0;
				for(j=0;j<CB_SIZE;j++)
				{
					for (vi=0; vi<VEC_DEM; vi++)
						Dispar[j] += (VoiceData[i][vi] - CodeBook1[j][vi]) * (VoiceData[i][vi] - CodeBook1[j][vi]);
					
					if(j>0)
					{
						if(Dispar[j]<Dispar[CodeBL])
						{
							CodeBL=j;
						}	
					}
				}
				for(j=0;j<CB_SIZE;j++)
					Dispar[j]=0;
				Code[i]=CodeBL;
			}
			//¼���������������
			outfileYs<<VoiceLen<<' ';
			for(i=0;i<VoiceLen;i++)
				outfileYs<<1+Code[i]<<' ';
			outfileYs.close();

			/**********�ͷ��ڴ�***********/
			free(Code);
			Code=NULL;
			for(i=0;i<VoiceLen;i++)
			{
				free(VoiceData[i]);
				VoiceData[i]=NULL;
			}
			free(VoiceData);
			VoiceData=NULL;    
			free(Dispar);
			Dispar=NULL;
		}
	}

	AfxMessageBox("ok");
}

void CPjvoice2Dlg::OnTrainHMM() 
{
	// TODO: Add your control notification handler code here
    //��HMM
	for(int order=1;order<=ORDERNUM;order++)//���δ���11�����������
	{

		FILE *fp;
		fp=fopen("��ʼHMM.txt", "r");
		HMM hmm;
		ReadHMM(fp,&hmm);
		fclose(fp);

		CString ordername;
		switch(order)
        {
		case 1:
			ordername="����";break;
		case 2:
			ordername="����";break;
		case 3:
			ordername="ƽ��";break;  
		case 4:
			ordername="ǰ��";break;
		case 5:
			ordername="ǰ��";break;
		case 6:
			ordername="����";break;
		case 7:
			ordername="ֹͣ";break;
		case 8:
			ordername="�½�";break;
		case 9:
			ordername="��ת";break;
		case 10:
			ordername="վ��";break;
		case 11:
			ordername="��ת";break;
        default:
            MessageBox("û�д�����");break;
        }
		
		int *T=(int *)calloc(WENJIAN+1,sizeof(int));
		int i=0;
		int **O=(int **)calloc(WENJIAN+1,sizeof(int*));
		//������

        for(int wj=1;wj<=WENJIAN;wj++)//����ÿ�������е�WENJIAN�������ļ�
		{
			FILE *fVoice;	
			CString xuhao;
			xuhao.Format("ѹ������%d",wj);
			fVoice=fopen(ordername+xuhao+".txt","r");
			fscanf(fVoice,"%d",&(T[wj]));

			O[wj]=(int *)calloc(T[wj]+1,sizeof(int));

			for(int j=1;j<=T[wj];j++)
			{
				fscanf(fVoice,"%d",&(O[wj][j]));
			}
			fclose(fVoice);	
		}

//		int pniter;
		double plogprobinit;
		double plogprobfinal;
		//double pViterbi;
		//�����ڴ�
		double ***alpha=(double ***)calloc(WENJIAN+1,sizeof(double **));
		for(i=0;i<=WENJIAN;i++)
		{
			alpha[i]=(double **)calloc(T[i]+1,sizeof(double *));
			for(int j=0;j<=T[i];j++)
			{
				alpha[i][j]=(double *)calloc(hmm.N+1,sizeof(double));
			}
		}

		double ***alphaStar=(double ***)calloc(WENJIAN+1,sizeof(double **));
		for(i=0;i<=WENJIAN;i++)
		{
			alphaStar[i]=(double **)calloc(T[i]+1,sizeof(double *));
			for(int j=0;j<=T[i];j++)
			{
				alphaStar[i][j]=(double *)calloc(hmm.N+1,sizeof(double));
			}
		}

		double ***alphaWave=(double ***)calloc(WENJIAN+1,sizeof(double **));
		for(i=0;i<=WENJIAN;i++)
		{
			alphaWave[i]=(double **)calloc(T[i]+1,sizeof(double *));
			for(int j=0;j<=T[i];j++)
			{
				alphaWave[i][j]=(double *)calloc(hmm.N+1,sizeof(double));
			}
		}

		double ***beta=(double ***)calloc(WENJIAN+1,sizeof(double **));
		for(i=0;i<=WENJIAN;i++)
		{
			beta[i]=(double **)calloc(T[i]+1,sizeof(double *));
			for(int j=0;j<=T[i];j++)
			{
				beta[i][j]=(double *)calloc(hmm.N+1,sizeof(double));
			}
		}

		double ***betaStar=(double ***)calloc(WENJIAN+1,sizeof(double **));
		for(i=0;i<=WENJIAN;i++)
		{
			betaStar[i]=(double **)calloc(T[i]+1,sizeof(double *));
			for(int j=0;j<=T[i];j++)
			{
				betaStar[i][j]=(double *)calloc(hmm.N+1,sizeof(double));
			}
		}

		double ***betaWave=(double ***)calloc(WENJIAN+1,sizeof(double **));
		for(i=0;i<=WENJIAN;i++)
		{
			betaWave[i]=(double **)calloc(T[i]+1,sizeof(double *));
			for(int j=0;j<=T[i];j++)
			{
				betaWave[i][j]=(double *)calloc(hmm.N+1,sizeof(double));
			}
		}

		
		double ***gamma=(double ***)calloc(WENJIAN+1,sizeof(double **));
		for(i=0;i<=WENJIAN;i++)
		{
			gamma[i]=(double **)calloc(T[i]+1,sizeof(double *));
			for(int j=0;j<=T[i];j++)
			{
				gamma[i][j]=(double *)calloc(hmm.N+1,sizeof(double));
			}
		}


		double **alphaWaveSum=(double **)calloc(WENJIAN+1,sizeof(double*));
		for(i=0;i<=WENJIAN;i++)
		{			
			alphaWaveSum[i]=(double *)calloc(T[i]+1,sizeof(double));
		}

		//BaumWelch
		BaumWelch(&hmm,T,O,alpha,alphaStar,alphaWave,alphaWaveSum,
			beta,betaStar,betaWave,gamma,
			/*&pniter,*/&plogprobinit,&plogprobfinal);

		//д��HMM
		FILE *fWriteHMM;
		fWriteHMM=fopen(ordername+"_HMM.txt", "w");
		PrintHMM(fWriteHMM,&hmm);
		fclose(fWriteHMM);

		//�ͷ��ڴ�
		for(i=0;i<=WENJIAN;i++)
		{
			for(int j=0;j<=T[i];j++)
			{
				free(alpha[i][j]);
				alpha[i][j]=NULL;
			}
			free(alpha[i]);
			alpha[i]=NULL;  
		}
		free(alpha);
	    alpha=NULL; 
		
		for(i=0;i<=WENJIAN;i++)
		{
			for(int j=0;j<=T[i];j++)
			{
				free(alphaStar[i][j]);
				alphaStar[i][j]=NULL;
			}
			free(alphaStar[i]);
			alphaStar[i]=NULL;  
		}
		free(alphaStar);
	    alphaStar=NULL;  

		for(i=0;i<=WENJIAN;i++)
		{
			for(int j=0;j<=T[i];j++)
			{
				free(alphaWave[i][j]);
				alphaWave[i][j]=NULL;
			}
			free(alphaWave[i]);
			alphaWave[i]=NULL;  
		}
		free(alphaWave);
	    alphaWave=NULL;
		
		for(i=0;i<=WENJIAN;i++)
		{
			for(int j=0;j<=T[i];j++)
			{
				free(beta[i][j]);
				beta[i][j]=NULL;
			}
			free(beta[i]);
			beta[i]=NULL;  
		}
		free(beta);
	    beta=NULL; 
		
		for(i=0;i<=WENJIAN;i++)
		{
			for(int j=0;j<=T[i];j++)
			{
				free(betaStar[i][j]);
				betaStar[i][j]=NULL;
			}
			free(betaStar[i]);
			betaStar[i]=NULL;  
		}
		free(betaStar);
	    betaStar=NULL;  

		for(i=0;i<=WENJIAN;i++)
		{
			for(int j=0;j<=T[i];j++)
			{
				free(betaWave[i][j]);
				betaWave[i][j]=NULL;
			}
			free(betaWave[i]);
			betaWave[i]=NULL;  
		}
		free(betaWave);
	    betaWave=NULL;  

		for(i=0;i<=WENJIAN;i++)
		{
			for(int j=0;j<=T[i];j++)
			{
				free(gamma[i][j]);
			    gamma[i][j]=NULL;
			}
			free(gamma[i]);
			gamma[i]=NULL;  
		}
		free(gamma);
	    gamma=NULL;  

		for(i=0;i<=WENJIAN;i++)
		{
			free(alphaWaveSum[i]);
			alphaWaveSum[i]=NULL;
		}
		free(alphaWaveSum);
		alphaWaveSum=NULL;  

		for(i=0;i<=WENJIAN;i++)
		{
			free(O[i]);
			O[i]=NULL;
		}
		free(O);
		O=NULL;  

		free(T);
		T=NULL;
	}

	AfxMessageBox("ok");
}

void CPjvoice2Dlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	HMM hmm[11];

	//��HMM
	FILE *fpHMM1;
	fpHMM1=fopen("����_HMM.txt", "r");
	ReadHMM(fpHMM1,&hmm[0]);
	fclose(fpHMM1);
	
	FILE *fpHMM2;
	fpHMM2=fopen("����_HMM.txt", "r");
	ReadHMM(fpHMM2,&hmm[1]);
	fclose(fpHMM2);
	
	FILE *fpHMM3;
	fpHMM3=fopen("ƽ��_HMM.txt", "r");
	ReadHMM(fpHMM3,&hmm[2]);
	fclose(fpHMM3);
	
	FILE *fpHMM4;
	fpHMM4=fopen("ǰ��_HMM.txt", "r");
	ReadHMM(fpHMM4,&hmm[3]);
	fclose(fpHMM4);

	FILE *fpHMM5;
	fpHMM5=fopen("ǰ��_HMM.txt", "r");
	ReadHMM(fpHMM5,&hmm[4]);
	fclose(fpHMM5);

	FILE *fpHMM6;
	fpHMM6=fopen("����_HMM.txt", "r");
	ReadHMM(fpHMM6,&hmm[5]);
	fclose(fpHMM6);
	
	FILE *fpHMM7;
	fpHMM7=fopen("ֹͣ_HMM.txt", "r");
	ReadHMM(fpHMM7,&hmm[6]);
	fclose(fpHMM7);
	
	FILE *fpHMM8;
	fpHMM8=fopen("�½�_HMM.txt", "r");
	ReadHMM(fpHMM8,&hmm[7]);
	fclose(fpHMM8);
	
	FILE *fpHMM9;
	fpHMM9=fopen("��ת_HMM.txt", "r");
	ReadHMM(fpHMM9,&hmm[8]);
	fclose(fpHMM9);

	FILE *fpHMM10;
	fpHMM10=fopen("վ��_HMM.txt", "r");
	ReadHMM(fpHMM10,&hmm[9]);
	fclose(fpHMM10);

	FILE *fpHMM11;
	fpHMM11=fopen("��ת_HMM.txt", "r");
	ReadHMM(fpHMM11,&hmm[10]);
	fclose(fpHMM11);
	
	for(int i=0; i<11; i++)
	{ 
		//A�Ĺ�һ��
		for(int j=1;j<=hmm[i].N; j++)
		{
			double sum1=0.0;
			for(int k=1; k<=hmm[i].N ; k++)
			{
				sum1=sum1+hmm[i].A[j][k];
			}
			for(k=1; k<=hmm[i].N ;k++)
			{
				hmm[i].A[j][k]=hmm[i].A[j][k]/sum1;
			}
		}

		//B�Ĺ�һ��
		for(j=1;j<=hmm[i].N; j++)
		{
			double sum2=0.0;
			for(int k=1; k<=hmm[i].M ; k++)
			{
				sum2=sum2+hmm[i].B[j][k];
			}
			for(k=1; k<=hmm[i].M ;k++)
			{
				hmm[i].B[j][k]=hmm[i].B[j][k]/sum2;
			}
		}

		//pi�Ĺ�һ��		
		double sum3=0.0;
		for(j=1;j<=hmm[i].N; j++)
		{
			sum3=sum3+hmm[i].pi[j];
		}
		for(j=1;j<=hmm[i].N; j++)
		{
			hmm[i].pi[j]=hmm[i].pi[j]/sum3;
		}
	}
//"����";"����";"ƽ��";"ǰ��";"ǰ��";"����";"ֹͣ";"�½�";"��ת";"վ��";"��ת"

	FILE *fWriteHMM1;
	fWriteHMM1=fopen("���˹�һ_HMM.txt", "w");
	PrintHMM(fWriteHMM1,&hmm[0]);
	fclose(fWriteHMM1);

	FILE *fWriteHMM2;
	fWriteHMM2=fopen("������һ_HMM.txt", "w");
	PrintHMM(fWriteHMM2,&hmm[1]);
	fclose(fWriteHMM2);

	FILE *fWriteHMM3;
	fWriteHMM3=fopen("ƽ�ɹ�һ_HMM.txt", "w");
	PrintHMM(fWriteHMM3,&hmm[2]);
	fclose(fWriteHMM3);

	FILE *fWriteHMM4;
	fWriteHMM4=fopen("ǰ����һ_HMM.txt", "w");
	PrintHMM(fWriteHMM4,&hmm[3]);
	fclose(fWriteHMM4);

	FILE *fWriteHMM5;
	fWriteHMM5=fopen("ǰ���һ_HMM.txt", "w");
	PrintHMM(fWriteHMM5,&hmm[4]);
	fclose(fWriteHMM5);

	FILE *fWriteHMM6;
	fWriteHMM6=fopen("������һ_HMM.txt", "w");
	PrintHMM(fWriteHMM6,&hmm[5]);
	fclose(fWriteHMM6);

	FILE *fWriteHMM7;
	fWriteHMM7=fopen("ֹͣ��һ_HMM.txt", "w");
	PrintHMM(fWriteHMM7,&hmm[6]);
	fclose(fWriteHMM7);

	FILE *fWriteHMM8;
	fWriteHMM8=fopen("�½���һ_HMM.txt", "w");
	PrintHMM(fWriteHMM8,&hmm[7]);
	fclose(fWriteHMM8);

	FILE *fWriteHMM9;
	fWriteHMM9=fopen("��ת��һ_HMM.txt", "w");
	PrintHMM(fWriteHMM9,&hmm[8]);
	fclose(fWriteHMM9);

	FILE *fWriteHMM10;
	fWriteHMM10=fopen("վ����һ_HMM.txt", "w");
	PrintHMM(fWriteHMM10,&hmm[9]);
	fclose(fWriteHMM10);

	FILE *fWriteHMM11;
	fWriteHMM11=fopen("��ת��һ_HMM.txt", "w");
	PrintHMM(fWriteHMM11,&hmm[10]);
	fclose(fWriteHMM11);

	AfxMessageBox("ok");
}

void CPjvoice2Dlg::OnButton8() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CString str;//�����ַ���
	GetDlgItem(IDC_BUTTON8)->GetWindowText(str);//��ȡ��ť�Ի���������ַ���
	if (str=="¼��ʶ��")//�԰�ť����������ʾ����record
	{
		//GetDlgItem(IDC_BUTTON8)->SetWindowText("stop");//record->stop
		PrePareRecord();//׼��ʶ��
	    StartRecord();//��ʼʶ��
		GetDlgItem(IDC_BUTTON8)->SetWindowText("����ʶ��");
	}
	else//��ť�Ի���Ҳ���Ǳ��stop
	{
		waveInReset(m_hWaveIn);//ֹͣ¼��
	    waveInUnprepareHeader(m_hWaveIn,*m_pWaveHdr,sizeof(WAVEHDR));//�������
		GetDlgItem(IDC_BUTTON8)->SetWindowText("¼��ʶ��");//stop->record�Ի�����stop���record
	}
}

void CPjvoice2Dlg::OnYongyushibie() 
{
	// TODO: Add your control notification handler code here

	void PreEmphasise (short *s, float k);

	float twopi;
	int hi;
	float Hamming[FrmLen];
	twopi=8.0F*atan(1.0F);
	for( hi=0;hi<FrmLen;hi++)
	{
		Hamming[hi]=(float)(0.54-0.46*cos((float)hi*twopi/(float)(FrmLen-1)));
	} 

	int codetrain=0;
	ofstream  outfileCode("������������û�õ�.txt");
	for(int order=1;order<=ORDERNUM;order++)//���δ���11�����������
	{
		CString ordername;
		switch(order)
        {
		case 1:
			ordername="����";break;
		case 2:
			ordername="����";break;
		case 3:
			ordername="ƽ��";break;  
		case 4:
			ordername="ǰ��";break;
		case 5:
			ordername="ǰ��";break;
		case 6:
			ordername="����";break;
		case 7:
			ordername="ֹͣ";break;
		case 8:
			ordername="�½�";break;
		case 9:
			ordername="��ת";break;
		case 10:
			ordername="վ��";break;
		case 11:
			ordername="��ת";break;
        default:
            MessageBox("û�д�����");break;
        }

		for(int wj=1;wj<=1;wj++)//����ÿ�������е�WENJIAN�������ļ�
		{  
			CString nameofwj;
			//nameofwj.Format("���%d"+ordername+"_32k16b.wav",wj);
			nameofwj.Format("���"+ordername+".wav",wj);

			CString voicedata;
			voicedata.Format("���%d"+ordername+"��������",wj);

			TWavHeader waveheader;//��ȡ.wav�����ļ���ͷ��Ϣ
			int jc=0;                //����ͳ��.wav�ʼ������ĳ���
			int i;
			int m_framenum=0;         //�˵����������֡����֡��512�ģ�
			short buffer[FrmLen];      //�洢ÿ�δ�.wav�ʼ��ж�ȡ������
			FILE *sourcefile;           //��.wav�ļ�

			ofstream outfile1(voicedata+".txt");           //���˵��������ݶ��롰����.txt��
			outfile1.precision(4);

			sourcefile=fopen(nameofwj,"rb");                     //��.wav�ļ�

			fread(&waveheader,sizeof(struct _TWavHeader),1,sourcefile);           //��ȡ����ͷ��Ϣ
			while(fread(buffer,sizeof(short),FrmLen,sourcefile)==FrmLen)           //��ȡ�����źŵĳ���
			{
				jc++;          //ͳ�������źŵĳ���
			}

			int cishu=0;        //����ͳ�ƶ�ȡ�ڼ��Σ������ȵ�ʱ���źţ�
			short *pi=(short *)calloc(jc*FrmLen,sizeof(short));    //Ϊ��ȡ�������źŷ����ڴ�
			short *p=pi;    

			fseek(sourcefile,sizeof(struct _TWavHeader),SEEK_SET);   //�ƶ��ļ�ָ��
			//fread(&waveheader,sizeof(struct _TWavHeader),1,sourcefile);
			while(fread(buffer,sizeof(short),FrmLen,sourcefile)==FrmLen)    //ʵ�ʶ�ȡ�������ݡ�һ֡����
			{
				for(int i=0;i<FrmLen;++i)
				{
					p[FrmLen*cishu+i]=buffer[i];                     //��buffer���������ȫ���ŵ�pָ��Ĵ洢�ռ�
				}
				PreEmphasise(p+(FrmLen*cishu),(float)0.97);
				//PreEmphasise(p+(FrmLen*cishu),(float)0.97);
				//PreEmphasise(p+(FrmLen*cishu),(float)0.97);
				cishu++;
			}
			//PreEmphasise(p,(float)0.97);

		////////�˵���///////
			
			short *data,*q;  //�洢���źſ��ǲ���
			short *m_vdata; //�˵���֮�������
			float *energy;   //���ǲ���֮��ÿһ֡���ݵ�ƽ������
			int j,h;          
			int num=0,num1=0;      //���ڶ˵��⣬ͳ����Ч֡����Ч֡
			short kz=0;              //�˵����״̬
			float m_energy=0.0;    //ƽ������
			
			/*for (i=0;i<512*20;i++)//ȡ�û�������50����50+512*3������������ƽ������
			{
				m_energy+=(float)fabs((float)p[i]/120);//ƽ�����Ⱦ���ֵ�ۼӣ�����2000��������Ե���Ӧ���Ǹ���ʶ��Ч�������˵ģ�������Ϊʲô����
			}
			m_energy=(float)(m_energy/(float)50); //ƽ����������ʵ����ƽ�����ȣ�*/


			//m_energy=2333.00;//����һ֡�հ׵������õ���ƽ������������õ��ľ���ֵ
			m_energy=2333.00;

			//h=(int)(jc*FrmLen/2-(FRAMELEN-STEPLEN))/STEPLEN;  //���ǲ���֮���֡��
			h=(int)(jc*FrmLen-(FRAMELEN-STEPLEN))/STEPLEN;  //2015.4.2


			CString ing3;//////�м����//////
			ing3.Format("initial %d",(int)(jc*FrmLen/FRAMELEN));
//			MessageBox(ing3);

			CString ing;//////�м����//////
			ing.Format("h %d",h);
//			MessageBox(ing);

			data=(short*)calloc(h*FRAMELEN,sizeof(short));//�洢���źſ��ǲ���
			energy=(float*)calloc(h,sizeof(float));//Ϊÿ֡ƽ����������洢�ռ䣬һ��h֡
			m_vdata=(short*)calloc(200*FRAMELEN,sizeof(short)); //���ڴ洢�˵���֮���������ÿ֡512

			q=data;//��dataͷָ�븳��q

			for (i=0;i<h;i++)//���h֡��������ÿ֡�Ķ�ʱ����
			{
				energy[i]=0.0;//ÿ֡�Ķ�ʱ����
				for (j=0;j<FRAMELEN;j++)//FRAMELENΪ����֡����������512����ÿ֡��ƽ�����ȣ�������������energy������
				{
					q[j]=(short)p[j];
					q[j]=q[j]*Hamming[j];
					energy[i]=energy[i]+(float)fabs(q[j]);
				}
				
				q=q+FRAMELEN;//�洢����ָ��ƫ������һ֡�Ĳ���������512��
				p=p+STEPLEN;
				//��������������ָ��ƫ��һ��������STEPLEN=160����Ҳ����˵ÿ��ֻ�ƶ�160����ʵ�ʴ洢��512�������Է����洢��һЩ�������ݡ�

			}
			q=data;//�ٴδ�ͷ��ַ��ʼ��������������Ч֡
			
			for (i=0;i<h;i++) //��ȡ��Ч����֡
			{

				switch(kz) //kz�ĳ�ʼֵΪ0����̬������
				{
					case 0:
						{
							if (energy[i]>m_energy) //�����֡ƽ����������m_energy����֡Ϊ��Ч��������
							{
								num++;//������ʼֵΪ0����������Ч֡���м���
								for (j=0;j<FRAMELEN;j++)//�Ѹ�֡���ݸ���m_vdata
								{
									m_vdata[m_framenum*FRAMELEN+j]=q[j];//����200֡��¼��ʱ��Ϊ6.4S
								}
								//ing4.Format("%d",i);
								//MessageBox(ing4);
								m_framenum++;//��ʼֵΪ0����������Ч֡���м��������Ǿ�̬������
								kz=1;//�ı�kz��ִֵ����һ��������Ѱ����Ч����֡��
							}
							break;
						}
						
					case 1:
						{
							if (energy[i]>m_energy) 
							{
								num++;
								for (j=0;j<FRAMELEN;j++)
								{
									m_vdata[m_framenum*FRAMELEN+j]=q[j];
								}
								//ing4.Format("%d",i);
								//MessageBox(ing4);
								m_framenum++;
							}
							else//�������һ֡С��ƽ������������Ч֡���㣨��������Щ�������ݣ�
							{
								num=0;
								m_framenum=0;
								kz=0;
							}
							if (num>6)//���������10֡����ʵ���ǲ�������������ƽ����������е�case 2
							{
								kz=2;
							}
							break;
						}
					case 2://����10��������Ч��������ʱ��0.1sʱ�䣩һ������ʱ��Ϊ0.01s,������ֵ����������Ǵ���ƽ����������С��ƽ������������Ч����
						{
							if (energy[i]>m_energy) 
							{
								num++;
								for (j=0;j<FRAMELEN;j++)
								{
									m_vdata[m_framenum*FRAMELEN+j]=q[j];
								}
								//ing4.Format("%d",i);
								//MessageBox(ing4);
								m_framenum++;
								if (num>200)//������Ч����ʱ�䳬��2S��������Щ�������ݣ�
								{
									m_framenum=0;
									kz=0;
									num=0;
									num1=0;
								}
							}
							else//��������и�����С��ƽ��������ִ��case 3���ò�������Ϊ��Ч��������
							{
								num++;
								num1++;//�ж�����������־��1
								for (j=0;j<FRAMELEN;j++)
								{
									m_vdata[m_framenum*FRAMELEN+j]=q[j];
								}
								//ing4.Format("%d",i);
								//MessageBox(ing4);
								m_framenum++;
								kz=3;
								if (num>198)//֡����������Ч����ʱ�䳬��1.98S��������Щ�������ݣ�
								{
									m_framenum=0;
									kz=0;
									num=0;
									num1=0;
								}
							}
						break;
						}
					case 3:
						{
							if (energy[i]<m_energy)  //�ж���������
							{
								num1++;
								if (num1>10)//������������10֡����ʱȷ��Ϊ��Ч�����յ㣬��ʱ���Խ��в�����ȡ��ƥ���ˣ������������������0.1s
								{
									num1=0;
									kz=4;
								}
							}
							else//�˴��������������㣬����ִ��case 2
							{
								num++;
								num1=0;//���ж�����������־����
								for (j=0;j<FRAMELEN;j++)
								{
									m_vdata[m_framenum*FRAMELEN+j]=q[j];
								}
								//ing4.Format("%d",i);
								//MessageBox(ing4);
								m_framenum++;
								kz=2;
								if (m_framenum>198)
								{
									m_framenum=0;
									kz=0;
									num=0;
									num1=0;
								}
							}
						break;
						}
					default:
						break;
				}
				q=q+FRAMELEN;//һ����������һ֡�������ݣ�������������Ҫƫ��һ��֡��

			}

		//	CString ingh;//////�м����//////
			//ingh.Format("m_framenum %d",m_framenum);
//			MessageBox(ingh);
			/////////�˵������////////


			/*short m_vdatamax=fabs(m_vdata[1]);
			for(i=0;i<m_framenum*FRAMELEN;i++)//�����
			{	
				if(fabs(m_vdata[i])>m_vdatamax)
					m_vdatamax=fabs(m_vdata[i]);
			}

			for(i=0;i<m_framenum*FRAMELEN;i++)
			{	
				outfile1<<10000*m_vdata[i]/m_vdatamax<<' ';//10000Ϊ��һ��
			}*/

			short sum=0;
			for(i=0;i<m_framenum*FRAMELEN;i++)
			{	
				sum+=pow(m_vdata[i],2);
			}
			
			CString ingh;//////�м����//////
			ingh.Format("m_framenum %d",sum);
		//	MessageBox(ingh);

			for(i=0;i<m_framenum*FRAMELEN;i++)
			{	
				outfile1<<short(1000*m_vdata[i]/sqrt(sum))<<' ';
			}



			/*outfile1<<"�����ź���֡���� "<<m_framenum;
			  
			outfile1<<"��Ƶ���ݵĴ�С: "<<waveheader.wSampleLength<<endl; 
			outfile1<<"��־����RIFF��: "<<waveheader.rId<<endl;    
			outfile1<<"���ݴ�С,��������ͷ�Ĵ�С����Ƶ�ļ��Ĵ�С: "<<waveheader.rLen<<endl;    //���ݴ�С,��������ͷ�Ĵ�С����Ƶ�ļ��Ĵ�С
			outfile1<<"��ʽ���ͣ�WAVE��: "<<waveheader.wId<<endl;       //��ʽ���ͣ�"WAVE"��
			outfile1<<"fmt: "<<waveheader.fId<<endl; 
			outfile1<<"Sizeof(WAVEFORMATEX): "<<waveheader.fLen<<endl; 
			outfile1<<"�����ʽ������WAVE_FORMAT_PCM��WAVEFORMAT_ADPCM��: "<<waveheader.wFormatTag<<endl;   
			outfile1<<"��������������Ϊ1��˫����Ϊ2: "<<waveheader.nChannels<<endl;   
			outfile1<<"����Ƶ��: "<<waveheader.nSamplesPerSec<<endl; 
   			outfile1<<"ÿ���������: "<<waveheader.nAvgBytesPerSec<<endl;      
			outfile1<<"�����: "<<waveheader.nBlockAlign<<endl;  
			outfile1<<"WAVE�ļ��Ĳ�����С: "<<waveheader.wBitsPerSample<<endl; 
  			outfile1<<"data: "<<waveheader.dId<<endl;   
			*/
			outfile1.close();
			fclose(sourcefile);

			CString mfccData;
			mfccData.Format("���%d"+ordername+"mfcc",wj);
			ofstream outfileMfcc(mfccData+".txt");  //�洢mfcc����
			ifstream ifile(voicedata+".txt");     //�ӡ����ԡ����������������

			codetrain+=mfccd(outfileMfcc,ifile,outfileCode);      //������ȡ

			free(pi);pi=NULL;
			free(data);data=NULL;
			free(energy);energy=NULL;
			free(m_vdata);m_vdata=NULL;

			ifile.close();
			outfileMfcc.close();
			}

        for(wj=1;wj<=1;wj++)//����ÿ�������е�WENJIAN�������ļ�
		{	
			ifstream infileCB("cb.txt");

			CString nameYasuo;
			nameYasuo.Format("���%d",wj);

			//CString nameMfcc;
			//nameMfcc.Format("mfcc%d",wj);
			ifstream infileYy(nameYasuo+ordername+"mfcc.txt");	

			ofstream outfileYs(nameYasuo+ordername+"ѹ������.txt");
			int VoiceLen=0;
			infileYy>>VoiceLen;//��֡��
			VoiceLen=(int)(VoiceLen*MFCCNUM/VEC_DEM);//��֡Ϊ��������������ת��Ϊ��ά���������������ȣ�VEC_DEM=9ά
			int* Code=(int*)calloc(VoiceLen,sizeof(int));//����֮�������
			double* Dispar=(double*)calloc(CB_SIZE,sizeof(double));
			int CodeBL=0;
			int i,j,vi;
			double CodeBook1[CB_SIZE][VEC_DEM];//����

			/***************������***************/
			for(i=0;i<CB_SIZE;i++)
			{
				for(j=0;j<VEC_DEM;j++)
				{
					infileCB>>CodeBook1[i][j];				
				}
			}
			infileCB.close();

			//��ʾ�����ĳ��ȣ���ά��������
			CString string;
			string.Format("%d",VoiceLen); 
			//MessageBox(string);

			/*********���벻������ά��������������**********/
			double** VoiceData=(double**)calloc(VoiceLen,sizeof(double*));
			for(int yi=0;yi<VoiceLen;yi++)
				 VoiceData[yi]=(double*)calloc(VEC_DEM,sizeof(double));

			/*********�������ļ��ж�����������**********/
			for(i=0;i<VoiceLen;i++)
				for(j=0;j<VEC_DEM;j++)
				{
					infileYy>>VoiceData[i][j];
				}		
			infileYy.close();

			/*********��������**********/
			for(i=0;i<VoiceLen;i++)
			{
				CodeBL=0;
				for(j=0;j<CB_SIZE;j++)
				{
					for (vi=0; vi<VEC_DEM; vi++)
						Dispar[j] += (VoiceData[i][vi] - CodeBook1[j][vi]) * (VoiceData[i][vi] - CodeBook1[j][vi]);
					
					if(j>0)
					{
						if(Dispar[j]<Dispar[CodeBL])
						{
							CodeBL=j;
						}	
					}
				}
				for(j=0;j<CB_SIZE;j++)
					Dispar[j]=0;
				Code[i]=CodeBL;
			}
			//¼���������������
			outfileYs<<VoiceLen<<' ';
			for(i=0;i<VoiceLen;i++)
				outfileYs<<1+Code[i]<<' ';
			outfileYs.close();

			/**********�ͷ��ڴ�***********/
			free(Code);
			Code=NULL;
			for(i=0;i<VoiceLen;i++)
			{
				free(VoiceData[i]);
				VoiceData[i]=NULL;
			}
			free(VoiceData);
			VoiceData=NULL;    
			free(Dispar);
			Dispar=NULL;
		}
	}
	//outfileCode.seekp(0, ios::beg); 
	//outfileCode<<codetrain<<' ';
	outfileCode.close();

	CString ing;
	ing.Format("xulian %d",codetrain);
	MessageBox(ing);

}

void CPjvoice2Dlg::OnButton10() 
{
	// TODO: Add your control notification handler code here
	void PreEmphasise (short *s, float k);
	//CString nameofwj;
	//nameofwj.Format("�հ�"+".wav",1);

	//CString voicedata;
	//voicedata.Format("�հ�"+"��������",1);

	TWavHeader waveheader;//��ȡ.wav�����ļ���ͷ��Ϣ
	int jc=0;                //����ͳ��.wav�ʼ������ĳ���
	//int i;
	int m_framenum=0;         //�˵����������֡����֡��512�ģ�
	short buffer[FrmLen];      //�洢ÿ�δ�.wav�ʼ��ж�ȡ������
	FILE *sourcefile;           //��.wav�ļ�

	ofstream outfile1("a�հ���������.txt");           //���˵��������ݶ��롰����.txt��
	outfile1.precision(4);

	sourcefile=fopen("�հ�.wav","rb");                     //��.wav�ļ�

	fread(&waveheader,sizeof(struct _TWavHeader),1,sourcefile);           //��ȡ����ͷ��Ϣ
	while(fread(buffer,sizeof(short),FrmLen,sourcefile)==FrmLen)           //��ȡ�����źŵĳ���
	{
		jc++;          //ͳ�������źŵĳ���
	}

	int cishu=0;        //����ͳ�ƶ�ȡ�ڼ��Σ������ȵ�ʱ���źţ�
	short *pi=(short *)calloc(jc*FrmLen,sizeof(short));    //Ϊ��ȡ�������źŷ����ڴ�
	short *p=pi;    

	fseek(sourcefile,sizeof(struct _TWavHeader),SEEK_SET);   //�ƶ��ļ�ָ��
	//fread(&waveheader,sizeof(struct _TWavHeader),1,sourcefile);
	long int sum=0;
	int ave=0;
	while(fread(buffer,sizeof(short),FrmLen,sourcefile)==FrmLen)    //ʵ�ʶ�ȡ�������ݡ�һ֡����
	{
		for(int i=0;i<FrmLen;++i)
		{
			p[FrmLen*cishu+i]=buffer[i];                     //��buffer���������ȫ���ŵ�pָ��Ĵ洢�ռ�
		}
		PreEmphasise(p+(FrmLen*cishu),(float)0.97);
		//PreEmphasise(p+(FrmLen*cishu),(float)0.97);
		//PreEmphasise(p+(FrmLen*cishu),(float)0.97);
		
		for(i=0;i<FrmLen;i++)
		{	
			outfile1<<*(p+(FrmLen*cishu)+i)<<' ';
			sum=sum+abs(*(p+(FrmLen*cishu)+i));
		}

		cishu++;
	}
	ave=sum/(cishu);
	//PreEmphasise(p,(float)0.97);
	CString ing3;//////�м����//////
    ing3.Format("%d",(int)(ave));
    MessageBox(ing3);



	free(pi);
	pi=NULL;
	outfile1.close();
	fclose(sourcefile);

}
