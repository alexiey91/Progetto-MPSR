/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - Utility varie per la stampa
 */
void clear_console() {
#ifdef __WIN32
    system("cls");
#else
    // Assume POSIX
    char str[] = {0x1b, 0x5b, 0x48, 0x1b, 0x5b, 0x4a, '\0'};
    printf("%s", str);;
#endif
}

void print_initial_settings(FILE *g, long long int seed, double step, int numero_run) {
    fprintf(g, "%s\t%lld\t\t%s\t%s\n", "SEED", seed, "FS distribution", simulation_traslator(type));
    fprintf(g, "%s\t%6.8f\t\t%s\t%d\n", "STEP", step, "Num run", numero_run);
    fprintf(g, "%s\t%s\t\t\t\t\t\t\t\t\t\t\t\t\t\t%s\n\n", "Threshold", (type == FE_ERL) ? "YES : 85%" : "NONE", "Autocorrelation");
    fprintf(g, "%s\t%s\t%s\t%s\t%s\t", "STOP", "EXECUTION TIME", "Util FS", "Sessions", "Requests");
    fprintf(g, "%s\t%s\t%s\t%s\t", "Average Response Time", "Throughput (Sessions)", "Throughput (Requests)", "Average Request per Session");
    fprintf(g, "%s\t%s\t%s\t%s\t%s\t%s\t\t", "# Completed", "% Completed", "# Dropped", "% Dropped", "# Aborted", "% Aborted");
    for(int j = 0; j < N_AUTOCORR; j++) {
        fprintf(g, "J = %d\t", j+1);
    }
    fprintf(g, "\n\n");
    fflush(g);
}

void print_system_state_on_file(FILE *g) {
    fprintf(g, "%6.8f\t%6.8f\t%6.16f\t%ld\t%ld\t", STOP, current_time, FS_average_utilization, opened_sessions, requests);
    fprintf(g, "%6.8f\t%6.8f\t%6.8f\t%6.8f\t", average_res_FS + average_res_BES, throughput_sessions, throughput_requests, (double) requests/opened_sessions);
    fprintf(g, "%ld\t%6.8f\t%ld\t%6.8f\t%ld\t%6.8f\t\t", completed_sessions, ((double) completed_sessions/opened_sessions)*100.0, dropped, ((double)dropped/(dropped+opened_sessions))*100.0, aborted, ((double)aborted/opened_sessions)*100.0);
    fflush(g);
}

void print_autocorrelation_on_file(FILE *g) {
    for(int j = 0; j<N_AUTOCORR; j++) {
        fprintf(g, "%6.16f\t", autocorr_value[j]);
    }
    fprintf(g, "\n");
    fflush(g);
}

void print_system_state(EVENT_TYPE t) {
    printf(":::::::::::::::::::::::::::::: Timer ::::::::::::::::::::::::::::::\n");
    printf("Stop......................: %6.8f\n", STOP);
    printf("Current...................: %6.8f\n", current_time);
    printf("\n");
    printf(":::::::::::::::::::::::: Front Server Info ::::::::::::::::::::::::\n");
    printf("Choosen distribution......: %s\n", simulation_traslator(type));
    printf("Queue length..............: %d\n", queue_length_FS);
    printf("Active....................: %s\n", (busy_FS) ? "YES" : "NO");
    printf("Average response time.....: %6.8f\n", average_res_FS);
    printf("Average utilization.......: %6.8f%%\n", FS_average_utilization*100);
    printf("\n");
    printf("::::::::::::::::::::::: Back-end Server Info ::::::::::::::::::::::\n");
    printf("Queue length..............: %d\n", queue_length_BES);
    printf("Active....................: %s\n", (busy_BES) ? "YES" : "NO");
    printf("Average response time.....: %6.8f\n", average_res_BES);
    printf("\n");
    printf("::::::::::::::::::::::::::: Client Info :::::::::::::::::::::::::::\n");
    printf("Number of active client...: %d\n", active_client);
    printf("Average response time.....: %6.8f\n", average_res_client);
    printf("\n");
    printf("::::::::::::::::::::::::::: Generic Info ::::::::::::::::::::::::::\n");
    printf("Event type................: %s\n", event_translator(t));
    printf("Opened sessions...........: %ld\n", opened_sessions);
    printf("Completed sessions........: %ld\n", completed_sessions);
    printf("Completed requests........: %ld\n", requests);
    printf("Requests per session......: %6.8f\n", (double) requests/opened_sessions);
    printf("Sessions throughput.......: %6.8f\n", throughput_sessions);
    printf("Requests throughput.......: %6.8f\n", throughput_requests);
    printf("Connection dropped........: %ld\n", dropped);
    printf("Connection aborted........: %ld\n", aborted);
    printf("Threshold.................: %s\n", (type == FE_ERL) ? "YES : 85%" : "NONE");
    printf("\n");
}
