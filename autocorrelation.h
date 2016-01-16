/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - Calcolo Autocorrelazione
 */

#define K_CORR 20
#define SIZE_CORR (K_CORR + 1)

static double sum_corr = 0.0;
static double hold_corr[SIZE_CORR];
static double cosum_corr[SIZE_CORR] = {0.0};
static double mean_corr = 0.0;
static long p_corr = 0, j_corr = 0;

void set_autocorr_data(long point, double average) {
    sum_corr += average;
    if(point < SIZE_CORR) {
        hold_corr[point] = average;
    }
    else {
        for (j_corr = 0; j_corr < SIZE_CORR; j_corr++) {
            cosum_corr[j_corr] += hold_corr[p_corr] * hold_corr[(p_corr + j_corr) % SIZE_CORR];
        }
        hold_corr[p_corr] = average;
        p_corr = (p_corr + 1) % SIZE_CORR;
    }
}

void compute_autocorr() {
    int i = 0;
    while (i < SIZE_CORR) {         /* empty the circular array */
        for (j_corr = 0; j_corr < SIZE_CORR; j_corr++) {
            cosum_corr[j_corr] += hold_corr[p_corr] * hold_corr[(p_corr + j_corr) % SIZE_CORR];
        }
        hold_corr[p_corr] = 0.0;
        p_corr = (p_corr + 1) % SIZE_CORR;
        i++;
    }
    mean_corr = sum_corr / (batch_num-1);
    for (j_corr = 0; j_corr <= K_CORR; j_corr++) {
        cosum_corr[j_corr] = (cosum_corr[j_corr] / (batch_num - 1 - j_corr)) - (mean_corr * mean_corr);
    }
}

void print_autocorr_on_file(FILE *g) {
    fprintf(g, "\n");
    fprintf(g, "Autocorrelation values:\nJ\tVALUE\n");
    for(j_corr = 1; j_corr < SIZE_CORR; j_corr++) {
        fprintf(g, "%3ld\t%6.8f\n", j_corr, cosum_corr[j_corr] / cosum_corr[0]);
    }
}
