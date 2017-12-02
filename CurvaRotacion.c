//Tarea 5 métodos computacionales 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 300

void cargarDatos(double *radioDat , double *velDat);
int main()
{
	double *radioDat, *velDat;
	radioDat = malloc(N*sizeof(double));
	velDat = malloc(N*sizeof(double));
	cargarDatos(radioDat,velDat);


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
	printf("%s\n", line_buffer);
	fscanf(dat, "%s\n", &line_buffer);
	printf("%s\n", line_buffer);
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