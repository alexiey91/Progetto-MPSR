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
    fprintf(g, "%s\t%s\n", "Threshold", "DA IMPLEMENTARE");
    fprintf(g, "%s\t%s\t%s\t%s\t", "STOP", "Util FS", "Sessions", "Requests");
    fprintf(g, "%s\t%s\t%s\t", "Throughput (Sessions)", "Throughput (Requests)", "Average Request per Session");
    fprintf(g, "%s\t%s\t%s\t%s\t\n", "# Dropped", "% Dropped", "# Aborted", "% Aborted");
    fflush(g);
}

void print_system_state_on_file(FILE *g) {
    fprintf(g, "%6.8f\t%6.8f\t%ld\t%ld\t", STOP, FS_average_utilization, sessions, requests);
    fprintf(g, "%6.8f\t%6.8f\t%6.8f\t", throughput_sessions, throughput_requests, (double) requests/sessions);
    fprintf(g, "%ld\t%6.8f\t%ld\t%6.8f\t\n", dropped, ((double)dropped/(dropped+sessions))*100.0, aborted, ((double)aborted/sessions)*100.0);
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
    printf("Utilization...............: %6.8f%%\n", FS_average_utilization*100);
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
    printf("Completed sessions........: %d\n", sessions);
    printf("Completed requests........: %d\n", requests);
    printf("Requests per session......: %6.8f\n", (double) requests/sessions);
    printf("Sessions throughput.......: %6.8f\n", throughput_sessions);
    printf("Requests throughput.......: %6.8f\n", throughput_requests);
// Solo nel caso peggiore    if(type == FE_ERL)
    printf("Connection dropped........: \n");
    printf("Connection aborted........: \n");
    printf("Threshold.................: \n");
    printf("\n");
}
