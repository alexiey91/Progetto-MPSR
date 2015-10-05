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

#include "rng.c"
#include "rvms.c"
#include "global_variables.h"
#include "file_manager.c"
#include "generate_random_value.h"
#include "event_manager.c"
#include "utils.h"
#include "user_signal.c"

void initialize() {
    busy_FS = 0;
    busy_BES = 0;
    FS_counter = 0;
    BES_counter = 0;
    client_counter = 0;
    queue_length_FS = 0;
    queue_length_BES = 0;
    active_client = 0;
    average_res_FS = 0.0;
    average_res_BES = 0.0;
    average_res_client = 0.0;
    FS_utilization = 0.0;
    FS_average_utilization = 0.0;
    completed_sessions = 0;
    opened_sessions = 0;
    requests = 0;
    dropped = 0;
    aborted = 0;
    threshold_exceeded = 0;
    throughput_sessions = 0.0;
    throughput_requests = 0.0;
    arrivals = 1;
    current_time = START;
    prev_time = START;
    ev_list = NULL;
    add_event(&ev_list, GetArrival(START), NEW_SESSION);
    prev_batch_time_completition = START;
    __throughput_sessions = 0.0;
    __throughput_requests = 0.0;
    __FS_utilization = 0.0;
    __FS_average_utilization = 0.0;
    __opened_sessions = 0;
    __completed_sessions = 0;
    __requests = 0;
    __dropped = 0;
    __aborted = 0;
}

void reset() {
    average_res_FS = 0.0;
    average_res_BES = 0.0;
    average_res_client = 0.0;
    FS_utilization = 0.0;
    FS_average_utilization = 0.0;
    completed_sessions = 0;
    opened_sessions = 0;
    requests = 0;
    dropped = 0;
    aborted = 0;
    throughput_sessions = 0.0;
    throughput_requests = 0.0;
    prev_batch_time_completition = current_time;
}

void compute_statistics() {
    __throughput_sessions += throughput_sessions;
    __throughput_requests += throughput_requests;
    __FS_utilization += FS_utilization;
    __FS_average_utilization += FS_average_utilization;
    __opened_sessions += opened_sessions;
    __completed_sessions += completed_sessions;
    __requests += requests;
    __dropped += dropped;
    __aborted += aborted;
}

void begin_simulation(FILE *graphic) {

    int i = 0;
    initialize();
    for(current_batch = 1L; current_batch <= batch_num; current_batch++) {
        reset();
        // fare qualcosa
        while(completed_sessions < batch_size) {
            // fare tutto
            Event *current = pop_event(&ev_list);
            current_time = current->time;
            FS_utilization += (current_time - prev_time) * (busy_FS);
            FS_average_utilization = FS_utilization / (current_time-prev_batch_time_completition);
            throughput_requests = ((double) requests)/ (current_time-prev_batch_time_completition);
            throughput_sessions = ((double) completed_sessions)/(current_time-prev_batch_time_completition);

            manage_event(current);
            prev_time = current_time;

            if(visual_flag == 'Y' || visual_flag == 'y') {
                if(i%300 == 0) {
                    clear_console();
                    // Stampo
                    print_system_state(current->type);
                    i=0;
                    usleep(10000);
                }
                i++;
            }
        }
        print_system_state_on_file(graphic);
        printf("\nSimulation completed! (Batch completed: %ld)\n", current_batch);
        compute_statistics();
    }
    // stampa stato finale
    print_final_state(graphic);
}

int main (int argc, char *argv[]) {

    int choice;
    long long int SEED = 0;
    char t_flag;
    FILE *graphic;

    if(argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return EXIT_FAILURE;
    }
    add_signals();

    clear_console();
    printf("\nMulti-tier system simulator\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("This system is composed by\n");
    printf(" - Front Server\n - Back-end Server\n");
    printf("----------------------------------------------------------------------------------------\n\n");
    printf("Choose if Front Server service time has to be:\n");
    printf(" 1 - Exponetially distribuited\n");
    printf(" 2 - Distributed as a 10-Erlang\n");
    printf(" 3 - Distributed as Hyperexponential with p=0.1\n");
    printf("Your choice: ");
    scanf("%d", &choice);
    getchar();
    printf("----------------------------------------------------------------------------------------\n\n");

    switch(choice) {
        case 1:
            type = FE_EXP;
            break;
        case 2:
            type = FE_ERL;
            break;
        case 3:
            type = FE_HYP;
            break;
        default:
            printf("Errore. Nessuna distribuzione per il frontend selezionata.\n");
            return EXIT_FAILURE;
    }

    printf("Choose if threshold should be active:\n");
    printf("Your choice: ");
    scanf("%s", &t_flag);
    getchar();
    if(t_flag == 'Y' || t_flag == 'y') {
        printf("Threshold enabled\n");
        threshold_flag = 1;
    }
    else {
        printf("Threshold disabled\n");
        threshold_flag = 0;
    }
    printf("----------------------------------------------------------------------------------------\n\n");

    printf("Please select a SEED: \n");
    printf(" 1 - 615425336\n");
    printf(" 2 - 37524306\n");
    printf(" 3 - 123456789\n");
    printf(" 4 - Insert another SEED\n");
    printf("Your choice: ");
    scanf("%d", &choice);
    getchar();

    switch(choice) {
        case 1:
            SEED = 615425336;
            break;
        case 2:
            SEED = 37524306;
            break;
        case 3:
            SEED = 123456789;
            break;
        case 4:
            printf("Enter your SEED: ");
            scanf("%lld", &SEED);
            getchar();
            break;
        default:
            printf("Errore. Nessuna distribuzione per il frontend selezionata.\n");
            return EXIT_FAILURE;
    }

    printf("Chosen SEED is: %lld\n", SEED);
    printf("----------------------------------------------------------------------------------------\n\n");

    PutSeed(SEED);

    printf("Insert simulation parameters:\n");
    printf("Batch size (b): ");
    scanf("%ld", &batch_size);
    getchar();
    printf("Number of batches (k): ");
    scanf("%ld", &batch_num);
    getchar();

    printf("The inserted parameters are: %ld %ld\n", batch_size, batch_num);
    printf("----------------------------------------------------------------------------------------\n\n");

    printf("Would you like to see the \"system state\" during the simulation?\n");
    printf("Choose [Y/N]:");
    scanf("%c", &visual_flag);
    getchar();
    if(visual_flag == 'Y' || visual_flag == 'y')
        printf("You choose to displat system state\n");
    else
        printf("You choose NOT to displat system state\n");
    printf("----------------------------------------------------------------------------------------\n\n");

    printf("Press ANY key to start simulation\n");
    getchar();

    //Aprire i file per la simulazione e iniziare a scriverci dentro
    graphic = open_file();
    print_initial_settings(graphic, SEED, batch_size, batch_num);

    begin_simulation(graphic);

    //chiudere i file
    close_file(graphic);

    return EXIT_SUCCESS;
}
