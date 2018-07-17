/************************************************************************
**      File:   hmm.h
**      ���ܣ��ṩHMM�����ݽṹ�Ͷ���
*/
#include "StdAfx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "rec.h"

typedef struct {
	int M; 		/* �۲�ֵ�ø���; V={1,2,...,M}*/
	int N;		/* ״̬�ĸ�����  Q={1,2,...,N} */
	double	**A;	/* A[1..N][1..N]. a[i][j] �Ǵ�tʱ�̵�״̬iת�Ƶ�t+1ʱ��
					��״̬j��ת�Ƹ���*/
	double	**B;	/* B[1..N][1..M]. b[j][k] ����״̬j�۲쵽�۲�ֵk�ĸ��� */
	double	*pi;	/* pi[1..N] pi[i] �ǽ���Markov����״̬�ֲ� */
} HMM;

//����HMM���ļ���ȡ����ʼ������������
void ReadHMM(FILE *fp, HMM *phmm);
void PrintHMM(FILE *fp, HMM *phmm);
void InitHMM(HMM *phmm, int N, int M, int seed);
void CopyHMM(HMM *phmm1, HMM *phmm2);
void FreeHMM(HMM *phmm);

//�������еĴ�ȡ��״̬�ļ���ȵ�
void ReadSequence(FILE *fp, int *pT, int **pO);
void PrintSequence(FILE *fp, int T, int *O);
void GenSequenceArray(HMM *phmm, int seed, int T, int *O, int *q);
int GenInitalState(HMM *phmm);
int GenNextState(HMM *phmm, int q_t);
int GenSymbol(HMM *phmm, int q_t);

//ǰ������㷨��BaumWelch�㷨
void Forward(HMM *phmm, int *T, int **O, double ***alpha, double ***alphaStar, double ***alphaWave, double **alphaWaveSum,
			 double *pprob);

void Backward(HMM *phmm, int *T, int **O, double ***beta, double ***betaStar, double ***betaWave, double **alphaWaveSum, 
			  double *pprob);

void BaumWelch(HMM *phmm, int *T, int **O, double ***alpha, double ***alphaStar, double ***alphaWave, double **alphaWaveSum,
	double ***beta, double ***betaStar, double ***betaWave,
	double ***gamma, /*int *pniter, */double *plogprobinit, double *plogprobfinal);
double **** AllocXi(int wenjian, int *T, int N);
void FreeXi(double **** xi, int wenjian , int *T, int N);
void ComputeGamma(HMM *phmm, int *T, double ***alphaStar, double ***betaStar, 
	double ***gamma);
void ComputeXi(HMM* phmm, int *T, int **O, double ***alphaStar, double ***betaStar, 
	double ****xi);
//Viterbi�㷨
void Viterbi(HMM *phmm, int T, int *O, double **delta, int **psi,
        int *q, double *pprob);
void ViterbiLog(HMM *phmm, int T, int *O, double **delta, int **psi,
        int *q, double *pprob);

//�������غ���

int hmmgetseed(void);
void hmmsetseed(int seed);
double hmmgetrand(void);
 
#define MAX(x,y)        ((x) > (y) ? (x) : (y))
#define MIN(x,y)        ((x) < (y) ? (x) : (y))
 

