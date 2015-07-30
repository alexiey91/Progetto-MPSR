/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - Utility varie
 */

void clear_console() {
    char str[] = {0x1b, 0x5b, 0x48, 0x1b, 0x5b, 0x4a, '\0'};
    printf("%s", str);
}

void print_system_state(EVENT_TYPE t) {
    printf(":::::::::::::::::::::::::::::: Timer ::::::::::::::::::::::::::::::\n");
    printf("Stop......................: %6.4f\n", STOP);
    printf("Current...................: %6.4f\n", current_time);
    printf("\n");
    printf(":::::::::::::::::::::::: Front Server Info ::::::::::::::::::::::::\n");
    printf("Choosen distribution......: %s\n", simulation_traslator(type));
    printf("Queue length..............: %d\n", queue_length_FS);
    printf("Active....................: %s\n", (busy_FS) ? "YES" : "NO");
    printf("Average response time.....: %6.4f\n", average_res_FS);
    printf("Utilization...............: %s\n", "DA IMPLEMENTARE");
    printf("\n");
    printf("::::::::::::::::::::::: Back-end Server Info ::::::::::::::::::::::\n");
    printf("Queue length..............: %d\n", queue_length_BES);
    printf("Active....................: %s\n", (busy_BES) ? "YES" : "NO");
    printf("Average response time.....: %6.4f\n", average_res_BES);
    printf("\n");
    printf("::::::::::::::::::::::::::: Client Info :::::::::::::::::::::::::::\n");
    printf("Number of active client...: %d\n", active_client);
    printf("Average response time.....: %6.4f\n", average_res_client);
    printf("\n");
    printf("::::::::::::::::::::::::::: Generic Info ::::::::::::::::::::::::::\n");
    printf("Event type................: %s\n", event_translator(t));
    printf("Completed sessions........: %d\n", sessions);
    printf("Completed requests........: %d\n", requests);
    printf("Requests per session......: %6.4f\n", (double) requests/sessions);
    printf("Sessions throughput.......: %6.4f\n", (double) sessions/current_time);
    printf("Requests throughput.......: %6.4f\n", (double) requests/current_time);
// Solo nel caso peggiore    if(type == FE_ERL)
    printf("Connection dropped........: \n");
    printf("Connection aborted........: \n");
    printf("Threshold.................: \n");
    printf("\n");
}
