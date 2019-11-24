#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <complex.h>

void fft(int index, int step);

int N;
int numThreads;
double complex *v_in;
double complex *v_out;
double complex *twiddle;

typedef struct parameters {
	int index;
	int step;
} Params;

void* threadFunction(void *var)
{
	Params params = *(Params*)var;
	fft(params.index, params.step);
	return NULL;
}

void fft(int index, int step) // index = indexul de start in vectori
{
	if (step >= N)
		return;

	int i;
	if (step < numThreads) // creez 2 thread-uri
	{
		pthread_t tid[2];
		Params params[2];
		params[0].index = index;
		params[0].step = 2 * step;
		params[1].index = index + step;
		params[1].step = 2 * step;

		for(i = 0; i < 2; i++) 
			pthread_create(&(tid[i]), NULL, threadFunction, &(params[i]));

		for(i = 0; i < 2; i++) 
			pthread_join(tid[i], NULL);
	}
	else // apelez recursiv, secvential
	{
		fft(index, step * 2);
		fft(index + step, step * 2);
	}

	if ((int)log2(step) % 2 == 0) // se interschimba v_in si v_out
	{
		for (i = 0; i < N; i += 2 * step) {
			double complex t = twiddle[i] * v_out[index + i + step];
			v_in[index + i / 2] = v_out[index + i] + t;
			v_in[index + (i + N) / 2] = v_out[index + i] - t;
		}
	}
	else 
	{
		for (i = 0; i < N; i += 2 * step) {
			double complex t = twiddle[i] * v_in[index + i + step];
			v_out[index + i / 2] = v_in[index + i] + t;
			v_out[index + (i + N) / 2] = v_in[index + i] - t;
		}
	}
}

void take_input(int argc, char **argv) 
{
	FILE *fin;
	int i;

	fin = fopen(argv[1], "r");
	if (!fscanf(fin, "%d", &N)) {
		printf("Failed to read from file.\n");
		exit(1);
	}

	v_in = (double complex*) calloc(N, sizeof(double complex));
	v_out = (double complex*) calloc(N, sizeof(double complex));

	double aux;
	for (i = 0; i < N; i++) {
		if (!fscanf(fin, "%lf", &aux)) {
			printf("Failed to read from file.\n");
			exit(1);
		}
		v_in[i] = (double complex)aux;
		v_out[i] = v_in[i];
	}
	fclose(fin);

	numThreads = atoi(argv[3]);
	
	twiddle = (double complex*) calloc(N, sizeof(double complex));
	for (i = 0; i < N; i++) 
		twiddle[i] = cexp(-I * M_PI * i / N);
}

void write_output(int argc, char **argv) 
{
	FILE *fout;
	int i;

	fout = fopen(argv[2], "w");
	fprintf(fout, "%d\n", N);
	for (i = 0; i < N; i++) 
		fprintf(fout, "%lf %lf\n", creal(v_in[i]), cimag(v_in[i]));
	fclose(fout);
}

int main(int argc, char **argv)
{
	take_input(argc, argv);

	fft(0, 1);

	write_output(argc, argv);	

	return 0;
}
