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

#include "Utils.c"
#include "rng.c"

#define START 0.0           // il tempo di inizio
double STOP;                // il tempo a cui finire

SIMULATION_TYPES type;      // parametro per definire la dstribuzione del front server

int main (int argc, char *argv[]) {

    FILE *file, *graphic;
    int choice, i, numero_run;
    long long int SEED = 0;
    double init, fin, step;
    char file_name[50];

    if(argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("\e[1;1H\e[2J"); //serve a pulire la console
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

    printf("The inserted parameters are: %4.2f %4.2f %d\n", init, fin, numero_run);
    step = (fin-init)/numero_run;
    printf("Step length: %4.2f\n", step);

    //Aprire i file per la simulazione e iniziare a scriverci dentro

    printf("Press ANY key to start simulation\n");
    getchar();

    for(i=init; i<=fin; i+=step) {
        STOP = i;
        printf("\e[1;1H\e[2J"); //serve a pulire la console
        //fare la simulazione
        if(init == fin) break;
    }

    //chiudere i file

    return EXIT_SUCCESS;
}
