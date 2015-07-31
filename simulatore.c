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
#include "autocorrelation.h"
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
    sessions = 0;
    requests = 0;
    throughput_sessions = 0.0;
    throughput_requests = 0.0;
    arrivals = 1;
    current_time = START;
    prev_time = START;
    ev_list = NULL;
    reset_correlation();
    add_event(&ev_list, GetArrival(START), NEW_SESSION);
}

void begin_simulation() {
    int i = 0;
    initialize();
    // settare tutte le variabili

    while(ev_list != NULL) {
        Event *current = pop_event(&ev_list);
        current_time = current->time;
        // Aggiorno tutte le variabili
        // che mi servono (capire quali)
        FS_utilization += (current_time - prev_time) * (busy_FS);
        FS_average_utilization = FS_utilization / current_time;
        throughput_requests = ((double) requests)/current_time;
        throughput_sessions = ((double) sessions)/current_time;

        manage_event(current);
        prev_time = current_time;
        if(arrivals && current_time >= STOP)
            arrivals = 0;
        if(visual_flag == 'Y' || visual_flag == 'y') {
            if(i%50 == 0) {
                clear_console();
                // Stampo cose
                print_system_state(current->type);
                i=0;
                usleep(50000);
            }
            i++;
        }
    }
    compute_autocorrelation();
    printf("\nSIMULAZIONE CONCLUSA\n\n");
}

int main (int argc, char *argv[]) {

    int choice, i, numero_run;
    long long int SEED = 0;
    double init, fin, step;
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
    printf("Firts stop (time): ");
    scanf("%lf", &init);
    getchar();
    printf("Last stop (time): ");
    scanf("%lf", &fin);
    getchar();
    printf("Execution number: ");
    scanf("%d", &numero_run);
    getchar();

    printf("The inserted parameters are: %6.4f %6.4f %d\n", init, fin, numero_run);
    step = (fin-init)/numero_run;
    printf("Step length: %6.4f\n", step);
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
    print_initial_settings(graphic, SEED, step, numero_run);

    for(i=init; i<=fin; i+=step) {
        STOP = i;
        begin_simulation();
        print_system_state_on_file(graphic);
        if(init == fin) break;
    }

    //chiudere i file
    close_file(graphic);

    return EXIT_SUCCESS;
}
