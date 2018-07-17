#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include "Lbg1.h"
#include<iostream.h>
#include<fstream.h>
#include<iomanip.h>
#include "rec.h"


double g_dist_threshold = 0.0001;
int tssize=0;
FILE* fp;

tTSVector *TrainingSet;

tCBVector CodeBook[CB_SIZE];

int Lbg()
{
	fp = fopen(TS_FILE, "r");
	fscanf(fp, "%d", &(tssize));
	tssize=tssize*MFCCNUM/VEC_DEM;

    //////////////////数组初始化//////////////////////////
	TrainingSet=(tTSVector*)malloc(sizeof(tTSVector)*tssize);
	for(int fci=0;fci<tssize;fci++)
	{
		for(int fcj=0;fcj<VEC_DEM;fcj++)
		{
			TrainingSet[fci].data[fcj]=0;
		}
		TrainingSet[fci].cluster=0;
	}
    ///////////////////////////////////////////////////////

	ReadTS();
	DoCluster();


	free(TrainingSet);
	TrainingSet=NULL;
	fclose(fp);

	AfxMessageBox("训练结束");
	return 0;
}
/////////////////////////////////////////////////////////////
//read training set from file
int ReadTS()
{
	//read training set
	for (int i=0; i<tssize; i++)   //#define TS_SIZE 603219	training set size
		for (int j=0; j<VEC_DEM; j++)   //VEC_DEM 9		vector dimension 码书维数
			fscanf(fp, "%lf", &(TrainingSet[i].data[j]));

	fclose(fp);

	return 0;
}

//init code book, using some vectors from training set
void InitCB()
{
	for (int i=0; i<CB_SIZE; i++) {
		for (int j=0; j<VEC_DEM; j++) {
			CodeBook[i].data[j] = TrainingSet[i * tssize / CB_SIZE].data[j];

			CodeBook[i].sum[j] = 0;
		}

		CodeBook[i].num = 0;
	}
}

//update code book, using new cluster center
void UpdateCB()
{
	for (int i=0; i<CB_SIZE; i++) {   // CB_SIZE 128 码书大小
		//if CodeBook[i] means a empty cluster
		//split cluster with max members, give a half of its members to CodeBook[i]
		if (CodeBook[i].num == 0) {
			//find cluster with max members
			int MaxCluster=0;
			for (int j=0; j<CB_SIZE; j++)
				MaxCluster = CodeBook[j].num > CodeBook[MaxCluster].num ? j: MaxCluster; 

			//number of the half of members
			int Num = CodeBook[MaxCluster].num/2;
			
			//clear this biggest cluster to refill
			for (j=0; j<VEC_DEM; j++ )
				CodeBook[MaxCluster].sum[j] = 0;
			CodeBook[MaxCluster].num = 0;
			
			//find all member training vector belonged to the original biggest cluster
			//assign first half to CodeBook[i], and later half to the original biggest cluster
			for (j=0; j<tssize; j++) {
				if (TrainingSet[j].cluster != MaxCluster)		
					continue;
				if (CodeBook[i].num < Num) {
					TrainingSet[j].cluster = i;
					for (int m=0; m<VEC_DEM; m++)
						CodeBook[i].sum[m] += TrainingSet[j].data[m];
					CodeBook[i].num++;
				}
				else {
					for (int m=0; m<VEC_DEM; m++)
						CodeBook[MaxCluster].sum[m] += TrainingSet[j].data[m];
					CodeBook[MaxCluster].num++;
				}
			}

			//recalculate the original biggest cluster center if necessary
			if (MaxCluster < i) {
				for( j=0; j<VEC_DEM; j++ ) {
					CodeBook[MaxCluster].data[j] = CodeBook[MaxCluster].sum[j] / CodeBook[MaxCluster].num;
					CodeBook[MaxCluster].sum[j] = 0;
				}
				CodeBook[MaxCluster].num = 0;
			}
		}

		//calculate new cluster center, clear sum and num for next round
		for (int j=0; j<VEC_DEM; j++) {
			CodeBook[i].data[j] = CodeBook[i].sum[j] / CodeBook[i].num; //calculate cluster center
			CodeBook[i].sum[j] = 0;
		}
	}

	//clear num at last
	for (i=0; i<CB_SIZE; i++)
		CodeBook[i].num = 0;
	
}


//calculate distortion of two vector, using different methods.
double CalDistortion(double *X, double *Y/*, tDistortionType method*/)
{
	int i;
	double sum = 0;
	
		for (i=0; i<VEC_DEM; i++)
			sum += (X[i] - Y[i]) * (X[i] - Y[i]);
	return sum;
}

//write code book to file
int WriteCB(char *fname, double dist)
{
	char fname2[128];

	sprintf(fname2, "2%s", fname);

	FILE *fp1 = fopen(fname, "wt");
	FILE *fp2 = fopen(fname2, "wt");
	
	if (!fp1 || !fp2) {
		perror("Create code book file error!");
		return -1;
	}
	
	fprintf(fp2, "distortion: %.21lf\n", dist);
	for (int i=0; i<CB_SIZE; i++) {
		fprintf(fp2, "Center%02d: ", i);
		for (int j=0; j<VEC_DEM; j++ ) {
			fprintf(fp1, "%0.5lf ", CodeBook[i].data[j]);
			fprintf(fp2, "%0.5lf ", CodeBook[i].data[j]);
		}
		fprintf( fp1, "\n");
		fprintf( fp2, "Num=%03d\n", CodeBook[i].num);
		
	}
	fclose(fp1);
	fclose(fp2);
	
	return 0;
}

//
void DoCluster()
{
	double d = BIG_NUMBER;		//big number, distortion(失真) of all clusters(集群)  （1e+10）
	double d_bak;				//bak of distortion of all clusters
	double e = SMALL_NUMBER;	//small number, threshold(门限) of training
	double rd = 1;				//relative distortion

	double v_dist;				//distortion between ts vector and cb vector
	double temp_v_dist;			//temp distortion

	char fname[128];

	long nRound = 0;

	do {
		d_bak = d;
		d = 0;  //as accumulator of distortion

		if (nRound++)
		{
			UpdateCB();
		}
		
		else
			InitCB();

		//clustering
		for (int i=0; i<tssize; i++) { //cluster every ts vector
			//which cb vector should a ts vector belong to

			v_dist = BIG_NUMBER;
			for (int j=0; j<CB_SIZE; j++) {
				temp_v_dist = CalDistortion(TrainingSet[i].data, CodeBook[j].data/*, DistType*/);
				if (temp_v_dist < v_dist) {
					v_dist = temp_v_dist;
					TrainingSet[i].cluster = j; //belong to this cb vector
				}
			}
			//this cluster member increase
			CodeBook[ TrainingSet[i].cluster ].num ++; 
			//sum up ts vector to calculate cluster center in UpdateCB
			for (j=0; j<VEC_DEM; j++)
				CodeBook[ TrainingSet[i].cluster ].sum[j] += TrainingSet[i].data[j];

			//add up distortion
			d += v_dist;
		}
		d= d/tssize;
		rd = fabs((d_bak-d)/d);

//		printf("==Finished the %3dth round training, rd = %.21lf\n", nRound, rd);

		if(rd < g_dist_threshold)
		{
			sprintf(fname, "cb.txt", /*g_dist_threshold*/0); //(%.021lf)
			WriteCB(fname, rd);
			g_dist_threshold = rd / 2;
		}
	}
	while (rd > e && nRound < MAX_ROUND);
}




