/**
 *      Progetto MPSR anno 14/15.
 *      Autori: S. Martucci, A. Valenti
 *      Calcolo dell'autocorrelazione.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define K    20                             /* K is the maximum lag */
#define SIZE (K + 1)

int main(int argc, char *argv[]) {

    long   i = 0;                   /* data point index              */
    double x;                       /* current x[i] data point       */
    double sum = 0.0;               /* sums x[i]                     */
    long   n;                       /* number of data points         */
    long   j;                       /* lag index                     */
    double hold[SIZE];              /* K + 1 most recent data points */
    long   p = 0;                   /* points to the head of 'hold'  */
    double cosum[SIZE] = {0.0};     /* cosum[j] sums x[i] * x[i+j]   */
    double mean;

    char *new_file = "autocorrelation.xls";
    FILE *out = fopen(new_file, "w");

    while (i < SIZE) {              /* initialize the hold array with */
        scanf("%lf\n", &x);           /* the first K + 1 data values    */
	printf("%lf\n", x);
        sum     += x;
        hold[i]  = x;
        i++;
    }

    while (!feof(stdin)) {
        for (j = 0; j < SIZE; j++) {
            cosum[j] += hold[p] * hold[(p + j) % SIZE];
        }
        scanf("%lf\n", &x);
        sum    += x;
        hold[p] = x;
        p       = (p + 1) % SIZE;
        i++;
    }
    n = i;

    while (i < n + SIZE) {         /* empty the circular array */
        for (j = 0; j < SIZE; j++) {
            cosum[j] += hold[p] * hold[(p + j) % SIZE];
        }
        hold[p] = 0.0;
        p = (p + 1) % SIZE;
        i++;
    }

    mean = sum / n;
    for (j = 0; j <= K; j++) {
        cosum[j] = (cosum[j] / (n - j)) - (mean * mean);
    }
    fprintf(out, "%s\t%s\t\n\n", "J", "VALUE" );
    for (j = 1; j < SIZE; j++) {
        printf("%3ld\t%6.8f\n", j, cosum[j] / cosum[0]);
        fprintf(out, "%3ld\t%6.8f\n", j, cosum[j] / cosum[0]);
    }
    fclose(out);
    return (0);
}
