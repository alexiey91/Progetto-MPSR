/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - genera i valori random per arrivi e servizi
 */

#define MIN_REQ 5.0
#define MAX_REQ 35.0
#define ARRIVAL_TIME 1.0/35.0     // 1/lambda
#define THINK_TIME 7.0            // E[Z]
#define FS_COMPL_TIME 0.00456     // E[D]_front tempo di servizio (quello che spende nella palla)
#define BES_COMPL_TIME 0.00117    // E[D]_back tempo di servizio (quello che spende nella palla)
#define K_ERLANG 10               // Parametro per la distribuzione della 10 Erlang
#define P_HYP 0.1                 // Parametro per la sitribuzione iperesponenziale

double Exponentialm(double m) {
    SelectStream(50);
    return (-m * log(1.0 - Random()));
}

double Erlang(long n, double b) {
    SelectStream(2);
    long   i;
    double x = 0.0;
    for (i = 0; i < n; i++)
        x += Exponentialm(b);
    return (x);
}

double Hyperexponential(double p[], double m[], long i) {
    SelectStream(3);
    long j;
    double result = 0.0;
    for(j=0; j<i; j++) {
        result += p[i] * Exponentialm(m[i]);
    }
    return result;
}

long Equilikely(long a, long b) {
    SelectStream(4);
    return (a + (long) (Random() * (b - a + 1)));
}

double GetArrival(double prev_time) {

    return prev_time + Exponentialm(ARRIVAL_TIME);
}

double GetExponentialServiceFS(double prev_time) {

    return prev_time + Exponentialm(FS_COMPL_TIME);
}

double GetErlangServiceFS(double prev_time) {

    return prev_time + Erlang(K_ERLANG, FS_COMPL_TIME/K_ERLANG);
}

double GetHyperexpServiceFS(double prev_time) {

    long i = 2;
    double p[i], m[i];
    p[0] = P_HYP; p[1] = 1-P_HYP;
    m[0] = FS_COMPL_TIME / (2*p[0]);
    m[1] = FS_COMPL_TIME / (2*p[1]);
    return prev_time + Hyperexponential(p, m, i);
}

double GetServiceBES(double prev_time) {

    return prev_time + Exponentialm(BES_COMPL_TIME);
}

double GetServiceClient(double prev_time) {

    return prev_time + Exponentialm(THINK_TIME);
}

unsigned int GetRequests() {

    return (unsigned int)Equilikely(MIN_REQ, MAX_REQ);
}
