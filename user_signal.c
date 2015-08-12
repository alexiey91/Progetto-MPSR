#include <signal.h>

void sigprint() {
    print_system_state(SYSTEM_PRINT);
}

void add_signals() {
    signal(SIGUSR1, sigprint);
    signal(SIGUSR2, sigprint);
}
