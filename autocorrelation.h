/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - Calcolo dell'autocorrelazione
 */

#define N_AUTOCORR  20                       // numero valori autocorrelazione
#define SIZE_CORR   21                       // autocorrelazione + 1

double cosum[SIZE_CORR] = {0.0};             /* cosum[j] sums x[i] * x[i+j]   */
double autocorr_value[N_AUTOCORR] = {0.0};
double hold[SIZE_CORR] = {0.0};                      /* N_AUTOCORR +1 most recent data points */
long p_FS = 0, p_BES = 0, autocorr_counter_FS = 0, autocorr_counter_BES = 0;    /* points to the head of 'hold'  */
double sum_autocorr = 0.0;                   /* sums x[i]                     */

void reset_correlation() {
    int i;
    for (i = 0; i < N_AUTOCORR; ++i) {
        cosum[i] = 0.0;
        autocorr_value[i] = 0.0;
        hold[i] = 0.0;
    }
    cosum[i+1] = hold[i+1] = 0.0;
    p_FS = 0;
    p_BES = 0;
    autocorr_counter_FS = 0;
    autocorr_counter_BES = 0;    /* points to the head of 'hold'  */
    sum_autocorr = 0.0;
}

void compute_autocorrelation() {
    if(autocorr_counter_FS != autocorr_counter_BES || p_FS != p_BES) {
        printf("MESA CHE HAI TOPPATO!\n");
        return;
    }
    long n = autocorr_counter_FS;
    int j;
    while (autocorr_counter_FS < n + SIZE_CORR) {         /* empty the circular array */
      for (j = 0; j < SIZE_CORR; j++)
        cosum[j] += hold[p_FS] * hold[(p_FS + j) % SIZE_CORR];
      hold[p_FS] = 0.0;
      p_FS = (p_FS + 1) % SIZE_CORR;
      autocorr_counter_FS++;
    }
    double mean = sum_autocorr/n;
    for (j = 0; j <= N_AUTOCORR; j++)
      cosum[j] = (cosum[j] / (n - j)) - (mean * mean);
    for (j = 1; j < SIZE_CORR; j++)
        autocorr_value[j-1] =  cosum[j] / cosum[0];
        printf("%ld\n", n);
    for(j = 0; j<N_AUTOCORR; j++) {
           // printf("%d", j);
        printf("%6.8f\n", autocorr_value[j]);
    }
}
