//Tarea 5 métodos computacionales 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<time.h>
#define N 300
//Definimos k como el numero de masas que vamos a Hallar
#define k 3000
#define bb 0.2497
#define bd 5.16
#define ad 0.3105
#define ah 64.3

void cargarDatos(double *radioDat , double *velDat);
double likelihood (double *velDat,double *velModel);
void MyModel (double* velModel,double * R, double Mb, double Md, double Mh );
void iniciar (double* myLikelihood,double *velDat,double * R, double*MbWalk,double *MdWalk, double*MhWalk );
double gaussiana (double media);
void constLikelihood(double*myLikelihood,double*velDat,double*R,double*MbWalk,double*MdWalk,double*MhWalk);
int posMayor(double* lista);
void exportarDatos(double* radio, double* velExp, double* velModel);
int main()
{
	double *radioDat, *velDat, *myLikelihood, *MbWalk, *MdWalk, *MhWalk ;
	radioDat = malloc(N*sizeof(double));
	velDat = malloc(N*sizeof(double));
	myLikelihood = malloc(k*sizeof(double));
	MbWalk = malloc(k*sizeof(double));
	MdWalk= malloc(k*sizeof(double));
	MhWalk = malloc(k*sizeof(double));

	cargarDatos(radioDat,velDat);
	iniciar(myLikelihood,velDat, radioDat,MbWalk,MdWalk,MhWalk);
	constLikelihood(myLikelihood,velDat,radioDat,MbWalk,MdWalk,MhWalk);
	
	//Ahora hallamos el valor de los M que máximiza el liklihood:
	int posMax = posMayor(myLikelihood);
	double Mb = MbWalk[posMax];
	double Md = MdWalk[posMax];
	double Mh = MhWalk[posMax];

	printf("%s ", "El valor de las masas Mb, Md, Mh, respectivamnte, en masas galacticas es: ");
	printf("%lf ",Mb);
	printf("%lf " , Md);
	printf("%lf\n ", Mh);

	double* resultado = malloc(N*sizeof(double));
	MyModel(resultado,radioDat,Mb,Md, Mh);
	free(myLikelihood);
	free(MbWalk);
	free(MdWalk);
	free(MhWalk);
	exportarDatos(radioDat,velDat, resultado);
	free(resultado);
	free(radioDat);
	free(velDat);

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

	char line_buffer;
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

//Calcula la funciónde likelihood para los valores del modelo dado, considerando la desviacion como 300,
// de tal forma que se normalize la diferencia y no tienda a cero el el likelihood. Esto no altra el resultado 
//debido a que lo que nos interesa no es el valor del likelihood sino la relación con los m. 
double likelihood (double *velDat,double *velModel)
{
	double valor =0;
	for (int i = 0; i < N; ++i)
	{
		valor += pow((velDat[i] - velModel[i])/300,2);
	}
	
	return exp(-0.5*valor);
}

void MyModel (double* velModel,double* R, double Mb, double Md, double Mh )
{
	for (int i = 0; i < N; ++i)
	{
		double term1 = (pow(Mb,0.5)*R[i])/(pow((pow(R[i],2)+pow(bb,2)),(3.0/4.0)));
		double term2 = (pow(Md,0.5)*R[i])/pow((pow(R[i],2)+pow(bb + ad,2)),3.0/4.0);
		double term3 = (pow(Mh,0.5)*R[i])/pow((pow(R[i],2)+pow(ah,2)),1.0/4.0);
		velModel[i] = term1 + term2 + term3;
	}
}

void iniciar (double* myLikelihood, double *velDat ,double * R, double*MbWalk,double *MdWalk, double*MhWalk )
{
	double M1 = drand48();
	double M2 = drand48();
	double M3 = drand48();
	double velModel [N];
	MyModel(velModel,R, M1,M2,M3);
	myLikelihood[0] = likelihood(velDat,velModel);;
	MbWalk[0] = M1;
	MdWalk[0] = M2;
	MhWalk[0] = M3;	
}

//Genera un numero con una distribución normal de la media dada por paramatro y una desviación estandar
//que se escoge convenientemnte.
double gaussiana (double media)
{
	double z = 0;
	int n = 1000;
	//Usamos el teorem del límite central para generar un muero aletorio con media 0 y desviación estandar 0.
	for (int i = 0; i < n; ++i)
	{
		z += 0.5 - drand48();
	}
	//Desestandarizamos z
	double desv = 0.1 ;
	double med = media;
	double num = z*desv + med;
	
	return num;
}

void constLikelihood(double*myLikelihood,double*velDat,double*R,double*MbWalk,double*MdWalk,double*MhWalk)
{
	double *velTem;
	velTem =  malloc(N*sizeof(double));
	for (int i = 1; i < k; ++i)
	{
		double M1= gaussiana(MbWalk[i-1]);
		double M2= gaussiana(MdWalk[i-1]);
		double M3= gaussiana(MhWalk[i-1]);
		
		MyModel(velTem,R,M1,M2,M3);
		double likelihoodTem = likelihood(velDat, velTem);
		double alpha = myLikelihood[i-1]/likelihoodTem;

		if(alpha >= 1)
		{
			MbWalk[i] = M1;
			MdWalk[i] = M2;
			MhWalk[i] = M3;
			myLikelihood[i] = likelihoodTem;
		}
		else
		{
			double beta = drand48();

			if (beta < alpha)
			{
				MbWalk[i] = M1;
				MdWalk[i] = M2;
				MhWalk[i] = M3;
				myLikelihood[i] = likelihoodTem;	
			}
			else 
			{
				i --;
			}
		}
	}
	free(velTem);
}

int posMayor(double* lista)
{
	double	max =lista[0];
	int pos = 0;
	for (int i = 1; i < k; ++i) 
	{
		if (lista[i] > max)
		{
			max = lista[i];
			pos = i;
		}
	}
	return pos;
}

void exportarDatos(double* radio, double* velExp, double* velModel)
{
	
	FILE *arch;
	arch= fopen("resultados.dat", "w");
	if (!arch)
	{
		printf("Problemas abriendo el archivos %s\n", "resultados.dat" );
		exit(1);
	}

	for (int i = 0; i < N; ++i)
	{
		fprintf(arch, "%lf ", radio[i]);
		fprintf(arch, "%lf ", velExp[i]);
		fprintf(arch, "%lf \n", velModel[i]);
	}

	fclose(arch);
}
