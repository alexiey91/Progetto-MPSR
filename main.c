/**
 * Progetto MPSR.
 * Authors: S. Martucci, A. Valenti
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "rng.c"

// Constant parameters
#define START 0.0
#define F_COMPL_TIME 0.00456    // E[D]_front tempo di servizio (quello che spende nella palla)
#define B_COMPL_TIME 0.00117    // E[D]_back tempo di servizio (quello che spende nella palla)
#define THINK_TIME 7.0          // E[Z]
#define ARRIVAL_TIME 1.0/35.0   // 1/lambda
#define ARRIVAL_RATE 35.0       // lambda --> tasso arrivo sessioni al secondo
#define MAX_SESSION 10000       // massimo numero di sessioni dopo il quale la simulazione si interrompe

// Capire se questi Threshold vanno applicati all' Erlang o alla Iperesponenziale
#define THRESHOLD_MAX 0.85   // sopra 85% utilizzazione => ignoro richieste
#define THRESHOLD_MIN 0.75   // sotto 75% utilizzazione => ricomincio a rispondere
bool exceeded;

//State variables
int sessioni_servite;
double throughput; //si calcola sessioni/tempo totale

//######### Simulation ###############

// Richieste A e B)
void steady_state_analysis(FILE* fp, FILE* graphicsFile) {
/*    int i = 0;
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

        area.PQ += (current_time - prev_time) * qprio;

        curr_utilFS = area.utilFS/current_time;

        event_manager(curr);
        prev_time = current_time;

        if(arrivals && current_time >= STOP) arrivals = 0;  // after final instant, arrivals is inhibited
    }

    // final print
    printState(1);
    printf("\n%sSIMULAZIONE CONCLUSA%s\n\n", GREEN, STD);

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

    printf("Selezionare il SEED da utilizzare:\n");
    printf("1 - 615425336\n");
    printf("2 - 37524306\n");
    printf("3 - 123456789\nSEED? ");
    scanf("%lld", &SEED);
    getchar();
    if(SEED != 1) SEED = (SEED == 2) ? 37524306 : 123456789;
    else SEED = 615425336;
    printf("Si è scelto il SEED: %s%lld%s\n", BOLD, SEED, STD);

    PutSeed(SEED);
    //FIXME implementare funzioni seguenti
//    fp = open_file(sim_type, ".txt", NULL, chosen_phase);
//    graphicsFile = open_file(sim_type, ".xls", nome_file, chosen_phase);
//    fprintf(graphicsFile, "%s\t%ld\n","SOGLIA_AR", MAX_AR);
//    fprintf(graphicsFile, "%s\t%ld\n","SOGLIA_TR", MAX_TR);
//    fprintf(graphicsFile, "%s\t%lld\n","SEED", SEED);
//    fprintf(graphicsFile, "%s\t","STOP");
//    print_header(graphicsFile);

    steady_state_analysis(fp, graphicsFile); //primo tipo
    //aggiungere 10-Erlang e Iperesponenziale
    //confrontare i risultati
    //per il peggiore aggiungere il Threshold

    fclose(fp);
    fclose(graphicsFile);

    return EXIT_SUCCESS;
}
