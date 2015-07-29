/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - main function
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include "ReqQueue.c"
#include "EventList.c"
#include "ClientOrderList.c"
#include "ArrivalQueue.c"
#include "Utils.c"
#include "rng.c"

// Constant parameters
#define START 0.0
#define FS_COMPL_TIME 0.00456    // E[D]_front tempo di servizio (quello che spende nella palla)
#define BES_COMPL_TIME 0.00117    // E[D]_back tempo di servizio (quello che spende nella palla)
#define THINK_TIME 7.0          // E[Z]
#define ARRIVAL_TIME 1.0/35.0   // 1/lambda
#define MAX_SESSION 10000       // massimo numero di sessioni dopo il quale la simulazione si interrompe
#define MIN_REQ 5.0
#define MAX_REQ 35.0

// Capire se questi Threshold vanno applicati all' Erlang o alla Iperesponenziale
#define THRESHOLD_MAX 0.85   // sopra 85% utilizzazione => ignoro richieste
#define THRESHOLD_MIN 0.75   // sotto 75% utilizzazione => ricomincio a rispondere
//bool exceeded;

//########## Pseudo-Random number generation functions ################

double Exponential(double m)              { return (-m * log(1.0 - Random()));  }

long Equilikely(long a, long b)           { return (a + (long) (Random() * (b - a + 1))); }

double GetArrival(double prev_time)       { return prev_time + Exponential(ARRIVAL_TIME); }

double GetServiceFS(double prev_time)     { return prev_time + Exponential(FS_COMPL_TIME);  }

double GetServiceBES(double prev_time)    { return prev_time + Exponential(BES_COMPL_TIME);  }

double GetServiceClient(double prev_time) { return prev_time + Exponential(THINK_TIME);  }

unsigned int GetRequests()                { return (unsigned int)Equilikely(MIN_REQ, MAX_REQ); }

//State variables
long sessions = 0, requests = 0;
double throughput; //si calcola sessioni/tempo totale
double STOP, current_time;
SIMULATION_TYPES sim_type;
int xfs, xbe, qfs, qbe, arrivals = 0;
long count_BES, count_FS;       // Welford's counters
double curr_utilFS, prev_time;
double av_resFS, av_resBES; // Average residence times

//########## Simulator data structures ##########
Event* event_list;            // Event List
Request* req_queue;           // Request Queue - it stores information regarding Requests amount
ClientReq* client_req_list;   // Matrix with Client completion time and session requests

ArrivalNode* arrival_queue_FS;    // Arrival times queue at FS
ArrivalNode* arrival_queue_BES;   // Arrival times queue at BES

//########### Struct containing data concerning integrals ###########
struct area {
    double centerFS;
    double queueFS;
    double utilFS;

    double centerBES;
    double queueBES;
    double utilBES;
};
typedef struct area _area;
_area area;

void reset_area(struct area* a) {
    if(a == NULL) {
            printf("Area is NULL.\n");
            return;
    }
    a->centerFS = 0.0;
    a->centerBES = 0.0;

    a->queueFS = 0.0;
    a->queueBES = 0.0;

    a->utilFS = 0.0;
    a->utilBES = 0.0;
}

// NewSession Management
void NewSession(Event* ev) {

    if(arrivals) {
        double new_time; // compute new session arrival time
        new_time = GetArrival(ev->time);

        if(new_time < STOP) {
            add_event(&event_list, new_time, NEW_SESSION);   // create NewSession event and schedule it
        }
    }

    enqueue_req(&req_queue, GetRequests());

    arrival_add(&arrival_queue_FS, ev->time);
    if(xfs) qfs++;     // if server is busy, add 1 to its queue
    else {
        xfs = 1;
        add_event(&event_list, GetServiceFS(ev->time), FS_COMPL); // create FS_COMPL event and schedule it
    }
}

// FS Completion Management
void FS_Completion(Event* ev) {

    // calculate average residence time in FS center. (Welford + destructive pop)
    count_FS++;
    double res_time_FS = ev->time - arrival_pop(&arrival_queue_FS); // i-th session's residence time
    av_resFS = av_resFS + (res_time_FS - av_resFS)/count_FS;

    arrival_add(&arrival_queue_BES, ev->time); // save entrance time into BE center

    // Exiting from FS
    if(qfs > 0) {
        qfs--;
        add_event(&event_list, GetServiceFS(ev->time), FS_COMPL);
    }
    else xfs = 0;

    // Entering in BES
    if(xbe) qbe++;
    else {
        xbe = 1;
        add_event(&event_list, GetServiceBES(ev->time), BES_COMPL);
    }
}

// BES Completion Management
void BES_Completion(Event* ev) {

    // calculate average residence time in BES center. (Welford + destructive pop)
    count_BES++;
    double res_time_BES = ev->time - arrival_pop(&arrival_queue_BES); // i-th session's residence time
    av_resBES = av_resBES + (res_time_BES - av_resBES)/count_BES;

    // Exiting from BES
    if(qbe > 0) {
        qbe--;
        add_event(&event_list, GetServiceBES(ev->time), BES_COMPL);
    }
    else xbe = 0;

    //Entering Client
    unsigned int this_requests = dequeue_req(&req_queue);
    if(this_requests <= 0) {
        // session is over. It will move out of the system
        sessions++;
    }
    else {
        // This session still has some requests to be executed. Let's go to the Clients
        requests++;
        //count_CLIENT++;
        //xc++; // increase the number of active clients

        // Calculate average residence time in Clients center
        double temp = GetServiceClient(ev->time);

        double time_res_client = temp - ev->time;
        add_event(&event_list, temp, CL_COMPL);

        //av_resCLIENT = av_resCLIENT + (time_res_client - av_resCLIENT)/count_CLIENT;
        //printf("[T]\nav_res_CLIENT = %6.4f\nev->time = %6.4f\ntemp = %6.4f, i-esima permanenza: %6.4f\n\n", av_resCLIENT, ev->time, temp, time_res_client);

        // Add this time and this session's remaining requests into ClientOrderList
        add_client_req(&client_req_list, temp, this_requests-1);
    }
    //printf("[T] %sto do PQ_MANAGEMENT%s\n", RED, STD);
    //PQ_management(ev->time);
}

// Client Completion Management
void Client_Completion(Event* ev) {
    //xc--; // decrease number of Clients
//    arrival_add(&arrival_queue_PQ, ev->time);

//    count_PQ++;
//    double res_time_prio = ev->time-arrival_pop(&arrival_queue_PQ);
//    av_resPQ = av_resPQ + (res_time_prio - av_resPQ)/count_PQ;

    // If SBAC hasn't triggered, PriorityQueue must be empty and this session goes directly from Clients to FS

    // Insert the updated value of remaining requests (from THIS session) into the ReqQueue
    ClientReq* coming_back_session = pop_ClientReq(&client_req_list);
    enqueue_req(&req_queue, coming_back_session->req);
    free(coming_back_session);

    if(xfs == 0) {
        xfs = 1;
        add_event(&event_list, GetServiceFS(ev->time), FS_COMPL);
    }
    else qfs++;
    arrival_add(&arrival_queue_FS, ev->time);

}

void event_manager(Event* curr) {
    switch(curr->type) {
        case NEW_SESSION:
            NewSession(curr);
            break;
        case FS_COMPL:
            FS_Completion(curr);
            break;
        case BES_COMPL:
            BES_Completion(curr);
            break;
        case CL_COMPL:
            Client_Completion(curr);
            break;
        default:
            printf("Wrong event!\n");
    }
    free(curr);
}

// Initializating function called between run
void initialization() {
    xfs              =  0;
    xbe              =  0;
    qfs              =  0;
    qbe              =  0;
//    dropped          =  0;
//    aborted          =  0;
//FIXME da vedere
    sessions         =  0;
    requests         =  0;
    throughput       = 0.0;

    event_list        = NULL;
    arrival_queue_FS  = NULL;
    arrival_queue_BES = NULL;
    req_queue         = NULL;
    client_req_list   = NULL;

    current_time  = START;
    prev_time     = START;

    av_resFS     = 0.0;
    av_resBES    = 0.0;
    //av_resCLIENT = 0.0;

    count_FS            = 0;
    count_BES           = 0;
    //count_CLIENT        = 0;
    arrivals = 1;
    add_event(&event_list, GetArrival(START), NEW_SESSION);
}

//######### Simulation ###############

void steady_state_analysis(FILE* fp, FILE* graphicsFile) {
    initialization();
    reset_area(&area);

    while(event_list != NULL) {
        Event* curr = pop_event(&event_list);
        current_time = curr->time; // Next-Event Time Advance

        // integral increasing for averages calculi
        area.centerFS += (current_time - prev_time) * (xfs+qfs);
        area.queueFS += (current_time - prev_time) * (qfs);
        area.utilFS += (current_time - prev_time) * (xfs);

        area.centerBES += (current_time - prev_time) * (xbe+qbe);
        area.queueBES += (current_time - prev_time) * (qbe);
        area.utilBES += (current_time - prev_time) * (xbe);

        curr_utilFS = area.utilFS/current_time;

        event_manager(curr);
        prev_time = current_time;
        if(arrivals && current_time >= STOP) arrivals = 0;
    }

    throughput = sessions/current_time;
    // final print
    printf("\nSIMULAZIONE CONCLUSA\n\n");
    printf("Throughput = sessioni / tempo totale : %f = %ld / %f \n", throughput, sessions, current_time);
/*
    // results file serialization
    fprintf(fp, "STOP..........................: %6.4f\n", STOP);
    if(variable_arrival_rate_toggle) {
        fprintf(fp, "Secondi in alta frequenza.....: %6.4f\n", high_rate);
        fprintf(fp, "Secondi in bassa frequenza....: %6.4f\n", STOP-high_rate);
    }
    fprintf(fp, "Ultimo tempo..................: %6.4f\n", current_time);
    fprintf(fp, "\n");
    fprintf(fp, "SBAC Active Request...........: %s\n", (SBAC_AR) ? "Attivo" : "Non attivo");
    fprintf(fp, "SBAC Threshold................: %s\n", (SBAC_TR) ? "Attivo" : "Non attivo");
    fprintf(fp, "Soglia AR.....................: %ld\n", MAX_AR);
    fprintf(fp, "Soglia TR.....................: %ld\n", MAX_TR);
    fprintf(fp, "Sessioni......................: %ld\n", sessions);
    fprintf(fp, "Richieste.....................: %ld\n", requests);
    fprintf(fp, "Richieste/Sessione............: %6.4f\n", (double)requests/sessions);
    fprintf(fp, "dropped.......................: %ld\n", dropped);
    fprintf(fp, "%% dropped.....................: %6.4f%%\n", ((double)dropped/(dropped+sessions))*100.0);
    fprintf(fp, "\n");
    fprintf(fp, "PopolazioneMediaFS............: %6.4f\n", area.centerFS/current_time);
    fprintf(fp, "PopolazioneMediaBES...........: %6.4f\n", area.centerBES/current_time);
    fprintf(fp, "CodaFS........................: %6.4f\n", area.queueFS/current_time);
    fprintf(fp, "CodaBES.......................: %6.4f\n", area.queueBES/current_time);
    fprintf(fp, "UtilizzazioneFS...............: %6.4f%%\n", (area.utilFS/current_time)*100.0);
    fprintf(fp, "UtilizzazioneBES..............: %6.4f%%\n", (area.utilBES/current_time)*100.0);
    fprintf(fp, "PopolazioneMediaPQ............: %6.4f\n", area.PQ/current_time);
    fprintf(fp, "\n");
    fprintf(fp, "Throughput (sessioni).........: %6.4f sessioni per unità di tempo\n", ((double)sessions)/(current_time-START));
    fprintf(fp, "Throughput (richieste)........: %6.4f richieste per unità di tempo\n", ((double)requests)/(current_time-START));
    fprintf(fp, "TempoMedioResidenzaFS.........: %6.4f\n", av_resFS);
    fprintf(fp, "TempoMedioResidenzaBES........: %6.4f\n", av_resBES);
    fprintf(fp, "TempoMedioResidenzaCLIENT.....: %6.4f\n", av_resCLIENT);
    fprintf(fp, "TempoMedioResidenzaPQ.........: %6.4f\n", av_resPQ);
    fprintf(fp, "TempoMedio Risposta /richiesta: %6.4f\n", (av_resFS+av_resBES+av_resPQ));
    fprintf(fp, "***************************************************************************\n");

    // Excel file serialization in order to plot graphics
    fprintf(graphicsFile, "%6.4f\t",STOP);
    fprintf(graphicsFile, "%6.4f\t",area.centerFS/current_time);
    fprintf(graphicsFile, "%6.4f\t",area.centerBES/current_time);
    fprintf(graphicsFile, "%6.4f\t",(area.utilFS/current_time)*100.0);
    fprintf(graphicsFile, "%6.4f\t",(area.utilBES/current_time)*100.0);
    fprintf(graphicsFile, "%6.4f\t",av_resFS);
    fprintf(graphicsFile, "%6.4f\t",av_resBES);
    fprintf(graphicsFile, "%6.4f\t",((double)sessions)/(current_time-START));
    fprintf(graphicsFile, "%6.4f\t",((double)requests)/(current_time-START));
    fprintf(graphicsFile, "%6.4f\t",(av_resFS+av_resBES+av_resPQ));
    fprintf(graphicsFile, "%ld\t", sessions);
    fprintf(graphicsFile, "%ld\t", dropped);
    fprintf(graphicsFile, "%6.4f\t",((double)dropped/(dropped+sessions))*100.0);
    fprintf(graphicsFile, "%ld\t", aborted);
    fprintf(graphicsFile, "%6.4f\t",((double)aborted/sessions)*100.0);
    fprintf(graphicsFile, "%6.4f\t", (double)requests/sessions);
    fprintf(graphicsFile, "\n");
*/
    return;
}

int main(int argc, char* argv[]) {

    FILE *fp, *graphicsFile;
    int choice, i, num_run;
    long long int SEED = 0;
    double init_f, fin_f, passo_f;
    char nome_file[50];

/**
 * Vengono aperte nuove sessioni con una distribuzione esponenziale di 35 sessioni/secondo
 * Da ogni sessione (che viene messa in una coda) prendo un numero di  richieste
 * distribuito Equilikely (5,35)
 * Prendo la richiesta dalla coda e la comincio ad elaborare -->
 * passo nel front end poi nel backend.
 * Se non è l'ultima richiesta di sessione aspetto il think time medio e metto in coda la richiesta nell frontend
 * Se è l'ultima richiesta esco.
 */

    if(argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("Inserire il tipo di analisi stazionaria.\n 1. Frontend esponenziale\n 2. Frontend 10-Erlang \n 3. Frontend iperesponenziale\n");
    printf("Scelta: ");
    scanf("%d",&choice);
    getchar();
    printf("----------------------------------------------------------------------------------------\n");

    switch(choice) {
        case 1:
            sim_type = FE_EXP;
            break;
        case 2:
            sim_type = FE_ERL;
            break;
        case 3:
            sim_type = FE_HYP;
            break;
        default:
            printf("Errore. Nessuna distribuzione per il frontend selezionata.\n");
            return EXIT_FAILURE;
    }

    printf("Hai scelto \"Analisi #%d\".\n", choice);

    printf("Selezionare il SEED da utilizzare:\n");
    printf("1 - 615425336\n");
    printf("2 - 37524306\n");
    printf("3 - 123456789\nSEED? ");
    scanf("%lld", &SEED);
    getchar();
    if(SEED != 1) SEED = (SEED == 2) ? 37524306 : 123456789;
    else SEED = 615425336;
    printf("Si è scelto il SEED: %lld\n", SEED);

    PutSeed(SEED);

    printf("Selezionare i parametri della simulazione: STOP_iniziale STOP_finale Numero_di_esecuzioni\nParametri: ");
    scanf("%lf %lf %d", &init_f, &fin_f, &num_run);
    getchar();
    printf("I parametri sono: %4.2f %4.2f %d\n", init_f, fin_f, num_run);
    passo_f = (fin_f-init_f)/num_run;
    printf("Il passo_f sarà: %4.2f\n", passo_f);

    //FIXME implementare funzioni seguenti
    fp = open_file(sim_type, ".txt", NULL);
    graphicsFile = open_file(sim_type, ".xls", nome_file);
//    fprintf(graphicsFile, "%s\t%ld\n","SOGLIA_TR", MAX_TR);
//    questo Threshold va impostato per il peggiore tra i due
    fprintf(graphicsFile, "%s\t%lld\n","SEED", SEED);
    fprintf(graphicsFile, "%s\t","STOP");
    print_header(graphicsFile);

    for(i=init_f; i<=fin_f; i+=passo_f) {
        STOP = i;
        steady_state_analysis(fp, graphicsFile);
        if(init_f == fin_f) break;
    }

    fclose(fp);
    fclose(graphicsFile);

    return EXIT_SUCCESS;
}
