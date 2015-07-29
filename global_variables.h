/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - Variabili Globali e include
 */

#define START 0.0           // il tempo di inizio
double STOP;                // il tempo a cui finire
int arrivals;
long sessions, requests;
double throughput, current_time, prev_time;
char visual_flag;

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
