/**
 *      Progetto MPSR anno 14/15.
 *      Autori: S. Martucci, A. Valenti
 *      Calcolo degli intervalli di confidenza.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rvms.c"

#define ALPHA 0.05

int main() {

    //leggo da file

    double xm = 0.0;
    double v = 0.0;
    double n = 0.0;
    double x;

    while (!feof(stdin)) {
    //for(tutti i dati letti Double x : data) {
        scanf("%lf\n", &x);
        double d = x - xm;
        n++;
        v = v + d*d*(n-1)/n;
        xm = xm + d/n;
    }

    double s = sqrt(v/n);

    //Calcolo il critical value
    double t = idfStudent((int) n-1, 1.0 - (ALPHA/2.0));

    double incr = (t * s) / sqrt((double)(n-1));

    printf("Intervallo di confidenza (1-alpha) = %6.2f\n", (1.0 - ALPHA));
    printf("media ............... = %6.8f\n", xm);
    printf("dev.st. ............. = %6.8f\n", s);
    printf("dim.pop. ............ = %6.8f\n", n);
    printf("varianza ............ = %d\n", (v / n));
    printf("t* .................. = %6.8f\n", t);
    printf("(t* s)/sqrt(n-1) .... = %6.8f\n", incr);
    printf("IC = [ %6.8f ; %6.8f ]\n", (xm - incr), (xm + incr));

    return EXIT_SUCCESS;
}
