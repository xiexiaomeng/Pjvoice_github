
#ifndef AFHONADFSOAFDNOAFDNS
#define AFHONADFSOAFDNOAFDNS

#define  FRAMELEN  512  //����֡������
#define  STEPLEN   160
#define  DELPNUM   12
#define  PI         3.1415926
#define  CHANNEL    24  //�˲�������
#define  MFCCNUM    36  //
#define  WENJIAN    37 //ÿ��������Ӧ���ļ�����
#define  ORDERNUM   11

#include<iostream.h>
#include<fstream.h>


#define FS 16 //�����������??/////
#define FrmLen 512//�����������(16*20)??///
//#define FFTLen 512
#define FiltNum 25
#define PCEP 13
#define GmmNum 8
#define infinite 1e+32

typedef struct
{
	float real ;
	float image ;
} COMPLEX ;	
int GetMfcc(int framenum,float *data,float *mfcc);

float Dtw(float *muban,int mubannum,float *data,int datanum);

int mfccd(ofstream outfileMfcc,ifstream ifile,ofstream outfileCode);
int mfccf(float* mfcc,short* m_v,int mf);
#endif

