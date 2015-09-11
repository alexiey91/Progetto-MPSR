/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - Variabili Globali e include
 */

#define THRESHOLD_MAX       0.85f          // Threshold massimo dopo il quale droppo
#define THRESHOLD_MIN       0.75f          // Se scendo qua sotto riprendo
#define START       0.0           // il tempo di inizio
double STOP;                      // il tempo a cui finire
double throughput_sessions, throughput_requests, current_time, prev_time,  FS_utilization, FS_average_utilization;
int arrivals, threshold_flag;
long opened_sessions, completed_sessions, requests, dropped, aborted;
char visual_flag;

double INIT, FIN, STEP;

#include "simulation_type.h"
#include "event_list.h"
#include "arrival_queue.h"
#include "req_queue.h"
#include "client_req.h"

// Definizione del tipo di simulazione
SIMULATION_TYPES type;
// Code
Event *ev_list;
Request* req_queue;           // Request Queue - it stores information regarding Requests amount
ClientReq* client_req_list;   // Matrix with Client completion time and session requests
ArrivalNode* arrival_queue_FS;    // Arrival times queue at FS
ArrivalNode* arrival_queue_BES;   // Arrival times queue at BES
