/*
**      File:   hmmutils.cpp
**      ���ܣ�HMM�ļ��Ķ�д�Ȳ���
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
** �������ƣ�ReadHMM
** ���ܣ���ȡHMM�ṹ
** ������fp���ļ�ָ��
**       phmm��HMM�ṹָ�룬����HMM�ṹ
** ����ֵ����
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
** �������ƣ�FreeHMM
** ���ܣ��ͷ�HMM�ṹ
** ������phmm��HMM�ṹָ��
** ����ֵ����
**/
void FreeHMM(HMM *phmm)
{
	free_dmatrix(phmm->A, 1, phmm->N, 1, phmm->N);
	free_dmatrix(phmm->B, 1, phmm->N, 1, phmm->M);
	free_dvector(phmm->pi, 1, phmm->N);
}

/**********************************************************************
** �������ƣ�InitHMM
** ���ܣ���ʼ��HMM�ṹ
** ������phmm��HMM�ṹָ��
**       N��״̬��
**       M���ɹ۲�ֵ�ø���
**       seed�����������
** ����ֵ����
**/

///////////////////ԭʼ��///////////////////////////
/*void InitHMM(HMM *phmm, int N, int M, int seed)
{
	int i, j, k;
	double sum;

	////��ʼ������������� 
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
///////////////////ԭʼ��///////////////////////////

void InitHMM(HMM *phmm, int N, int M, int seed)
{
	int i, j, k;
	double sum;
	
	ofstream outfiledm("��ʼHMM.txt");

	//��ʼ������������� 
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
	
	ofstream outfiledm("��ʼHMM.txt");

	// ��ʼ������������� 
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
** �������ƣ�CopyHMM
** ���ܣ�����HMM�ṹ
** ������phmm1��HMM�ṹָ��
**       phmm2��HMM�ṹָ��
** ����ֵ����
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
** �������ƣ�PrintHMM
** ���ܣ�����HMM�ṹ
** ������fp���ļ�ָ��
**       phmm��HMM�ṹָ��
** ����ֵ����
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

