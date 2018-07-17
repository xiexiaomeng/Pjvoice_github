/*
**      File:   backward.cpp
**      功能：给定观察值序列和HMM模型，利用前向后向算法
**            求取其概率
*/
#include "StdAfx.h"
#include <stdio.h>
#include "hmm.h"
#include <iostream.h>
#include <fstream.h>
#include "rec.h"

/***************************************************************************
** 函数名称：Forward
** 功能：前向算法估计参数
** 参数：phmm：指向HMM的指针，读取HMM参数
**       T：观察值序列的长度
**       O：观察值序列，数组
**       aipha：运算中用到的临时数组
**       aiphaStar：运算中用到的临时数组
**       aiphaWave：运算中用到的临时数组
**       aiphaWaveSum：运算中用到的临时数组
**       pprob：返回值，所要求的概率
**/
void Forward(HMM *phmm, int *T, int **O, double ***aipha ,double ***aiphaStar, double ***aiphaWave, 
			 double **aiphaWaveSum, double *pprob)
{
	int     i, j, k;   /* 状态 */
	int     t;      /* 时间下标 */
	double sum1,sum2;//中间变量，用于函数的递归

 
	/* 1. 初始化 */
	for(int wenj=1; wenj<=WENJIAN; wenj++)
	{
		aiphaWaveSum[wenj][1]=0.0;    //2014.12.11改

	    for (i = 1; i <= phmm->N; i++)//N为模型中隐马尔科夫链状态数
		{
			aipha[wenj][1][i] = phmm->pi[i]*(phmm->B[i][O[wenj][1]]);//初始化
			aiphaWaveSum[wenj][1] +=aipha[wenj][1][i];
		}

		for(i = 1; i <= phmm->N; i++)
		{
			aiphaStar[wenj][1][i] = aipha[wenj][1][i]/aiphaWaveSum[wenj][1];
		}
	}
	

	/* 2. 递归 */

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
				aiphaWave[wenj][t+1][j] = sum1 * (phmm->B[j][O[wenj][t+1]]);//出问题了
				sum2=0.0;
				for(k = 1; k <= phmm->N ; k++)
				{
					sum2 += aiphaWave[wenj][t+1][k];
				}
				aiphaWaveSum[wenj][t+1]=sum2;

				aiphaStar[wenj][t+1][j] = aiphaWave[wenj][t+1][j] / aiphaWaveSum[wenj][t+1];
			}

			for (j = 1; j <= phmm->N; j++) //j=1,...,N,这里可能有问题
			{
				aiphaStar[wenj][t+1][j] = aiphaWave[wenj][t+1][j] / aiphaWaveSum[wenj][t+1];
			}

		}
	}

 
	
	/* 3. 终止 */
	//*pprob = 1.0;
	*pprob = 0.0;

	for(wenj=1; wenj<=WENJIAN; wenj++)
	{
		for (i = 1; i <= T[wenj] ; i++)//计算概率P用于训练，参见书上公式
		{
			//*pprob += fabs(log(aiphaWaveSum[wenj][i]));
			*pprob += log(aiphaWaveSum[wenj][i]);//2014.12.11改
		}
	}
}


/***************************************************************************
** 函数名称：Backward
** 功能：后向算法估计参数
** 参数：phmm：指向HMM的指针
**       T：观察值序列的长度
**       O：观察值序列
**       beta：运算中用到的临时数组
**       betaStar：运算中用到的临时数组
**       betaWave：运算中用到的临时数组
**       alphaWaveSum:临时数组
**       pprob：返回值，所要求的概率
**/
void Backward(HMM *phmm, int *T, int **O, double ***beta, 
	 double ***betaStar, double ***betaWave, double **aiphaWaveSum , double *pprob)
{
	int     i, j;   /* 状态 */
	int     t;      /* 时间下标 */
	double sum;
 
 
	/* 1. 初始化 */
 
	for(int wenj=1; wenj<=WENJIAN; wenj++)
	{
		for (i = 1; i <= phmm->N; i++)
		{
			beta[wenj][T[wenj]][i] = 1.0;
			betaStar[wenj][T[wenj]][i]= 1.0;
		}
	}

 
	/* 2. 递归 */

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

	/* 3. 终止, 在实际的算法中用不到 */
/*	*pprob = 1.0;
	for (i = 1; i <= T; i++)//计算概率P用于训练，参见书上公式
		*pprob *= aiphaWaveSum[t];
*/
}
