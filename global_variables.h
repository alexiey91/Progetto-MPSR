/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - Variabili Globali e include
 */

#define THRESHOLD_MAX       0.85f          // Threshold massimo dopo il quale droppo
#define THRESHOLD_MIN       0.75f          // Se scendo qua sotto riprendo
#define START               0.0f           // il tempo di inizio

double throughput_sessions, throughput_requests, current_time, prev_time,  FS_utilization, FS_average_utilization;
double __throughput_sessions, __throughput_requests, __FS_utilization, __FS_average_utilization, prev_batch_time_completition;
int arrivals, threshold_flag;
long opened_sessions, completed_sessions, requests, dropped, aborted;
long __opened_sessions, __completed_sessions, __requests, __dropped, __aborted;
char visual_flag;

long batch_size;            // numero di job prima di finire un batch
long batch_num;             // numero di batch prima di terminare una simulazione
long current_batch;         // a quale batch sono arrivato

long STOP;                  // a che tempo devo fermare la simulazione
long STEP;                  // ogni quanto prendere i tempi
long CURRENT_STOP;

int SIM_TYPE;               // il tipo di simulazione

#include "simulation_type.h"
#include "event_list.h"
#include "arrival_queue.h"
#include "req_queue.h"
#include "client_req.h"
#include "autocorrelation.h"
#include "interval_calculator.h"

// Definizione del tipo di simulazione
SIMULATION_TYPES type;
// Code
Event *ev_list;
Request* req_queue;           // Request Queue - it stores information regarding Requests amount
ClientReq* client_req_list;   // Matrix with Client completion time and session requests
ArrivalNode* arrival_queue_FS;    // Arrival times queue at FS
ArrivalNode* arrival_queue_BES;   // Arrival times queue at BES
