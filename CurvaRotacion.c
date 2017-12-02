//Tarea 5 métodos computacionales 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 300
#define bb 0.2497
#define bd 5.16
#define ad 0.3105
#define ah 64.3
#define G 1

void cargarDatos(double *radioDat , double *velDat);
double likelihood (double *velDat,double *velModel);
void MyModel (double* velModel,double * R, double Mb, double Md, double Mh );
void iniciar (double* myLikelihood,double *velDat,double * R);
int main()
{
	double *radioDat, *velDat, *myLikelihood ;
	radioDat = malloc(N*sizeof(double));
	velDat = malloc(N*sizeof(double));
	myLikelihood = malloc(N*sizeof(double));
	cargarDatos(radioDat,velDat);
	iniciar(myLikelihood,velDat, radioDat);



	printf("%s\n" ,"Fin!" );
	return 0;
}

void cargarDatos(double *radioDat , double *velDat)
{
	FILE *dat;
	dat= fopen("RadialVelocities.dat", "r");
	if (!dat)
	{
		printf("Problemas abriendo el archivos %s\n", "RadialVelocities.dat" );
		exit(1);
	}

	char line_buffer[30];
	fscanf(dat, "%s ", &line_buffer);
	fscanf(dat, "%s\n", &line_buffer);

	double rval;
	double velval;	
	for (int i=0; i<N;i++ )
	{
		fscanf(dat, "%lf\n", &rval);
		fscanf(dat, "%lf", &velval);
		radioDat[i]= rval;
		velDat[i]=velval;		
	}
	fclose(dat);
}

double likelihood (double *velDat,double *velModel)
{
	double valor =0;
	for (int i = 0; i < N; ++i)
	{
		valor += pow((velDat[i] - velModel[i]),2)*0.5;
	}

	return valor;
}

void MyModel (double* velModel,double* R, double Mb, double Md, double Mh )
{
	for (int i = 0; i < N; ++i)
	{
		double term1 = (pow(Mb,0.5)*R[i])/pow((pow(R[i],2)+pow(bb,2)),3.0/4.0);
		double term2 = (pow(Md,0.5)*R[i])/pow((pow(R[i],2)+pow(bb + ad,2)),3.0/4.0);
		double term3 = (pow(Mh,0.5)*R[i])/pow((pow(R[i],2)+pow(ah,2)),1.0/4.0);
		velModel[i] = term1 + term2 + term3;
	}
}

void iniciar (double* myLikelihood, double *velDat ,double * R)
{
	double M1 = drand48();
	double M2 = drand48();
	double M3 = drand48();
	double velModel [N];
	MyModel(velModel,R, M1,M2,M3);
	myLikelihood[0] = likelihood(velDat,velModel);
}