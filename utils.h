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

void print_initial_settings(FILE *g, long long int seed, long bs, long bn) {
    fprintf(g, "%s\t%lld\t\t%s\t%s\n", "SEED", seed, "FS distribution", simulation_traslator(type));
    if(SIM_TYPE == 2) {
        fprintf(g, "%s\t%ld\t\t%s\t%ld\n", "Batch Size", bs, "Total Batch", bn);
    } else {
        fprintf(g, "%s\t%ld\t\t%s\t%ld\n", "Stop time", bs, "Step", bn);
    }
    fprintf(g, "%s\t%s\t\n\n", "Threshold", (threshold_flag) ? "YES : 85%" : "NONE");
    fprintf(g, "%s\t%s\t%s\t%s\t", (SIM_TYPE) ? "Current batch" : "Current Stop", "Util FS", "Sessions", "Requests");
    fprintf(g, "%s\t%s\t%s\t%s\t", "Average Response Time", "Throughput (Sessions)", "Throughput (Requests)", "Average Request per Session");
    fprintf(g, "%s\t%s\t%s\t%s\t%s\t%s\t\n\n", "# Completed", "% Completed", "# Dropped", "% Dropped", "# Aborted", "% Aborted");
    fflush(g);
}

void print_system_state_on_file(FILE *g) {
    fprintf(g, "%ld\t%6.16f\t%ld\t%ld\t", (SIM_TYPE) ? current_batch : CURRENT_STOP, FS_average_utilization, opened_sessions, requests);
    fprintf(g, "%6.8f\t%6.8f\t%6.8f\t%6.8f\t", average_res_FS + average_res_BES, throughput_sessions, throughput_requests, (double) requests/opened_sessions);
    fprintf(g, "%ld\t%6.8f\t%ld\t%6.8f\t%ld\t%6.8f\t\t\n", completed_sessions, ((double) completed_sessions/opened_sessions)*100.0, dropped, ((double)dropped/(dropped+opened_sessions))*100.0, aborted, ((double)aborted/opened_sessions)*100.0);
    fflush(g);
}

void print_final_state(FILE *g) {
    fprintf(g, "\n");
    fprintf(g, "%s\t%6.16f\t%ld\t%ld\t", "Final Statistics", __FS_average_utilization/batch_num, __opened_sessions/batch_num, __requests/batch_num);
    fprintf(g, "%6.8f\t%6.8f\t%6.8f\t%6.8f\t", average_res_FS + average_res_BES, __throughput_sessions, __throughput_requests, (double)__requests/__opened_sessions);
    fprintf(g, "%ld\t%6.8f\t%ld\t%6.8f\t%ld\t%6.8f\t\t\n", __completed_sessions, ((double)__completed_sessions/__opened_sessions)*100.0, __dropped, ((double)__dropped/(__dropped+__opened_sessions))*100.0, __aborted, ((double)__aborted/__opened_sessions)*100.0);
    fprintf(g, "%s\t%6.8f\n", "ELAPSED TIME:", current_time);
    fflush(g);
}

void print_system_state(EVENT_TYPE t) {
    printf(":::::::::::::::::::::::::::::: Timer ::::::::::::::::::::::::::::::\n");
    if(SIM_TYPE)
        printf("Current batch.............: %ld of %ld\n", current_batch, batch_num);
    else
        printf("Current stop..............: %ld of %ld\n", CURRENT_STOP, STOP);
    printf("Current job...............: %ld of %ld\n", completed_sessions, batch_size);
    printf("Current time..............: %6.8f\n", current_time);
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
    printf("Threshold.................: %s\n", threshold_flag ? "YES : 85%" : "NONE");
    printf("\n");
}
