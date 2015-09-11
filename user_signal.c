#include <signal.h>

void sigprint() {
    clear_console();
    print_system_state(SYSTEM_PRINT);
}

void sigauto() {
    clear_console();
    print_system_state(SYSTEM_PRINT);
    exit(EXIT_SUCCESS);
}

void add_signals() {
    signal(SIGUSR1, sigprint);
    signal(SIGUSR2, sigauto);
}
