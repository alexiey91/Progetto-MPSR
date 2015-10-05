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

double Exponential(double m) {
    return (-m * log(1.0 - Random()));
}

double Erlang(long n, double b) {
    long   i;
    double x = 0.0;
    for (i = 0; i < n; i++)
        x += Exponential(b);
    return (x);
}

double Hyperexponential() {
/*
    double m;
    if(Random() > P_HYP)
        m = FS_COMPL_TIME / (2*(1-P_HYP));
    else
        m = FS_COMPL_TIME / (2*P_HYP);
    return Exponential(m);
*/
    return Exponential(2*P_HYP*FS_COMPL_TIME) + Exponential(2*(1-P_HYP)*FS_COMPL_TIME);
}

long Equilikely(long a, long b) {
    return (a + (long) (Random() * (b - a + 1)));
}

double GetArrival(double prev_time) {
    return prev_time + Exponential(ARRIVAL_TIME);
}

double GetExponentialServiceFS(double prev_time) {
    return prev_time + Exponential(FS_COMPL_TIME);
}

double GetErlangServiceFS(double prev_time) {
    return prev_time + Erlang(K_ERLANG, FS_COMPL_TIME/K_ERLANG);
}

double GetHyperexpServiceFS(double prev_time) {
    return prev_time + Hyperexponential();
}

double GetServiceBES(double prev_time) {
    return prev_time + Exponential(BES_COMPL_TIME);
}

double GetServiceClient(double prev_time) {
    return prev_time + Exponential(THINK_TIME);
}

unsigned int GetRequests() {
    return (unsigned int)Equilikely(MIN_REQ, MAX_REQ);
}
