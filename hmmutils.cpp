/*
**      File:   hmmutils.cpp
**      功能：HMM文件的读写等操作
*/

#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nrutil.h"
#include "hmm.h"
#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>

/**********************************************************************
** 函数名称：ReadHMM
** 功能：读取HMM结构
** 参数：fp：文件指针
**       phmm：HMM结构指针，保存HMM结构
** 返回值：无
**/
void ReadHMM(FILE *fp, HMM *phmm)
{
	int i, j, k;

	fscanf(fp, "%d\n", &(phmm->M)); 

	fscanf(fp, "%d\n", &(phmm->N)); 

	//fscanf(fp, "\n");
	phmm->A = (double **) dmatrix(1, phmm->N, 1, phmm->N);
	for (i = 1; i <= phmm->N; i++) { 
		for (j = 1; j <= phmm->N; j++) {
			fscanf(fp, "%lf", &(phmm->A[i][j])); 
		}
		fscanf(fp,"\n");
	}

	//fscanf(fp, "\n");
	phmm->B = (double **) dmatrix(1, phmm->N, 1, phmm->M);
	for (j = 1; j <= phmm->N; j++) { 
		for (k = 1; k <= phmm->M; k++) {
			fscanf(fp, "%lf", &(phmm->B[j][k])); 
		}
		fscanf(fp,"\n");
	}

	//fscanf(fp, "\n");
	phmm->pi = (double *) dvector(1, phmm->N);
	for (i = 1; i <= phmm->N; i++) 
		fscanf(fp, "%lf", &(phmm->pi[i])); 

}

/**********************************************************************
** 函数名称：FreeHMM
** 功能：释放HMM结构
** 参数：phmm：HMM结构指针
** 返回值：无
**/
void FreeHMM(HMM *phmm)
{
	free_dmatrix(phmm->A, 1, phmm->N, 1, phmm->N);
	free_dmatrix(phmm->B, 1, phmm->N, 1, phmm->M);
	free_dvector(phmm->pi, 1, phmm->N);
}

/**********************************************************************
** 函数名称：InitHMM
** 功能：初始化HMM结构
** 参数：phmm：HMM结构指针
**       N：状态数
**       M：可观察值得个数
**       seed：随机数种子
** 返回值：无
**/

///////////////////原始的///////////////////////////
/*void InitHMM(HMM *phmm, int N, int M, int seed)
{
	int i, j, k;
	double sum;

	////初始化随机数发生器 
	hmmsetseed(seed);	

	phmm->M = M;
 
	phmm->N = N;
 
	phmm->A = (double **) dmatrix(1, phmm->N, 1, phmm->N);

	for (i = 1; i <= phmm->N; i++) 
	{
		sum = 0.0;
		for (j = 1; j <= phmm->N; j++) 
		{
			phmm->A[i][j] = hmmgetrand(); 
			sum += phmm->A[i][j];
		}
		for (j = 1; j <= phmm->N; j++) 
			phmm->A[i][j] /= sum;
	}
 
	phmm->B = (double **) dmatrix(1, phmm->N, 1, phmm->M);

	for (j = 1; j <= phmm->N; j++) 
	{
		sum = 0.0;	
		for (k = 1; k <= phmm->M; k++) 
		{
			phmm->B[j][k] = hmmgetrand();
			sum += phmm->B[j][k];
		}
		for (k = 1; k <= phmm->M; k++) 
			phmm->B[j][k] /= sum;
	}
 
	phmm->pi = (double *) dvector(1, phmm->N);
	sum = 0.0;
	for (i = 1; i <= phmm->N; i++) 
	{
		phmm->pi[i] = hmmgetrand(); 
		sum += phmm->pi[i];
	}

	for (i = 1; i <= phmm->N; i++) 
		phmm->pi[i] /= sum;
}*/
///////////////////原始的///////////////////////////

void InitHMM(HMM *phmm, int N, int M, int seed)
{
	int i, j, k;
	double sum;
	
	ofstream outfiledm("初始HMM.txt");

	//初始化随机数发生器 
	hmmsetseed(seed);	

	phmm->M = M;
	outfiledm<<phmm->M<<endl;////
 
	phmm->N = N;
	outfiledm<<phmm->N<<endl;////
 
	phmm->A = (double **) dmatrix(1, phmm->N, 1, phmm->N);
/*
	for (i = 1; i <= phmm->N; i++) 
	{
		sum = 0.0;
		for (j = 1; j <= phmm->N; j++) 
		{
			phmm->A[i][j] = hmmgetrand(); 
		
			sum += phmm->A[i][j];
		}
		for (j = 1; j <= phmm->N; j++) 
		{
			phmm->A[i][j] /= sum;
			outfiledm<<phmm->A[i][j]<<' ';////
		}
		outfiledm<<endl;
			
	}
*/
	for (i = 1; i < phmm->N; i++) 
	{
		sum = 0.0;
		for (j = i; j <= i+1; j++) 
		{
			phmm->A[i][j] = hmmgetrand(); 
		
			sum += phmm->A[i][j];
		}
		for (j = 1; j <= phmm->N; j++) 
		{
			phmm->A[i][j] /= sum;
			outfiledm<<phmm->A[i][j]<<' ';////
		}
		outfiledm<<endl;
			
	}
	phmm->A[phmm->N][phmm->N] = 1.0;
	for(i = 1; i <= phmm->N; i++)
		outfiledm<<phmm->A[phmm->N][i]<<' ';
	outfiledm<<endl;
	
 
	phmm->B = (double **) dmatrix(1, phmm->N, 1, phmm->M);

	for (j = 1; j <= phmm->N; j++) 
	{
		sum = 0.0;	
		for (k = 1; k <= phmm->M; k++) 
		{
			phmm->B[j][k] = hmmgetrand();
			sum += phmm->B[j][k];
		}
		for (k = 1; k <= phmm->M; k++) 
		{
			phmm->B[j][k] /= sum;
			outfiledm<<phmm->B[j][k]<<' ';////
		}
		outfiledm<<endl;
	}
 
	phmm->pi = (double *) dvector(1, phmm->N);
	sum = 0.0;
	for (i = 1; i <= phmm->N; i++) 
	{
		phmm->pi[i] = hmmgetrand(); 
		sum += phmm->pi[i];
	}

	for (i = 1; i <= phmm->N; i++) 
	{
		phmm->pi[i] /= sum;
		outfiledm<<phmm->pi[i]<<' ';////
	}
	outfiledm<<endl;////
	outfiledm.close();
}

/*
void InitHMM(HMM *phmm, int N, int M, int seed)
{
	int i, j, k;
	double sum;
	
	ofstream outfiledm("初始HMM.txt");

	// 初始化随机数发生器 
	hmmsetseed(seed);	

	phmm->M = M;
	outfiledm<<phmm->M<<endl;////
 
	phmm->N = N;
	outfiledm<<phmm->N<<endl;////
 
	phmm->A = (double **) dmatrix(1, phmm->N, 1, phmm->N);

	for (i = 1; i <= phmm->N; i++) 
	{
		sum = 0.0;
		for (j = 1; j <= phmm->N; j++) 
		{
			phmm->A[i][j] = 1.0; 
		
			sum += phmm->A[i][j];
		}
		for (j = 1; j <= phmm->N; j++) 
		{
			phmm->A[i][j] /= sum;
			outfiledm<<phmm->A[i][j]<<' ';////
		}
		outfiledm<<endl;
			
	}
 
	phmm->B = (double **) dmatrix(1, phmm->N, 1, phmm->M);

	for (j = 1; j <= phmm->N; j++) 
	{
		sum = 0.0;	
		for (k = 1; k <= phmm->M; k++) 
		{
			phmm->B[j][k] = 1.0;
			sum += phmm->B[j][k];
		}
		for (k = 1; k <= phmm->M; k++) 
		{
			phmm->B[j][k] /= sum;
			outfiledm<<phmm->B[j][k]<<' ';////
		}
		outfiledm<<endl;
	}
 
	phmm->pi = (double *) dvector(1, phmm->N);
	sum = 0.0;
	for (i = 1; i <= phmm->N; i++) 
	{
		phmm->pi[i] = 1.0; 
		sum += phmm->pi[i];
	}

	for (i = 1; i <= phmm->N; i++) 
	{
		phmm->pi[i] /= sum;
		outfiledm<<phmm->pi[i]<<' ';////
	}
	outfiledm<<endl;////
	outfiledm.close();
}
*/


/**********************************************************************
** 函数名称：CopyHMM
** 功能：拷贝HMM结构
** 参数：phmm1：HMM结构指针
**       phmm2：HMM结构指针
** 返回值：无
**/
void CopyHMM(HMM *phmm1, HMM *phmm2)
{
	int i, j, k;
 
	phmm2->M = phmm1->M;

	phmm2->N = phmm1->N;
 
	phmm2->A = (double **) dmatrix(1, phmm2->N, 1, phmm2->N);
 
	for (i = 1; i <= phmm2->N; i++)
		for (j = 1; j <= phmm2->N; j++)
			phmm2->A[i][j] = phmm1->A[i][j];
 
	phmm2->B = (double **) dmatrix(1, phmm2->N, 1, phmm2->M);
	for (j = 1; j <= phmm2->N; j++)
		for (k = 1; k <= phmm2->M; k++)
			phmm2->B[j][k] = phmm1->B[j][k];
 
	phmm2->pi = (double *) dvector(1, phmm2->N);

	for (i = 1; i <= phmm2->N; i++)
		phmm2->pi[i] = phmm1->pi[i]; 
}

/**********************************************************************
** 函数名称：PrintHMM
** 功能：保存HMM结构
** 参数：fp：文件指针
**       phmm：HMM结构指针
** 返回值：无
**/
void PrintHMM(FILE *fp, HMM *phmm)
{
	int i, j, k;

	fprintf(fp, "%d\n", phmm->M); 
	fprintf(fp, "%d\n", phmm->N); 
 
	//fprintf(fp, "\n");
	for (i = 1; i <= phmm->N; i++) 
	{
		for (j = 1; j <= phmm->N; j++) 
		{
			fprintf(fp, "%f ", phmm->A[i][j] );
		}
		fprintf(fp, "\n");
	}
 
	//fprintf(fp, "\n");
	for (j = 1; j <= phmm->N; j++) 
	{
		for (k = 1; k <= phmm->M; k++)
		{
			fprintf(fp, "%f ", phmm->B[j][k]);
		}
		fprintf(fp, "\n");
	}
 
	//fprintf(fp, "\n");
	for (i = 1; i <= phmm->N; i++) 
	{
		fprintf(fp, "%f ", phmm->pi[i]);
	}

	fprintf(fp, "\n\n");
}

