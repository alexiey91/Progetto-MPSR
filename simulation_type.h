/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - Struttura dati per la simulazione
 */

enum _SIMULATION_TYPES {
    FE_EXP,
    FE_ERL,
    FE_HYP
};

typedef enum _SIMULATION_TYPES SIMULATION_TYPES;

char* simulation_traslator(SIMULATION_TYPES t){
	switch(t){
		case FE_EXP:
            return "Exponential";
		break;
		case FE_ERL:
            return "Erlang";
		break;
		case FE_HYP:
            return "Hyperexponential";
		break;
		default:
            return "Error event";
		break;
	}
	return "Wrong code!\n";
}
