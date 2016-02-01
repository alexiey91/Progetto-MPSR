/**
    Progetto MPSR
    Test degli estremi
**/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "rngs.h"
#include "rvms.h"

#define SEED 48271
#define N 100000L
//#define K 5000L
#define K N/20
#define ALPHA 0.05f
#define D 7

#define square(x) (x)*(x)

FILE *openFileForWrite(char *filePath) {
	if(filePath == NULL)
		return NULL;
	return fopen(filePath, "w");
}

void closeFile(FILE *f) {
	if(f != NULL) {
		fclose(f);
	}
}

void writeDataToFile(FILE *f, double v, double v1, double v2, long stream ) {
	if(f == NULL)
		return;
	if(fprintf(f, "%ld;%.6f;%.6f;%.6f\n", stream, v, v1, v2) < 0) {
		fprintf(stderr, "Failed to write data to file\n");
	}
}

int checkIfTestFailed(double v, double v1, double v2) {
	return v < v1 || v > v2;
}

int main(int argc, char **argv) {
	char *filename = "result.csv";
	long failed = 0L, passed = 0L;
	if(argc > 1) {
		filename = argv[1]; //nome del file
	}

	FILE *dataOut = openFileForWrite(filename);
	if(dataOut == NULL) {
		perror("Data out open");
		exit(EXIT_FAILURE);
	}

	double v1_star = idfChisquare(K - 1, ALPHA/2.0);
	double v2_star = idfChisquare(K - 1, 1.0 - (ALPHA/2.0));

	long stream = 0;
	for(; stream < 256L; stream++) {

		//inizializzazione
		long o[K];

		double v = 0.0;
		double e_x = (double) N / (double) (K);

		//imposta tutti gli intervalli dell'istogramma a 0
		memset(o, 0, K * sizeof(long));

		PlantSeeds(SEED);
		SelectStream(stream);

		//popolo il campione
		long i = 0;
		for(; i < N; i++) {
			double r = Random();
            double u;
            int j;
            for(j = 1; j < D; j++) {
                u = Random();
                if(u > r) r = u;
            }
            u = exp(D * log(r));
			long x = u * K;
			o[x] ++;
		}

		//calcolo la statistica del test
		for(i = 0; i < K; i++) {
			v += (square(o[i] - e_x));
		}
		v /= e_x;

		writeDataToFile(dataOut, v, v1_star, v2_star, stream);

		//risultati completi
		printf("===Uniformity Test (X~U(0,1))===\n");
		printf("stream ........... = %ld\n", stream);
		printf("seed ............. = %ld\n", SEED);
		printf("alpha ............ = %f\n", ALPHA);
		printf("n ................ = %ld\n", N);
		printf("k bins ........... = %ld\n", K);
		printf("n/k .............. = %f\n", e_x);
		printf("v1* .............. = %f\n", v1_star);
		printf("v2* .............. = %f\n", v2_star);
		printf("v ................ = %f\n", v);
		printf("result ........... = ");

		if(checkIfTestFailed(v, v1_star, v2_star)) {
			printf("FAILED\n");
			failed++;
		} else {
			printf("PASSED\n");
			passed++;
		}
	}

	printf("Number of failed: %ld\n", failed);
	printf("Number of passed: %ld\n", passed);
	printf("Total number of tests: %ld\n", failed+passed);

	closeFile(dataOut);
	return 0;
}
