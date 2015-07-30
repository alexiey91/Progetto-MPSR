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
    printf("STOP.............: %6.4f\n", STOP);
    printf("Time.............: %6.4f\n", current_time);
    printf("Event type.......: %s\n", event_translator(t));
//    printf("FS utilization...: %6.4f\n", curr_utilFS);
    printf("FS distribution..: %s\n", simulation_traslator(type));
    printf("FS queue length..: %d\n", queue_length_FS);
    printf("FS busy..........: %s\n", (busy_FS) ? "YES" : "NO");
    printf("BES queue length.: %d\n", queue_length_BES);
    printf("BES busy.........: %s\n", (busy_BES) ? "YES" : "NO");
    printf("Number of client.: %d\n", active_client);
}
