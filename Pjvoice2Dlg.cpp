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

//#define RECORD_BUFFER_SIZE 3904 //输入的缓冲区长度//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define RECORD_BUFFER_SIZE 3904 //2015.6.9 输入的缓冲区长度//
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
	//录音初始化
	m_iBufNum = 2;//录音缓存区缓存区个数//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
		MessageBox(_T("打开录音设备出错！"), _T("警告"),MB_OK);//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		return FALSE;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	}
	//在列表框中插入命令

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

BOOL CPjvoice2Dlg::OpenDevice()//设置一些参数//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
{
	m_wfx.wFormatTag = WAVE_FORMAT_PCM;//录音格式//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_wfx.nChannels =1;//声道数//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_wfx.nSamplesPerSec = 32000;   //采样率//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_wfx.wBitsPerSample = 16;//采样位//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_wfx.nAvgBytesPerSec = 32000*2;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_wfx.nBlockAlign = 2;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_wfx.cbSize = 0;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	if(waveInOpen(&m_hWaveIn,WAVE_MAPPER,&m_wfx,(DWORD)this->m_hWnd,NULL,CALLBACK_WINDOW))//打开波形输入设备为录音//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	{
		MessageBeep(MB_ICONEXCLAMATION);//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		AfxMessageBox(_T("Audio can not be open!"));//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		return FALSE;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	}
    return TRUE;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%<<
}

void CPjvoice2Dlg::PrePareRecord()//出问题了
{
	for (int i = 0; i < m_iBufNum; i++)//增加缓存，m_iBufNum==2，所以分配两个缓存区
	{
		m_pWaveHdr[i]->lpData = (char*)m_pBuffer[i];//语音数据缓存区头指针（lpData）
		m_pWaveHdr[i]->dwBufferLength = RECORD_BUFFER_SIZE;//缓存区长度
		m_pWaveHdr[i]->dwBytesRecorded = 0;//缓存区当前已经占用的长度
		m_pWaveHdr[i]->dwFlags = 0;
		m_pWaveHdr[i]->dwLoops = 1;
		m_pWaveHdr[i]->dwUser = 0;
		m_pWaveHdr[i]->lpNext = NULL;
		m_pWaveHdr[i]->reserved = 0;
		waveInPrepareHeader(m_hWaveIn,m_pWaveHdr[i],sizeof(WAVEHDR));//将缓冲区信息和输入设备相关联
		waveInAddBuffer(m_hWaveIn,m_pWaveHdr[i],sizeof(WAVEHDR));//将缓冲区地址和输入设备相关联
	}
}

void CPjvoice2Dlg::StartRecord()
{
     MMRESULT mmResult = waveInStart(m_hWaveIn);//打开波形输入设备
}
void CPjvoice2Dlg::OnMM_WIM_CLOSE(UINT wParam, LONG lParam)//录音关闭时（消息响应函数，当录音关闭时会执行该函数，该函数为空）
{
	//AfxMessageBox("缓存已满");
}

void CPjvoice2Dlg::OnMM_WIM_OPEN(UINT wParam,LONG lParam)//开始录音时（消息响应函数，当录音开始时会执行该函数，该函数为空）
{
    ;
}

void CPjvoice2Dlg::OnMM_WIM_DATA(UINT wParam, LONG lParam)//调用此函数,录音过程处理（消息响应函数，当一个录音缓存区已满时会自动调用该函数）
{
//AfxMessageBox("缓存已满");

	void PreEmphasise (short *s, float k);//预处理声明

	//汉明窗
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
    static short  ss=0;//计算平均能量标志，当ss=2时计算平均能量
	if (((PWAVEHDR) lParam)->dwBytesRecorded < RECORD_BUFFER_SIZE)//判断缓存区是否已满，未满该函数返回（防止未满就执行意外情况）
	{
		TRACE(_T("No useful data\n"));
		return;
	}
       ss++;
	if (m_firstrecord&&ss==2)//当ss==2时算平均能量，只算一次，由于要判断energy[i]<m_energy1所以不会进行参数提取等操作
	{
		//m_energy1=18622.00;//计算一帧空白的语音得到的平均能量，计算得到的经验值
		p=(short*)((PWAVEHDR) lParam)->lpData;//把语音数据缓存区头指针赋给p
		m_energy1=0.0;

		/*//////////)))))))))))))))))))))
	    for (i=50;i<50+512*3;i++)//取该缓存区第50个到50+512*3个语音数据算平均幅度
		{
			//PreEmphasise(q,(float)0.97);
			//m_energy1+=(float)fabs((double)p[i]/2000);//平均幅度绝对值累加，除以2000和下面乘以的数应该是根据识别效果调整了的
			m_energy1+=(float)fabs((double)(p[i]-p[i-1])*0.97/2000);//2015.6.2 平均幅度绝对值累加，除以2000和下面乘以的数应该是根据识别效果调整了的
			//m_energy1+=(float)fabs((double)p[i]);//平均幅度绝对值累加，除以2000和下面乘以的数应该是根据识别效果调整了的
		}
		m_energy1=(float)(1.0*m_energy1/(float)3); //2015.6.10平均能量（其实就是平均幅度）*/
		
		//m_energy1=李飞;
    	//////////)))))))))))))))))))))
			m_energy1=1.1;

		m_firstrecord=FALSE;//把条件置假避免再次计算平均能量
		return;
	}
	//因为语音数据是16位的，而分配的存储空间是char型的所以要除以2，由于每次移动的是步长所以一个缓存区存的帧数要除以steplen（此h=7.46875所以为7帧）
	h=(RECORD_BUFFER_SIZE/2-(FRAMELEN-STEPLEN))/STEPLEN;
	
	//data=(short*)malloc(sizeof(short)*h*FRAMELEN);//为7帧语音数据分配新的存储空间data为头指针一共7*512=3584个空间
	//energy=(float*)malloc(sizeof(float)*h);//为每帧平均能量分配存储空间，一共h帧

	data=(short*)calloc(h*FRAMELEN,sizeof(short));//存储的信号考虑步移
	//data_q=(short*)calloc(h*FRAMELEN,sizeof(short));/////////////////////////////////////////////////
	energy=(float*)calloc(h,sizeof(float));//为每帧平均能量分配存储空间，一共h帧
	q=data;//把data头指针赋给q
	p=(short*)((PWAVEHDR) lParam)->lpData;//把缓存区语音数据赋给p
	for (i=0;i<h;i++)//求该7帧语音数据每帧的短时能量
	{
		energy[i]=0.0;//每帧的短时能量
		//PreEmphasise(p,(float)0.97);
		for (j=0;j<FRAMELEN;j++)//FRAMELEN为语音帧采样点数（512）求每帧的平均幅度（能量）保存在energy数组中
		{
			//short data_q=(short)p[j]/(float)2000;
			q[j]=(short)p[j]/(float)1;//2015.6.9
		}
		PreEmphasise(q,(float)0.97);
		//PreEmphasise(q,(float)0.97);
		//PreEmphasise(q,(float)0.97);
		for (j=0;j<FRAMELEN;j++)//FRAMELEN为语音帧采样点数（512）求每帧的平均幅度（能量）保存在energy数组中
		{
			//short data_q=(short)p[j]/(float)2000;
			q[j]=q[j]*Hamming[j];
			energy[i]=energy[i]+(float)fabs((short)q[j]/(float)2000);//2015.6.9
		}
		q=q+FRAMELEN;//存储语音指针偏移语音一帧的采样点数（512）
		p=p+STEPLEN;
		//缓存区语音数据指针偏移一个步长（STEPLEN=160），也就是说每次只移动160，而实际存储了512个，所以反复存储了一些语音数据。
	} 
	q=data;//再次从头地址开始运算来求它的有效帧


			//CString huhu;
			//huhu.Format("%f",m_energy1);
			//AfxMessageBox(huhu);                   //xjfopajgopajgoiahjgoiahhjkkokopkp


	for (i=0;i<h;i++) //获取有效语音帧
	{
		switch(kl) //k的初始值为0（静态变量）
		{
	        case 0:
				{
					if (energy[i]>m_energy1) //如果该帧平均能量大于m_energy，该帧为有效语音数据
					{
					    numl++;//静态变量初始值为0，对语音有效帧进行计数
						for (j=0;j<FRAMELEN;j++)//把该帧数据赋给m_vdata1
						{
							m_vdata1[m_framenum1*FRAMELEN+j]=q[j];//最大存200帧，录音时间为6.4S
						}
						m_framenum1++;//初始值为0，对语音有效帧进行计数（不是静态变量）
					    kl=1;//改变k的值执行下一步（继续寻找有效语音帧）
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
					else//如果出现一帧小于平均能量，将有效帧置零（即舍弃这些语音数据）
					{
						numl=0;
						m_framenum1=0;
						kl=0;
					}
					if (numl>6)//如果有连续10帧（其实就是步长）语音大于平均能量这进行到case 2
					{
						kl=2;
					}
					break;
				}
			case 2://超过10个步长有效语音数据时（0.1s时间）一个步长时间为0.01s,后面出现的语音不管是大于平均能量还是小于平均能量都是有效语音
				{
					if (energy[i]>m_energy1) 
					{
						numl++;
						for (j=0;j<FRAMELEN;j++)
						{
							m_vdata1[m_framenum1*FRAMELEN+j]=q[j];
						}
						m_framenum1++;
						if (m_framenum1>200)//（即有效语音时间超过2S，舍弃这些语音数据）
						{
							m_framenum1=0;
							kl=0;
							numl=0;
							num1l=0;
						}
					}
					else//如果出现有个步长小于平均能量才执行case 3，该步长还是为有效语音数据
					{
						numl++;
						num1l++;//判断语音结束标志加1
						for (j=0;j<FRAMELEN;j++)
						{
							m_vdata1[m_framenum1*FRAMELEN+j]=q[j];
						}
						m_framenum1++;
						kl=3;
						if (m_framenum1>198)//帧个数（即有效语音时间超过1.98S，舍弃这些语音数据）
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
					if (energy[i]<m_energy1)  //判断语音结束
					{
						num1l++;
						if (num1l>10)//当有连续大于10帧语音时确定为有效语音终点，此时可以进行参数提取或匹配了，即语音间隔可以容忍0.1s
						{
							num1l=0;
							kl=4;
							m_rec=TRUE;
						}
					}
					else//此处不是声音结束点，跳回执行case 2
					{
						numl++;
						num1l=0;//将判断声音结束标志置零
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
		q=q+FRAMELEN;//一个步长存了一帧语音数据，所以语音数据要偏移一个帧长
	}

	//MessageBox("满bu了");
	if (m_rec==TRUE)//录音初始化使它为FALSE,m_rec为（一段语音可以进行MFCC参数提取及DTW匹配的）标志
	{

		kl=0;
		numl=0;
		num1l=0;
		m_framenum1--;	
		if (m_framenum1<20||m_framenum1>300)//即连续有效语音时间在0.35S（35*160/16000）到1.5S时才进行参数提取或匹配
		{
			m_framenum1=0;
			m_rec=FALSE;
			MessageBox("请重说");
		}
		else
		{	
			waveInReset(m_hWaveIn); //停止录音
	        waveInUnprepareHeader(m_hWaveIn,*m_pWaveHdr,sizeof(WAVEHDR));//清除缓存

			/////////////////////////////////////
			CString wo;
	    	wo.Format("%d，FRAMELEN %d",m_framenum1,FRAMELEN);
		    //MessageBox("满了"+wo);

			ofstream outfile1("aaaa.txt"); 


			short m_vdatamax1=fabs(m_vdata1[1]);//2015.11.16
			for(int aaa=0;aaa<m_framenum1*FRAMELEN;aaa++)//找最大
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
				outfile1<<m_vdata1[aaa]<<' ';//10000为归一化
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
			//mfcc=(float*)malloc(sizeof(float)*m_framenum1*MFCCNUM);//为MFCC参数分配存储空间，mfcc为头指针
			//m=mfcc;//把mfcc头指针赋给m

			int mindex=mfccf(mfcc,m_vdata1,m_framenum1*FRAMELEN);	
            //AfxMessageBox("shsihsihi");

			////////////////压缩
            ifstream infileCB("cb.txt");

			ofstream outfileYs("aaaa压缩录音.txt");
			int VoiceLen=0;
			//infileYy>>VoiceLen;//读帧数
			VoiceLen=mindex;
			VoiceLen=(int)(VoiceLen*MFCCNUM/VEC_DEM);//将帧为度量的语音长度转换为以维数度量的语音长度，VEC_DEM=36维
			int* Code=(int*)calloc(VoiceLen,sizeof(int));//编码之后的数据
			double* Dispar=(double*)calloc(CB_SIZE,sizeof(double));
			int CodeBL=0;
			int i,j,vi;
			double CodeBook1[CB_SIZE][VEC_DEM];//码书

			/////////////读码书/////////////
			for(i=0;i<CB_SIZE;i++)
			{
				for(j=0;j<VEC_DEM;j++)
				{
					infileCB>>CodeBook1[i][j];				
				}
			}
			infileCB.close();

			//显示语音的长度（以维数度量）
			CString string;
			string.Format("%d",VoiceLen); 
			//MessageBox(string);

			//////////////申请不定长二维数组存放语音数据/////////////
			double** VoiceData=(double**)calloc(VoiceLen,sizeof(double*));
			for(int yi=0;yi<VoiceLen;yi++)
				 VoiceData[yi]=(double*)calloc(VEC_DEM,sizeof(double));

			///////////////////读入语音数据/////////////////
			for(i=0;i<VoiceLen;i++)
				for(j=0;j<VEC_DEM;j++)
				{
					//infileYy>>VoiceData[i][j];
					VoiceData[i][j]=(double)mfcc[i*VEC_DEM+j];
					//VoiceData[i][j]=m_vdata1[i*VEC_DEM+j];
				
				}		
			//infileYy.close();

			////////////////////语音编码////////////////
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
			//录入编码后的语音数据
			outfileYs<<VoiceLen<<' ';
			for(i=0;i<VoiceLen;i++)
				outfileYs<<1+Code[i]<<' ';
			outfileYs.close();

			/////////////////释放内存///////////////////

			for(i=0;i<VoiceLen;i++)
			{
				free(VoiceData[i]);
				VoiceData[i]=NULL;
			}
			free(VoiceData);
			VoiceData=NULL;    
			free(Dispar);
			Dispar=NULL;
			///////////////压缩

			/////////////////////识别
			HMM hmm[11];
			//读HMM
			FILE *fpHMM1;
			fpHMM1=fopen("后退归一_HMM.txt", "r");
			ReadHMM(fpHMM1,&hmm[1]);//2015.11.2
			fclose(fpHMM1);
			
			FILE *fpHMM2;
			fpHMM2=fopen("后仰归一_HMM.txt", "r");
			ReadHMM(fpHMM2,&hmm[5]);//2015.11.2
			fclose(fpHMM2);
			
			FILE *fpHMM3;
			fpHMM3=fopen("平躺归一_HMM.txt", "r");
			ReadHMM(fpHMM3,&hmm[6]);//2015.11.2
			fclose(fpHMM3);
			
			FILE *fpHMM4;
			fpHMM4=fopen("前进归一_HMM.txt", "r");
			ReadHMM(fpHMM4,&hmm[0]);//2015.11.2
			fclose(fpHMM4);

			FILE *fpHMM5;
			fpHMM5=fopen("前倾归一_HMM.txt", "r");
			ReadHMM(fpHMM5,&hmm[7]);//2015.11.2
			fclose(fpHMM5);

			FILE *fpHMM6;
			fpHMM6=fopen("上升归一_HMM.txt", "r");
			ReadHMM(fpHMM6,&hmm[8]);//2015.11.2
			fclose(fpHMM6);
			
			FILE *fpHMM7;
			fpHMM7=fopen("停止归一_HMM.txt", "r");
			ReadHMM(fpHMM7,&hmm[4]);//2015.11.2
			fclose(fpHMM7);
			
			FILE *fpHMM8;
			fpHMM8=fopen("下降归一_HMM.txt", "r");
			ReadHMM(fpHMM8,&hmm[9]);//2015.11.2
			fclose(fpHMM8);
			
			FILE *fpHMM9;
			fpHMM9=fopen("右转归一_HMM.txt", "r");
			ReadHMM(fpHMM9,&hmm[3]);//2015.11.2
			fclose(fpHMM9);

			FILE *fpHMM10;
			fpHMM10=fopen("站立归一_HMM.txt", "r");
			ReadHMM(fpHMM10,&hmm[10]);//2015.11.2
			fclose(fpHMM10);

			FILE *fpHMM11;
			fpHMM11=fopen("左转归一_HMM.txt", "r");
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

			//申请内存
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
			case 0:	AfxMessageBox(pMaxStr+"  前进");//2015.11.2
				break;
			case 1:	AfxMessageBox(pMaxStr+"  后退");//2015.11.2
				break;
			case 2:	AfxMessageBox(pMaxStr+"  左转");//2015.11.2
				break;
			case 3:	AfxMessageBox(pMaxStr+"  右转");//2015.11.2
				break;
			case 4:	AfxMessageBox(pMaxStr+"  停止");//2015.11.2
				break;
			case 5:	AfxMessageBox(pMaxStr+"  后仰");//2015.11.2
				break;
			case 6:	AfxMessageBox(pMaxStr+"  平躺");//2015.11.2
				break;
			case 7:	AfxMessageBox(pMaxStr+"  前倾");//2015.11.2
				break;
			case 8:	AfxMessageBox(pMaxStr+"  上升");//2015.11.2
				break;
			case 9:	AfxMessageBox(pMaxStr+"  下降");//2015.11.2
				break;
			case 10: AfxMessageBox(pMaxStr+"  站立");//2015.11.2
				break;
			default: AfxMessageBox(pMaxStr+"  识别错误");
				break;
			}

			//释放内存
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
			/////////////////////识别
			free(Code);
			Code=NULL;
			
			free(mfcc);//释放内存
			mfcc=NULL;
			PrePareRecord();//准备录音
	        StartRecord();//开始录音
		}
	}
	
	if (m_rec==FALSE)
	{
		waveInAddBuffer (m_hWaveIn, (PWAVEHDR) lParam, sizeof (WAVEHDR));//给输入设备增加一个缓存 
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

	int codetrain=0;//用于码书训练的
	ofstream  outfileCode("码书训练.txt");
	for(int order=1;order<=ORDERNUM;order++)//依次处理11个命令的数据
	{
		CString ordername;
		switch(order)
        {
		case 1:
			ordername="后退";break;
		case 2:
			ordername="后仰";break;
		case 3:
			ordername="平躺";break;  
		case 4:
			ordername="前进";break;
		case 5:
			ordername="前倾";break;
		case 6:
			ordername="上升";break;
		case 7:
			ordername="停止";break;
		case 8:
			ordername="下降";break;
		case 9:
			ordername="右转";break;
		case 10:
			ordername="站立";break;
		case 11:
			ordername="左转";break;
        default:
            MessageBox("没有此命令");break;
        }

		for(int wj=1;wj<=WENJIAN;wj++)//处理每个命令中的WENJIAN个语音文件
		{  
			CString nameofwj;
			nameofwj.Format(ordername+"%d",wj);//读.wav文件

			CString voicedata;
			voicedata.Format(ordername+"语音数据%d",wj);//初始语音

			TWavHeader waveheader;//读取.wav语音文件的头信息
			int jc=0;                //用于统计.wav问价语音的长度
			int i;
			int m_framenum=0;         //端点检测后语音的帧数（帧长512的）
			short buffer[FrmLen];      //存储每次从.wav问价中读取的数据
			FILE *sourcefile;           //打开.wav文件

			ofstream outfile1(voicedata+".txt");           //将端点检测后的数据读入“试试.txt”
			outfile1.precision(4);

			sourcefile=fopen(nameofwj+".wav","rb");                     //打开.wav文件

			fread(&waveheader,sizeof(struct _TWavHeader),1,sourcefile);           //读取语音头信息
			while(fread(buffer,sizeof(short),FrmLen,sourcefile)==FrmLen)           //读取语音信号的长度
			{
				jc++;          //统计语音信号的长度
			}

			int cishu=0;        //用于统计读取第几次，（长度的时域信号）
			short *pi=(short *)calloc(jc*FrmLen,sizeof(short));    //为读取的语音信号分配内存
			short *p=pi;    

			fseek(sourcefile,sizeof(struct _TWavHeader),SEEK_SET);   //移动文件指针
			//fread(&waveheader,sizeof(struct _TWavHeader),1,sourcefile);
			while(fread(buffer,sizeof(short),FrmLen,sourcefile)==FrmLen)    //实际读取语音数据。一帧（）
			{
				for(int i=0;i<FrmLen;++i)
				{
					p[FrmLen*cishu+i]=buffer[i];                     //将buffer里面的数据全部放到p指向的存储空间
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


		////////端点检测///////
			
			short *data,*q;  //存储的信号考虑步移
			short *m_vdata; //端点检测之后的数据
			float *energy;   //考虑步移之后每一帧数据的平均能量
			int j,h;          
			int num=0,num1=0;      //用于端点检测，统计有效帧和无效帧
			short kz=0;              //端点检测的状态
			float m_energy=0.0;    //平均能量
			
			/*for (i=0;i<512*20;i++)//取该缓存区第50个到50+512*3个语音数据算平均幅度
			{
				m_energy+=(float)fabs((float)p[i]/120);//平均幅度绝对值累加，除以2000和下面乘以的数应该是根据识别效果调整了的，？？？为什么调整
			}
			m_energy=(float)(m_energy/(float)50); //平均能量（其实就是平均幅度）*/


			//m_energy=2333.00;//计算一帧空白的语音得到的平均能量，计算得到的经验值
			m_energy=2333.00;

			//h=(int)(jc*FrmLen/2-(FRAMELEN-STEPLEN))/STEPLEN;  //考虑步移之后的帧数，此处有问题（不应该除以2）2015.4.2
			h=(int)(jc*FrmLen-(FRAMELEN-STEPLEN))/STEPLEN;  //2015.4.2

			CString ing3;//////中间调试//////
			ing3.Format("initial %d",(int)(jc*FrmLen/FRAMELEN));
//			MessageBox(ing3);

			CString ing;//////中间调试//////
			ing.Format("h %d",h);
//			MessageBox(ing);

			data=(short*)calloc(h*FRAMELEN,sizeof(short));//存储的信号考虑步移
			energy=(float*)calloc(h,sizeof(float));//为每帧平均能量分配存储空间，一共h帧
			m_vdata=(short*)calloc(200*FRAMELEN,sizeof(short)); //用于存储端点检测之后的能量，每帧512

			q=data;//把data头指针赋给q

			for (i=0;i<h;i++)//分帧，求该h帧语音数据每帧的短时能量
			{
				energy[i]=0.0;//每帧的短时能量
				//HammingWindow(p);
				for (j=0;j<FRAMELEN;j++)//FRAMELEN为语音帧采样点数（512）求每帧的平均幅度（能量）保存在energy数组中
				{
					q[j]=(short)p[j];
					q[j]=q[j]*Hamming[j];
					energy[i]=energy[i]+(float)fabs(q[j]);
				}
				q=q+FRAMELEN;//存储语音指针偏移语音一帧的采样点数（512）
				p=p+STEPLEN;
				//缓存区语音数据指针偏移一个步长（STEPLEN=160），也就是说每次只移动160，而实际存储了512个，所以反复存储了一些语音数据。

			}
			q=data;//再次从头地址开始运算来求它的有效帧
			
			for (i=0;i<h;i++) //获取有效语音帧
			{

				switch(kz) //kz的初始值为0（静态变量）
				{
					case 0:
						{
							if (energy[i]>m_energy) //如果该帧平均能量大于m_energy，该帧为有效语音数据
							{
								num++;//变量初始值为0，对语音有效帧进行计数
								for (j=0;j<FRAMELEN;j++)//把该帧数据赋给m_vdata
								{
									m_vdata[m_framenum*FRAMELEN+j]=q[j];//最大存200帧，录音时间为6.4S
								}
								//ing4.Format("%d",i);
								//MessageBox(ing4);
								m_framenum++;//初始值为0，对语音有效帧进行计数（不是静态变量）
								kz=1;//改变kz的值执行下一步（继续寻找有效语音帧）
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
							else//如果出现一帧小于平均能量，将有效帧置零（即舍弃这些语音数据）
							{
								num=0;
								m_framenum=0;
								kz=0;
							}
							if (num>10)//如果有连续10帧（其实就是步长）语音大于平均能量这进行到case 2
							{
								kz=2;
							}
							break;
						}
					case 2://超过10个步长有效语音数据时（0.1s时间）一个步长时间为0.01s,后面出现的语音不管是大于平均能量还是小于平均能量都是有效语音
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
								if (num>200)//（即有效语音时间超过2S，舍弃这些语音数据）
								{
									m_framenum=0;
									kz=0;
									num=0;
									num1=0;
								}
							}
							else//如果出现有个步长小于平均能量才执行case 3，该步长还是为有效语音数据
							{
								num++;
								num1++;//判断语音结束标志加1
								for (j=0;j<FRAMELEN;j++)
								{
									m_vdata[m_framenum*FRAMELEN+j]=q[j];
								}
								//ing4.Format("%d",i);
								//MessageBox(ing4);
								m_framenum++;
								kz=3;
								if (num>198)//帧个数（即有效语音时间超过1.98S，舍弃这些语音数据）
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
							if (energy[i]<m_energy)  //判断语音结束
							{
								num1++;
								if (num1>10)//当有连续大于10帧语音时确定为有效语音终点，此时可以进行参数提取或匹配了，即语音间隔可以容忍0.1s
								{
									num1=0;
									kz=4;
								}
							}
							else//此处不是声音结束点，跳回执行case 2
							{
								num++;
								num1=0;//将判断声音结束标志置零
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
				q=q+FRAMELEN;//一个步长存了一帧语音数据，所以语音数据要偏移一个帧长

			}

			CString ingh;//////中间调试//////
			ingh.Format("m_framenum %d",m_framenum);
//			MessageBox(ingh);
			/////////端点检测结束////////

			/*short m_vdatamax=fabs(m_vdata[1]);
			for(i=0;i<m_framenum*FRAMELEN;i++)//找最大
			{	
				if(fabs(m_vdata[i])>m_vdatamax)
					m_vdatamax=fabs(m_vdata[i]);
			}

			for(i=0;i<m_framenum*FRAMELEN;i++)
			{	
				outfile1<<10000*m_vdata[i]/m_vdatamax<<' ';//10000为归一化
			}*/

			short sum=0;
			for(i=0;i<m_framenum*FRAMELEN;i++)//找最大
			{	
				sum+=m_vdata[i]*m_vdata[i];
			}

			for(i=0;i<m_framenum*FRAMELEN;i++)
			{	
				outfile1<<short(1000*m_vdata[i]/sqrt(sum))<<' ';//10000为归一化
			}


			/*outfile1<<"语音信号总帧数： "<<m_framenum;
			  
			outfile1<<"音频数据的大小: "<<waveheader.wSampleLength<<endl; 
			outfile1<<"标志符（RIFF）: "<<waveheader.rId<<endl;    
			outfile1<<"数据大小,包括数据头的大小和音频文件的大小: "<<waveheader.rLen<<endl;    //数据大小,包括数据头的大小和音频文件的大小
			outfile1<<"格式类型（WAVE）: "<<waveheader.wId<<endl;       //格式类型（"WAVE"）
			outfile1<<"fmt: "<<waveheader.fId<<endl; 
			outfile1<<"Sizeof(WAVEFORMATEX): "<<waveheader.fLen<<endl; 
			outfile1<<"编码格式，包括WAVE_FORMAT_PCM，WAVEFORMAT_ADPCM等: "<<waveheader.wFormatTag<<endl;   
			outfile1<<"声道数，单声道为1，双声道为2: "<<waveheader.nChannels<<endl;   
			outfile1<<"采样频率: "<<waveheader.nSamplesPerSec<<endl; 
   			outfile1<<"每秒的数据量: "<<waveheader.nAvgBytesPerSec<<endl;      
			outfile1<<"块对齐: "<<waveheader.nBlockAlign<<endl;  
			outfile1<<"WAVE文件的采样大小: "<<waveheader.wBitsPerSample<<endl; 
  			outfile1<<"data: "<<waveheader.dId<<endl;   
			*/
			outfile1.close();
			fclose(sourcefile);

			CString mfccData;
			mfccData.Format(ordername+"mfcc%d",wj);
			ofstream outfileMfcc(mfccData+".txt");  //存储mfcc数据
			ifstream ifile(voicedata+".txt");     //读入语音数据

			codetrain+=mfccd(outfileMfcc,ifile,outfileCode);      //特征提取

			free(pi);pi=NULL;
			free(data);data=NULL;
			free(energy);energy=NULL;
			free(m_vdata);m_vdata=NULL;

			ifile.close();
			outfileMfcc.close();
			}
	}
	outfileCode.seekp(0, ios::beg); //数据用于训练码书
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

	//读HMM
	FILE *fpHMM1;
	fpHMM1=fopen("后退归一_HMM.txt", "r");
	ReadHMM(fpHMM1,&hmm[0]);
	fclose(fpHMM1);
	
	FILE *fpHMM2;
	fpHMM2=fopen("后仰归一_HMM.txt", "r");
	ReadHMM(fpHMM2,&hmm[1]);
	fclose(fpHMM2);
	
	FILE *fpHMM3;
	fpHMM3=fopen("平躺归一_HMM.txt", "r");
	ReadHMM(fpHMM3,&hmm[2]);
	fclose(fpHMM3);
	
	FILE *fpHMM4;
	fpHMM4=fopen("前进归一_HMM.txt", "r");
	ReadHMM(fpHMM4,&hmm[3]);
	fclose(fpHMM4);

	FILE *fpHMM5;
	fpHMM5=fopen("前倾归一_HMM.txt", "r");
	ReadHMM(fpHMM5,&hmm[4]);
	fclose(fpHMM5);

	FILE *fpHMM6;
	fpHMM6=fopen("上升归一_HMM.txt", "r");
	ReadHMM(fpHMM6,&hmm[5]);
	fclose(fpHMM6);
	
	FILE *fpHMM7;
	fpHMM7=fopen("停止归一_HMM.txt", "r");
	ReadHMM(fpHMM7,&hmm[6]);
	fclose(fpHMM7);
	
	FILE *fpHMM8;
	fpHMM8=fopen("下降归一_HMM.txt", "r");
	ReadHMM(fpHMM8,&hmm[7]);
	fclose(fpHMM8);
	
	FILE *fpHMM9;
	fpHMM9=fopen("右转归一_HMM.txt", "r");
	ReadHMM(fpHMM9,&hmm[8]);
	fclose(fpHMM9);

	FILE *fpHMM10;
	fpHMM10=fopen("站立归一_HMM.txt", "r");
	ReadHMM(fpHMM10,&hmm[9]);
	fclose(fpHMM10);

	FILE *fpHMM11;
	fpHMM11=fopen("左转归一_HMM.txt", "r");
	ReadHMM(fpHMM11,&hmm[10]);
	fclose(fpHMM11);
	
	//读语音
	FILE *fVoice;	
	//fVoice=fopen("李飞1前进压缩语音.txt","r");		
	//fVoice=fopen("李飞1后退压缩语音.txt","r");
	//fVoice=fopen("李飞1后仰压缩语音.txt","r");	
	//fVoice=fopen("李飞1平躺压缩语音.txt","r");	
	//fVoice=fopen("李飞1前倾压缩语音.txt","r");	
	fVoice=fopen("李飞1上升压缩语音.txt","r");	
	//fVoice=fopen("李飞1停止压缩语音.txt","r");	
	//fVoice=fopen("李飞1下降压缩语音.txt","r");
	//fVoice=fopen("李飞1右转压缩语音.txt","r");	
	//fVoice=fopen("李飞1站立压缩语音.txt","r");	
	//fVoice=fopen("李飞1左转压缩语音.txt","r");	


	//"后退";"后仰";"平躺";"前进";"前倾";"上升";"停止";"下降";"右转""站立";"左转"
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

	//申请内存
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
	case 0:	AfxMessageBox("后退");
		break;
	case 1:	AfxMessageBox("后仰");
		break;
	case 2:	AfxMessageBox("平躺");
		break;
	case 3:	AfxMessageBox("前进");
		break;
	case 4:	AfxMessageBox("前倾");
		break;
	case 5:	AfxMessageBox("上升");
		break;
	case 6:	AfxMessageBox("停止");
		break;
	case 7:	AfxMessageBox("下降");
		break;
	case 8:	AfxMessageBox("右转");
		break;
	case 9:	AfxMessageBox("站立");
		break;
	case 10: AfxMessageBox("左转");
		break;
	default: AfxMessageBox("识别错误");
		break;
	}


	//释放内存
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
	for(int order=1;order<=ORDERNUM;order++)//依次处理11个命令的数据
	{
		CString ordername;
		switch(order)
        {
		case 1:
			ordername="后退";break;
		case 2:
			ordername="后仰";break;
		case 3:
			ordername="平躺";break;  
		case 4:
			ordername="前进";break;
		case 5:
			ordername="前倾";break;
		case 6:
			ordername="上升";break;
		case 7:
			ordername="停止";break;
		case 8:
			ordername="下降";break;
		case 9:
			ordername="右转";break;
		case 10:
			ordername="站立";break;
		case 11:
			ordername="左转";break;
        default:
            MessageBox("没有此命令");break;
        }

		for(int wj=1;wj<=WENJIAN;wj++)//处理每个命令中的WENJIAN个语音文件
		{	
			ifstream infileCB("cb.txt");

			CString nameMfcc;
			nameMfcc.Format("mfcc%d",wj);
			ifstream infileYy(ordername+nameMfcc+".txt");	

			CString nameYasuo;
			nameYasuo.Format("压缩语音%d",wj);

			ofstream outfileYs(ordername+nameYasuo+".txt");
			int VoiceLen=0;
			infileYy>>VoiceLen;//读帧数
			VoiceLen=(int)(VoiceLen*MFCCNUM/VEC_DEM);//将帧为度量的语音长度转换为以维数度量的语音长度，VEC_DEM=36维
			int* Code=(int*)calloc(VoiceLen,sizeof(int));//编码之后的数据
			double* Dispar=(double*)calloc(CB_SIZE,sizeof(double));
			int CodeBL=0;
			int i,j,vi;
			double CodeBook1[CB_SIZE][VEC_DEM];//码书

			/***************读码书***************/
			for(i=0;i<CB_SIZE;i++)
			{
				for(j=0;j<VEC_DEM;j++)
				{
					infileCB>>CodeBook1[i][j];				
				}
			}
			infileCB.close();

			//显示语音的长度（以维数度量）
			CString string;
			string.Format("%d",VoiceLen); 
			//MessageBox(string);

			/*********申请不定长二维数组存放语音数据**********/
			double** VoiceData=(double**)calloc(VoiceLen,sizeof(double*));
			for(int yi=0;yi<VoiceLen;yi++)
				 VoiceData[yi]=(double*)calloc(VEC_DEM,sizeof(double));

			/*********从语音文件中读入语音数据**********/
			for(i=0;i<VoiceLen;i++)
				for(j=0;j<VEC_DEM;j++)
				{
					infileYy>>VoiceData[i][j];
				}		
			infileYy.close();

			/*********语音编码**********/
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
			//录入编码后的语音数据
			outfileYs<<VoiceLen<<' ';
			for(i=0;i<VoiceLen;i++)
				outfileYs<<1+Code[i]<<' ';
			outfileYs.close();

			/**********释放内存***********/
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
    //读HMM
	for(int order=1;order<=ORDERNUM;order++)//依次处理11个命令的数据
	{

		FILE *fp;
		fp=fopen("初始HMM.txt", "r");
		HMM hmm;
		ReadHMM(fp,&hmm);
		fclose(fp);

		CString ordername;
		switch(order)
        {
		case 1:
			ordername="后退";break;
		case 2:
			ordername="后仰";break;
		case 3:
			ordername="平躺";break;  
		case 4:
			ordername="前进";break;
		case 5:
			ordername="前倾";break;
		case 6:
			ordername="上升";break;
		case 7:
			ordername="停止";break;
		case 8:
			ordername="下降";break;
		case 9:
			ordername="右转";break;
		case 10:
			ordername="站立";break;
		case 11:
			ordername="左转";break;
        default:
            MessageBox("没有此命令");break;
        }
		
		int *T=(int *)calloc(WENJIAN+1,sizeof(int));
		int i=0;
		int **O=(int **)calloc(WENJIAN+1,sizeof(int*));
		//读语音

        for(int wj=1;wj<=WENJIAN;wj++)//处理每个命令中的WENJIAN个语音文件
		{
			FILE *fVoice;	
			CString xuhao;
			xuhao.Format("压缩语音%d",wj);
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
		//申请内存
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

		//写入HMM
		FILE *fWriteHMM;
		fWriteHMM=fopen(ordername+"_HMM.txt", "w");
		PrintHMM(fWriteHMM,&hmm);
		fclose(fWriteHMM);

		//释放内存
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

	//读HMM
	FILE *fpHMM1;
	fpHMM1=fopen("后退_HMM.txt", "r");
	ReadHMM(fpHMM1,&hmm[0]);
	fclose(fpHMM1);
	
	FILE *fpHMM2;
	fpHMM2=fopen("后仰_HMM.txt", "r");
	ReadHMM(fpHMM2,&hmm[1]);
	fclose(fpHMM2);
	
	FILE *fpHMM3;
	fpHMM3=fopen("平躺_HMM.txt", "r");
	ReadHMM(fpHMM3,&hmm[2]);
	fclose(fpHMM3);
	
	FILE *fpHMM4;
	fpHMM4=fopen("前进_HMM.txt", "r");
	ReadHMM(fpHMM4,&hmm[3]);
	fclose(fpHMM4);

	FILE *fpHMM5;
	fpHMM5=fopen("前倾_HMM.txt", "r");
	ReadHMM(fpHMM5,&hmm[4]);
	fclose(fpHMM5);

	FILE *fpHMM6;
	fpHMM6=fopen("上升_HMM.txt", "r");
	ReadHMM(fpHMM6,&hmm[5]);
	fclose(fpHMM6);
	
	FILE *fpHMM7;
	fpHMM7=fopen("停止_HMM.txt", "r");
	ReadHMM(fpHMM7,&hmm[6]);
	fclose(fpHMM7);
	
	FILE *fpHMM8;
	fpHMM8=fopen("下降_HMM.txt", "r");
	ReadHMM(fpHMM8,&hmm[7]);
	fclose(fpHMM8);
	
	FILE *fpHMM9;
	fpHMM9=fopen("右转_HMM.txt", "r");
	ReadHMM(fpHMM9,&hmm[8]);
	fclose(fpHMM9);

	FILE *fpHMM10;
	fpHMM10=fopen("站立_HMM.txt", "r");
	ReadHMM(fpHMM10,&hmm[9]);
	fclose(fpHMM10);

	FILE *fpHMM11;
	fpHMM11=fopen("左转_HMM.txt", "r");
	ReadHMM(fpHMM11,&hmm[10]);
	fclose(fpHMM11);
	
	for(int i=0; i<11; i++)
	{ 
		//A的归一化
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

		//B的归一化
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

		//pi的归一化		
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
//"后退";"后仰";"平躺";"前进";"前倾";"上升";"停止";"下降";"右转";"站立";"左转"

	FILE *fWriteHMM1;
	fWriteHMM1=fopen("后退归一_HMM.txt", "w");
	PrintHMM(fWriteHMM1,&hmm[0]);
	fclose(fWriteHMM1);

	FILE *fWriteHMM2;
	fWriteHMM2=fopen("后仰归一_HMM.txt", "w");
	PrintHMM(fWriteHMM2,&hmm[1]);
	fclose(fWriteHMM2);

	FILE *fWriteHMM3;
	fWriteHMM3=fopen("平躺归一_HMM.txt", "w");
	PrintHMM(fWriteHMM3,&hmm[2]);
	fclose(fWriteHMM3);

	FILE *fWriteHMM4;
	fWriteHMM4=fopen("前进归一_HMM.txt", "w");
	PrintHMM(fWriteHMM4,&hmm[3]);
	fclose(fWriteHMM4);

	FILE *fWriteHMM5;
	fWriteHMM5=fopen("前倾归一_HMM.txt", "w");
	PrintHMM(fWriteHMM5,&hmm[4]);
	fclose(fWriteHMM5);

	FILE *fWriteHMM6;
	fWriteHMM6=fopen("上升归一_HMM.txt", "w");
	PrintHMM(fWriteHMM6,&hmm[5]);
	fclose(fWriteHMM6);

	FILE *fWriteHMM7;
	fWriteHMM7=fopen("停止归一_HMM.txt", "w");
	PrintHMM(fWriteHMM7,&hmm[6]);
	fclose(fWriteHMM7);

	FILE *fWriteHMM8;
	fWriteHMM8=fopen("下降归一_HMM.txt", "w");
	PrintHMM(fWriteHMM8,&hmm[7]);
	fclose(fWriteHMM8);

	FILE *fWriteHMM9;
	fWriteHMM9=fopen("右转归一_HMM.txt", "w");
	PrintHMM(fWriteHMM9,&hmm[8]);
	fclose(fWriteHMM9);

	FILE *fWriteHMM10;
	fWriteHMM10=fopen("站立归一_HMM.txt", "w");
	PrintHMM(fWriteHMM10,&hmm[9]);
	fclose(fWriteHMM10);

	FILE *fWriteHMM11;
	fWriteHMM11=fopen("左转归一_HMM.txt", "w");
	PrintHMM(fWriteHMM11,&hmm[10]);
	fclose(fWriteHMM11);

	AfxMessageBox("ok");
}

void CPjvoice2Dlg::OnButton8() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CString str;//定义字符串
	GetDlgItem(IDC_BUTTON8)->GetWindowText(str);//获取按钮对话框上面的字符串
	if (str=="录音识别")//对按钮话框上面显示的是record
	{
		//GetDlgItem(IDC_BUTTON8)->SetWindowText("stop");//record->stop
		PrePareRecord();//准备识别
	    StartRecord();//开始识别
		GetDlgItem(IDC_BUTTON8)->SetWindowText("正在识别");
	}
	else//按钮对话框也就是变成stop
	{
		waveInReset(m_hWaveIn);//停止录音
	    waveInUnprepareHeader(m_hWaveIn,*m_pWaveHdr,sizeof(WAVEHDR));//清除缓存
		GetDlgItem(IDC_BUTTON8)->SetWindowText("录音识别");//stop->record对话框由stop变成record
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
	ofstream  outfileCode("调试语音数据没用到.txt");
	for(int order=1;order<=ORDERNUM;order++)//依次处理11个命令的数据
	{
		CString ordername;
		switch(order)
        {
		case 1:
			ordername="后退";break;
		case 2:
			ordername="后仰";break;
		case 3:
			ordername="平躺";break;  
		case 4:
			ordername="前进";break;
		case 5:
			ordername="前倾";break;
		case 6:
			ordername="上升";break;
		case 7:
			ordername="停止";break;
		case 8:
			ordername="下降";break;
		case 9:
			ordername="右转";break;
		case 10:
			ordername="站立";break;
		case 11:
			ordername="左转";break;
        default:
            MessageBox("没有此命令");break;
        }

		for(int wj=1;wj<=1;wj++)//处理每个命令中的WENJIAN个语音文件
		{  
			CString nameofwj;
			//nameofwj.Format("李飞%d"+ordername+"_32k16b.wav",wj);
			nameofwj.Format("李飞"+ordername+".wav",wj);

			CString voicedata;
			voicedata.Format("李飞%d"+ordername+"语音数据",wj);

			TWavHeader waveheader;//读取.wav语音文件的头信息
			int jc=0;                //用于统计.wav问价语音的长度
			int i;
			int m_framenum=0;         //端点检测后语音的帧数（帧长512的）
			short buffer[FrmLen];      //存储每次从.wav问价中读取的数据
			FILE *sourcefile;           //打开.wav文件

			ofstream outfile1(voicedata+".txt");           //将端点检测后的数据读入“试试.txt”
			outfile1.precision(4);

			sourcefile=fopen(nameofwj,"rb");                     //打开.wav文件

			fread(&waveheader,sizeof(struct _TWavHeader),1,sourcefile);           //读取语音头信息
			while(fread(buffer,sizeof(short),FrmLen,sourcefile)==FrmLen)           //读取语音信号的长度
			{
				jc++;          //统计语音信号的长度
			}

			int cishu=0;        //用于统计读取第几次，（长度的时域信号）
			short *pi=(short *)calloc(jc*FrmLen,sizeof(short));    //为读取的语音信号分配内存
			short *p=pi;    

			fseek(sourcefile,sizeof(struct _TWavHeader),SEEK_SET);   //移动文件指针
			//fread(&waveheader,sizeof(struct _TWavHeader),1,sourcefile);
			while(fread(buffer,sizeof(short),FrmLen,sourcefile)==FrmLen)    //实际读取语音数据。一帧（）
			{
				for(int i=0;i<FrmLen;++i)
				{
					p[FrmLen*cishu+i]=buffer[i];                     //将buffer里面的数据全部放到p指向的存储空间
				}
				PreEmphasise(p+(FrmLen*cishu),(float)0.97);
				//PreEmphasise(p+(FrmLen*cishu),(float)0.97);
				//PreEmphasise(p+(FrmLen*cishu),(float)0.97);
				cishu++;
			}
			//PreEmphasise(p,(float)0.97);

		////////端点检测///////
			
			short *data,*q;  //存储的信号考虑步移
			short *m_vdata; //端点检测之后的数据
			float *energy;   //考虑步移之后每一帧数据的平均能量
			int j,h;          
			int num=0,num1=0;      //用于端点检测，统计有效帧和无效帧
			short kz=0;              //端点检测的状态
			float m_energy=0.0;    //平均能量
			
			/*for (i=0;i<512*20;i++)//取该缓存区第50个到50+512*3个语音数据算平均幅度
			{
				m_energy+=(float)fabs((float)p[i]/120);//平均幅度绝对值累加，除以2000和下面乘以的数应该是根据识别效果调整了的，？？？为什么调整
			}
			m_energy=(float)(m_energy/(float)50); //平均能量（其实就是平均幅度）*/


			//m_energy=2333.00;//计算一帧空白的语音得到的平均能量，计算得到的经验值
			m_energy=2333.00;

			//h=(int)(jc*FrmLen/2-(FRAMELEN-STEPLEN))/STEPLEN;  //考虑步移之后的帧数
			h=(int)(jc*FrmLen-(FRAMELEN-STEPLEN))/STEPLEN;  //2015.4.2


			CString ing3;//////中间调试//////
			ing3.Format("initial %d",(int)(jc*FrmLen/FRAMELEN));
//			MessageBox(ing3);

			CString ing;//////中间调试//////
			ing.Format("h %d",h);
//			MessageBox(ing);

			data=(short*)calloc(h*FRAMELEN,sizeof(short));//存储的信号考虑步移
			energy=(float*)calloc(h,sizeof(float));//为每帧平均能量分配存储空间，一共h帧
			m_vdata=(short*)calloc(200*FRAMELEN,sizeof(short)); //用于存储端点检测之后的能量，每帧512

			q=data;//把data头指针赋给q

			for (i=0;i<h;i++)//求该h帧语音数据每帧的短时能量
			{
				energy[i]=0.0;//每帧的短时能量
				for (j=0;j<FRAMELEN;j++)//FRAMELEN为语音帧采样点数（512）求每帧的平均幅度（能量）保存在energy数组中
				{
					q[j]=(short)p[j];
					q[j]=q[j]*Hamming[j];
					energy[i]=energy[i]+(float)fabs(q[j]);
				}
				
				q=q+FRAMELEN;//存储语音指针偏移语音一帧的采样点数（512）
				p=p+STEPLEN;
				//缓存区语音数据指针偏移一个步长（STEPLEN=160），也就是说每次只移动160，而实际存储了512个，所以反复存储了一些语音数据。

			}
			q=data;//再次从头地址开始运算来求它的有效帧
			
			for (i=0;i<h;i++) //获取有效语音帧
			{

				switch(kz) //kz的初始值为0（静态变量）
				{
					case 0:
						{
							if (energy[i]>m_energy) //如果该帧平均能量大于m_energy，该帧为有效语音数据
							{
								num++;//变量初始值为0，对语音有效帧进行计数
								for (j=0;j<FRAMELEN;j++)//把该帧数据赋给m_vdata
								{
									m_vdata[m_framenum*FRAMELEN+j]=q[j];//最大存200帧，录音时间为6.4S
								}
								//ing4.Format("%d",i);
								//MessageBox(ing4);
								m_framenum++;//初始值为0，对语音有效帧进行计数（不是静态变量）
								kz=1;//改变kz的值执行下一步（继续寻找有效语音帧）
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
							else//如果出现一帧小于平均能量，将有效帧置零（即舍弃这些语音数据）
							{
								num=0;
								m_framenum=0;
								kz=0;
							}
							if (num>6)//如果有连续10帧（其实就是步长）语音大于平均能量这进行到case 2
							{
								kz=2;
							}
							break;
						}
					case 2://超过10个步长有效语音数据时（0.1s时间）一个步长时间为0.01s,后面出现的语音不管是大于平均能量还是小于平均能量都是有效语音
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
								if (num>200)//（即有效语音时间超过2S，舍弃这些语音数据）
								{
									m_framenum=0;
									kz=0;
									num=0;
									num1=0;
								}
							}
							else//如果出现有个步长小于平均能量才执行case 3，该步长还是为有效语音数据
							{
								num++;
								num1++;//判断语音结束标志加1
								for (j=0;j<FRAMELEN;j++)
								{
									m_vdata[m_framenum*FRAMELEN+j]=q[j];
								}
								//ing4.Format("%d",i);
								//MessageBox(ing4);
								m_framenum++;
								kz=3;
								if (num>198)//帧个数（即有效语音时间超过1.98S，舍弃这些语音数据）
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
							if (energy[i]<m_energy)  //判断语音结束
							{
								num1++;
								if (num1>10)//当有连续大于10帧语音时确定为有效语音终点，此时可以进行参数提取或匹配了，即语音间隔可以容忍0.1s
								{
									num1=0;
									kz=4;
								}
							}
							else//此处不是声音结束点，跳回执行case 2
							{
								num++;
								num1=0;//将判断声音结束标志置零
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
				q=q+FRAMELEN;//一个步长存了一帧语音数据，所以语音数据要偏移一个帧长

			}

		//	CString ingh;//////中间调试//////
			//ingh.Format("m_framenum %d",m_framenum);
//			MessageBox(ingh);
			/////////端点检测结束////////


			/*short m_vdatamax=fabs(m_vdata[1]);
			for(i=0;i<m_framenum*FRAMELEN;i++)//找最大
			{	
				if(fabs(m_vdata[i])>m_vdatamax)
					m_vdatamax=fabs(m_vdata[i]);
			}

			for(i=0;i<m_framenum*FRAMELEN;i++)
			{	
				outfile1<<10000*m_vdata[i]/m_vdatamax<<' ';//10000为归一化
			}*/

			short sum=0;
			for(i=0;i<m_framenum*FRAMELEN;i++)
			{	
				sum+=pow(m_vdata[i],2);
			}
			
			CString ingh;//////中间调试//////
			ingh.Format("m_framenum %d",sum);
		//	MessageBox(ingh);

			for(i=0;i<m_framenum*FRAMELEN;i++)
			{	
				outfile1<<short(1000*m_vdata[i]/sqrt(sum))<<' ';
			}



			/*outfile1<<"语音信号总帧数： "<<m_framenum;
			  
			outfile1<<"音频数据的大小: "<<waveheader.wSampleLength<<endl; 
			outfile1<<"标志符（RIFF）: "<<waveheader.rId<<endl;    
			outfile1<<"数据大小,包括数据头的大小和音频文件的大小: "<<waveheader.rLen<<endl;    //数据大小,包括数据头的大小和音频文件的大小
			outfile1<<"格式类型（WAVE）: "<<waveheader.wId<<endl;       //格式类型（"WAVE"）
			outfile1<<"fmt: "<<waveheader.fId<<endl; 
			outfile1<<"Sizeof(WAVEFORMATEX): "<<waveheader.fLen<<endl; 
			outfile1<<"编码格式，包括WAVE_FORMAT_PCM，WAVEFORMAT_ADPCM等: "<<waveheader.wFormatTag<<endl;   
			outfile1<<"声道数，单声道为1，双声道为2: "<<waveheader.nChannels<<endl;   
			outfile1<<"采样频率: "<<waveheader.nSamplesPerSec<<endl; 
   			outfile1<<"每秒的数据量: "<<waveheader.nAvgBytesPerSec<<endl;      
			outfile1<<"块对齐: "<<waveheader.nBlockAlign<<endl;  
			outfile1<<"WAVE文件的采样大小: "<<waveheader.wBitsPerSample<<endl; 
  			outfile1<<"data: "<<waveheader.dId<<endl;   
			*/
			outfile1.close();
			fclose(sourcefile);

			CString mfccData;
			mfccData.Format("李飞%d"+ordername+"mfcc",wj);
			ofstream outfileMfcc(mfccData+".txt");  //存储mfcc数据
			ifstream ifile(voicedata+".txt");     //从“试试”里面读入语音数据

			codetrain+=mfccd(outfileMfcc,ifile,outfileCode);      //特征提取

			free(pi);pi=NULL;
			free(data);data=NULL;
			free(energy);energy=NULL;
			free(m_vdata);m_vdata=NULL;

			ifile.close();
			outfileMfcc.close();
			}

        for(wj=1;wj<=1;wj++)//处理每个命令中的WENJIAN个语音文件
		{	
			ifstream infileCB("cb.txt");

			CString nameYasuo;
			nameYasuo.Format("李飞%d",wj);

			//CString nameMfcc;
			//nameMfcc.Format("mfcc%d",wj);
			ifstream infileYy(nameYasuo+ordername+"mfcc.txt");	

			ofstream outfileYs(nameYasuo+ordername+"压缩语音.txt");
			int VoiceLen=0;
			infileYy>>VoiceLen;//读帧数
			VoiceLen=(int)(VoiceLen*MFCCNUM/VEC_DEM);//将帧为度量的语音长度转换为以维数度量的语音长度，VEC_DEM=9维
			int* Code=(int*)calloc(VoiceLen,sizeof(int));//编码之后的数据
			double* Dispar=(double*)calloc(CB_SIZE,sizeof(double));
			int CodeBL=0;
			int i,j,vi;
			double CodeBook1[CB_SIZE][VEC_DEM];//码书

			/***************读码书***************/
			for(i=0;i<CB_SIZE;i++)
			{
				for(j=0;j<VEC_DEM;j++)
				{
					infileCB>>CodeBook1[i][j];				
				}
			}
			infileCB.close();

			//显示语音的长度（以维数度量）
			CString string;
			string.Format("%d",VoiceLen); 
			//MessageBox(string);

			/*********申请不定长二维数组存放语音数据**********/
			double** VoiceData=(double**)calloc(VoiceLen,sizeof(double*));
			for(int yi=0;yi<VoiceLen;yi++)
				 VoiceData[yi]=(double*)calloc(VEC_DEM,sizeof(double));

			/*********从语音文件中读入语音数据**********/
			for(i=0;i<VoiceLen;i++)
				for(j=0;j<VEC_DEM;j++)
				{
					infileYy>>VoiceData[i][j];
				}		
			infileYy.close();

			/*********语音编码**********/
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
			//录入编码后的语音数据
			outfileYs<<VoiceLen<<' ';
			for(i=0;i<VoiceLen;i++)
				outfileYs<<1+Code[i]<<' ';
			outfileYs.close();

			/**********释放内存***********/
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
	//nameofwj.Format("空白"+".wav",1);

	//CString voicedata;
	//voicedata.Format("空白"+"语音数据",1);

	TWavHeader waveheader;//读取.wav语音文件的头信息
	int jc=0;                //用于统计.wav问价语音的长度
	//int i;
	int m_framenum=0;         //端点检测后语音的帧数（帧长512的）
	short buffer[FrmLen];      //存储每次从.wav问价中读取的数据
	FILE *sourcefile;           //打开.wav文件

	ofstream outfile1("a空白语音数据.txt");           //将端点检测后的数据读入“试试.txt”
	outfile1.precision(4);

	sourcefile=fopen("空白.wav","rb");                     //打开.wav文件

	fread(&waveheader,sizeof(struct _TWavHeader),1,sourcefile);           //读取语音头信息
	while(fread(buffer,sizeof(short),FrmLen,sourcefile)==FrmLen)           //读取语音信号的长度
	{
		jc++;          //统计语音信号的长度
	}

	int cishu=0;        //用于统计读取第几次，（长度的时域信号）
	short *pi=(short *)calloc(jc*FrmLen,sizeof(short));    //为读取的语音信号分配内存
	short *p=pi;    

	fseek(sourcefile,sizeof(struct _TWavHeader),SEEK_SET);   //移动文件指针
	//fread(&waveheader,sizeof(struct _TWavHeader),1,sourcefile);
	long int sum=0;
	int ave=0;
	while(fread(buffer,sizeof(short),FrmLen,sourcefile)==FrmLen)    //实际读取语音数据。一帧（）
	{
		for(int i=0;i<FrmLen;++i)
		{
			p[FrmLen*cishu+i]=buffer[i];                     //将buffer里面的数据全部放到p指向的存储空间
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
	CString ing3;//////中间调试//////
    ing3.Format("%d",(int)(ave));
    MessageBox(ing3);



	free(pi);
	pi=NULL;
	outfile1.close();
	fclose(sourcefile);

}
