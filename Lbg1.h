
//#include  <StdAfx.h>
/////////////////////////////////////////////////////////
//#define TS_SIZE 2160	//training set size
#define CB_SIZE 128		//code book size 码书大小
#define VEC_DEM 9		//vector dimension 码书维数

#define MAX_ROUND 2000
#define BIG_NUMBER (1e+10)
#define SMALL_NUMBER (1e-20)

#define TS_FILE "码书训练.txt"
#define CB_FILE "cb.txt"
#define CB_FILE_2 "cb2.txt"

typedef struct{	//training set vector
	double data[VEC_DEM];	
	int	cluster;			//cluster belong to 
}tTSVector;

typedef struct{	//code book vector
	double data[VEC_DEM];	//vector data
	int	num;				//number of vectors in this cluster
	double sum[VEC_DEM];	//sum of vectors in this cluster, to calculate cluster center
}tCBVector;

int ReadTS();
void InitCB();
void UpdateCB();
double CalDistortion(double *X, double *Y);
int WriteCB(char *fname, double dist);
void DoCluster();
int Lbg();
