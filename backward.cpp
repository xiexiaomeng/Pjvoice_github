/*
**      File:   backward.cpp
**      ���ܣ������۲�ֵ���к�HMMģ�ͣ�����ǰ������㷨
**            ��ȡ�����
*/
#include "StdAfx.h"
#include <stdio.h>
#include "hmm.h"
#include <iostream.h>
#include <fstream.h>
#include "rec.h"

/***************************************************************************
** �������ƣ�Forward
** ���ܣ�ǰ���㷨���Ʋ���
** ������phmm��ָ��HMM��ָ�룬��ȡHMM����
**       T���۲�ֵ���еĳ���
**       O���۲�ֵ���У�����
**       aipha���������õ�����ʱ����
**       aiphaStar���������õ�����ʱ����
**       aiphaWave���������õ�����ʱ����
**       aiphaWaveSum���������õ�����ʱ����
**       pprob������ֵ����Ҫ��ĸ���
**/
void Forward(HMM *phmm, int *T, int **O, double ***aipha ,double ***aiphaStar, double ***aiphaWave, 
			 double **aiphaWaveSum, double *pprob)
{
	int     i, j, k;   /* ״̬ */
	int     t;      /* ʱ���±� */
	double sum1,sum2;//�м���������ں����ĵݹ�

 
	/* 1. ��ʼ�� */
	for(int wenj=1; wenj<=WENJIAN; wenj++)
	{
		aiphaWaveSum[wenj][1]=0.0;    //2014.12.11��

	    for (i = 1; i <= phmm->N; i++)//NΪģ����������Ʒ���״̬��
		{
			aipha[wenj][1][i] = phmm->pi[i]*(phmm->B[i][O[wenj][1]]);//��ʼ��
			aiphaWaveSum[wenj][1] +=aipha[wenj][1][i];
		}

		for(i = 1; i <= phmm->N; i++)
		{
			aiphaStar[wenj][1][i] = aipha[wenj][1][i]/aiphaWaveSum[wenj][1];
		}
	}
	

	/* 2. �ݹ� */

	for(wenj=1; wenj<=WENJIAN; wenj++)
	{
		for (t = 1; t <= T[wenj]-1; t++) 
		{
			for (j = 1; j <= phmm->N; j++) //j=1,...,N
			{
				sum1= 0.0;
				for (i = 1; i <= phmm->N; i++)
				{
					sum1 += phmm->A[i][j]*(aiphaStar[wenj][t][i]);
				}
				aiphaWave[wenj][t+1][j] = sum1 * (phmm->B[j][O[wenj][t+1]]);//��������
				sum2=0.0;
				for(k = 1; k <= phmm->N ; k++)
				{
					sum2 += aiphaWave[wenj][t+1][k];
				}
				aiphaWaveSum[wenj][t+1]=sum2;

				aiphaStar[wenj][t+1][j] = aiphaWave[wenj][t+1][j] / aiphaWaveSum[wenj][t+1];
			}

			for (j = 1; j <= phmm->N; j++) //j=1,...,N,�������������
			{
				aiphaStar[wenj][t+1][j] = aiphaWave[wenj][t+1][j] / aiphaWaveSum[wenj][t+1];
			}

		}
	}

 
	
	/* 3. ��ֹ */
	//*pprob = 1.0;
	*pprob = 0.0;

	for(wenj=1; wenj<=WENJIAN; wenj++)
	{
		for (i = 1; i <= T[wenj] ; i++)//�������P����ѵ�����μ����Ϲ�ʽ
		{
			//*pprob += fabs(log(aiphaWaveSum[wenj][i]));
			*pprob += log(aiphaWaveSum[wenj][i]);//2014.12.11��
		}
	}
}


/***************************************************************************
** �������ƣ�Backward
** ���ܣ������㷨���Ʋ���
** ������phmm��ָ��HMM��ָ��
**       T���۲�ֵ���еĳ���
**       O���۲�ֵ����
**       beta���������õ�����ʱ����
**       betaStar���������õ�����ʱ����
**       betaWave���������õ�����ʱ����
**       alphaWaveSum:��ʱ����
**       pprob������ֵ����Ҫ��ĸ���
**/
void Backward(HMM *phmm, int *T, int **O, double ***beta, 
	 double ***betaStar, double ***betaWave, double **aiphaWaveSum , double *pprob)
{
	int     i, j;   /* ״̬ */
	int     t;      /* ʱ���±� */
	double sum;
 
 
	/* 1. ��ʼ�� */
 
	for(int wenj=1; wenj<=WENJIAN; wenj++)
	{
		for (i = 1; i <= phmm->N; i++)
		{
			beta[wenj][T[wenj]][i] = 1.0;
			betaStar[wenj][T[wenj]][i]= 1.0;
		}
	}

 
	/* 2. �ݹ� */

    for(wenj=1; wenj<=WENJIAN; wenj++)
	{
		for (t = T[wenj] - 1; t >= 1; t--) 
		{
			for (i = 1; i <= phmm->N; i++) 
			{
				sum = 0.0;
				for (j = 1; j <= phmm->N; j++)
					sum += phmm->A[i][j] * (phmm->B[j][O[wenj][t+1]])* betaStar[wenj][t+1][j];
				betaWave[wenj][t][i] = sum;

				betaStar[wenj][t][i] = betaWave[wenj][t][i] / aiphaWaveSum[wenj][t+1];
			}
		}
 
	}

	/* 3. ��ֹ, ��ʵ�ʵ��㷨���ò��� */
/*	*pprob = 1.0;
	for (i = 1; i <= T; i++)//�������P����ѵ�����μ����Ϲ�ʽ
		*pprob *= aiphaWaveSum[t];
*/
}
