/*
**      File:   baumwelch.cpp
**      ���ܣ����ݸ����Ĺ۲����У���BaumWelch�㷨����HMMģ�Ͳ���
*/

#include "StdAfx.h"
#include <stdio.h> 
#include "nrutil.h"
#include "hmm.h"
#include <math.h>
#include<iostream.h>
#include<fstream.h>
#include<iomanip.h>

//#define DELTA 100
#define DELTA 43000


/******************************************************************************
**�������ƣ�BaumWelch
**���ܣ�BaumWelch�㷨
**������phmm��HMMģ��ָ��
**      T���۲����г���
**      O���۲�����
**      alpha��beta��gamma��pniter��Ϊ�м����
**      plogprobinit����ʼ����
**      ���ո���
**/ 
void BaumWelch(HMM *phmm, int *T, int **O, double ***alpha, double ***alphaStar, double ***alphaWave, double **alphaWaveSum,
	double ***beta, double ***betaStar, double ***betaWave,
	double ***gamma, /*int *pniter,*/ double *plogprobinit, double *plogprobfinal)
{
	int	i, j, k;
	int	t, l = 0;

	//*plogprobinit=0;*plogprobfinal=0;/////////////

	double	logprobf, logprobb;
	double	numeratorA, denominatorA;
	double	numeratorB, denominatorB;

	double ****xi;
	double delta, deltaprev, logprobprev;

	deltaprev = 10e-70;
	xi = AllocXi(WENJIAN , T, phmm->N);
	Forward(phmm, T, O, alpha, alphaStar, alphaWave, alphaWaveSum, &logprobf);	//ǰ��
	*plogprobinit = logprobf; // log P(O |��ʼ״̬) 
	Backward(phmm, T, O, beta, betaStar, betaWave, alphaWaveSum, &logprobb);  //����
	ComputeGamma(phmm, T, alphaStar, betaStar, gamma);   //alphaStar, betaStar�Ļ�
	ComputeXi(phmm, T, O, alphaStar, betaStar, xi);     //a[i][j]�ع��ķ��Ӽ���
	logprobprev = logprobf;


	int shishi=22;///////////////////////////
	
	do  {	

		// ���¹��� t=1 ʱ��״̬Ϊi ��Ƶ�� 
		for (i = 1; i <= phmm->N; i++) 
		{	
			double gamma1i=0.0;
			for(int ip=1; ip<= WENJIAN; ip++)
			{
				gamma1i=gamma1i+gamma[ip][1][i];//
			}
			phmm->pi[i] = .00001 + .99999*gamma1i;
			//phmm->pi[i] = gamma1i;
		}

		// ���¹���ת�ƾ���͹۲���� 
		for (i = 1; i <= phmm->N; i++) 
		{ 
			denominatorA = 0.0;
			denominatorB = 0.0;

			for(int wenj=1; wenj<= WENJIAN; wenj++)
			{
				for (t = 1; t <= T[wenj] - 1; t++) 
				{
					denominatorA += gamma[wenj][t][i];
				}
			}


			for (j = 1; j <= phmm->N; j++) 
			{
				numeratorA = 0.0;

				for(int wenj1=1; wenj1<= WENJIAN; wenj1++)
				{
		            for (t = 1; t <= T[wenj1] - 1; t++) 
						
					{
						numeratorA += xi[wenj1][t][i][j]/alphaWaveSum[wenj1][t+1];
            			//numeratorA += xi[wenj][t][i][j];
					}
				}
			

				phmm->A[i][j] = .00001 + .99999*numeratorA/denominatorA;
				//phmm->A[i][j] = numeratorA/denominatorA;
			}

			double gammaTi=0.0;
			for(int Ti=1; Ti<=WENJIAN ; Ti++)
			{
				gammaTi=gammaTi+gamma[Ti][T[Ti]][i];

			}
			denominatorB = denominatorA + gammaTi; 

			for (k = 1; k <= phmm->M; k++) 
			{
				numeratorB = 0.0;

				for(int wenj2=1; wenj2<= WENJIAN; wenj2++)
				{
					for (t = 1; t <= T[wenj2]; t++) 
					{
						if (O[wenj2][t] == k) 
							numeratorB += gamma[wenj2][t][i];
					}
				}


				phmm->B[i][k] = .00001 +.99999*numeratorB/denominatorB;
			//	phmm->B[i][k] = numeratorB/denominatorB;
			}
		}

		Forward(phmm, T, O, alpha, alphaStar, alphaWave, alphaWaveSum, &logprobf);	//ǰ��
	    Backward(phmm, T, O, beta, betaStar, betaWave, alphaWaveSum,&logprobb);//����
	    ComputeGamma(phmm, T, alphaStar, betaStar, gamma);
	    ComputeXi(phmm, T, O, alphaStar, betaStar, xi);
		// ��������ֱ�ӵĸ��ʲ� 
		delta = fabs(logprobf - logprobprev); 

		logprobprev = logprobf;
		l++;

	    CString strdelta;
	    strdelta.Format("%lf", delta);
	    //AfxMessageBox(strdelta);

     shishi--;///////////////////////////
	}while (shishi > 5); //  �����Ĳ�̫�󣬱����������˳� ///////////////////////////////
//	}while (delta > DELTA); //  �����Ĳ�̫�󣬱����������˳� 
 
//	*pniter = l;

	CString str;
	str.Format("ѭ������%d",l);
	//AfxMessageBox(str);

	*plogprobfinal = logprobf; // log P(O|estimated model) 
	FreeXi(xi, WENJIAN , T, phmm->N);
//	free_dvector(scale, 1, T);

	//AfxMessageBox("123456");
}

/************************************************************************
**���û��ӿں���
**/
void ComputeGamma(HMM *phmm, int *T, double ***alphaStar, double ***betaStar, 
	double ***gamma)
{

	int 	i, j;
	int	t;
	//double	denominator[WENJIAN];
	double	denominator;

	for(int wenj=1; wenj<=WENJIAN; wenj++)
	{
		for (t = 1; t <= T[wenj]; t++) 
		{
			//denominator[wenj] = 0.0;
			denominator = 0.0;
			for (j = 1; j <= phmm->N; j++) 
			{
				gamma[wenj][t][j] = alphaStar[wenj][t][j]*betaStar[wenj][t][j];
				//denominator[wenj] += gamma[wenj][t][j];
				denominator += gamma[wenj][t][j];
			}

			for (i = 1; i <= phmm->N; i++) 
			{
				//gamma[wenj][t][i] = gamma[wenj][t][i]/denominator[wenj];
				gamma[wenj][t][i] = gamma[wenj][t][i]/denominator;
			}
		}
	}
}

/************************************************************************
**���û��ӿں���
**/
void ComputeXi(HMM* phmm, int *T, int **O, double ***alphaStar, double ***betaStar, 
	double ****xi)
{
	int i, j;
	int t;
	double sum;

	for(int wenj=1; wenj<=WENJIAN ;wenj++)
	{
		for (t = 1; t <= T[wenj] - 1; t++) {
			sum = 0.0;	
			for (i = 1; i <= phmm->N; i++) 
				for (j = 1; j <= phmm->N; j++) 
				{
					xi[wenj][t][i][j] = alphaStar[wenj][t][i]*betaStar[wenj][t+1][j]
						*(phmm->A[i][j])
						*(phmm->B[j][O[wenj][t+1]]);
					sum += xi[wenj][t][i][j];
				}

			for (i = 1; i <= phmm->N; i++) 
				for (j = 1; j <= phmm->N; j++) 
					xi[wenj][t][i][j]  /= sum;
		}
	}
}

/************************************************************************
**���û��ӿں���
**/
double **** AllocXi(int wenjian, int *T, int N)
{
	int t;
	double ****xi;
	xi=(double ****) calloc(wenjian,sizeof(double ***));
	xi--;
	for(int wenj=1;wenj<=wenjian;wenj++)
	{
		xi[wenj] = (double ***) calloc(T[wenj],sizeof(double **));
		//(ԭ����)xi = (double ***) malloc(T*sizeof(double **));

		xi[wenj] --;

		for (t = 1; t <= T[wenj]; t++)
			xi[wenj][t] = dmatrix(1, N, 1, N);
	}

	return xi;
}

/************************************************************************
**���û��ӿں���
**/
void FreeXi(double **** xi, int wenjian , int *T, int N)
{
	int t;
	int wenj;
	for(wenj=1; wenj<=wenjian; wenj++)
	{
		for (t = 1; t <= T[wenj]; t++)
			free_dmatrix(xi[wenj][t], 1, N, 1, N);

		xi[wenj] ++;
		free(xi[wenj]);
		//xi[wenj]=NULL;////////////////
	}
    xi++;
	free(xi);
	//xi=NULL;////////////////////////
}
