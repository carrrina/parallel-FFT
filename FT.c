#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <complex.h>

int N;
int numThreads;
double *v_in;
double complex *v_out;
double complex *twiddle;

void* threadFunction(void *var)
{
	int thread_id = *(int*)var;
	int i, k, first, last, tmp;

	first = thread_id * ceil((double)N / (double)numThreads);
	tmp = (thread_id + 1) * ceil((double)N / (double)numThreads);
	if (tmp < N)
		last = tmp;
	else
		last = N;

	for (k = first; k < last; k++) {
		for (i = 0; i < N; i++) {
			v_out[k] += v_in[i] * cexp(twiddle[i] * k);
		}
	}
	return NULL;
}

int main(int argc, char **argv)
{
	FILE *fin, *fout;
	int i;

	fin = fopen(argv[1], "r");
	if (!fscanf(fin, "%d", &N)) {
		printf("Failed to read from file.\n");
		exit(1);
	}
	v_in = (double*) calloc(N, sizeof(double));
	for (i = 0; i < N; i++) {
		if (!fscanf(fin, "%lf", &v_in[i])) {
			printf("Failed to read from file.\n");
			exit(1);
		}
	}
	fclose(fin);
	numThreads = atoi(argv[3]);
	v_out = (double complex*) calloc(N, sizeof(double complex));
	twiddle = (double complex*) calloc(N, sizeof(double complex));
	for (i = 0; i < N; i++) 
		twiddle[i] = -2 * M_PI * I / N * i;

	pthread_t tid[numThreads];
	int thread_id[numThreads];
	for(i = 0; i < numThreads; i++)
		thread_id[i] = i;

	for(i = 0; i < numThreads; i++) 
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));

	for(i = 0; i < numThreads; i++) 
		pthread_join(tid[i], NULL);

	fout = fopen(argv[2], "w");
	fprintf(fout, "%d\n", N);
	for (i = 0; i < N; i++) 
		fprintf(fout, "%lf %lf\n", creal(v_out[i]), cimag(v_out[i]));
	fclose(fout);

	return 0;
}
