/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - Struttura dati per la simulazione
 */

#define New_Session      0
#define Service_FS       1
#define Service_BES      2
#define Service_C        3

enum _SIMULATION_TYPES {
    FE_EXP,
    FE_ERL,
    FE_HYP
};

typedef enum _SIMULATION_TYPES SIMULATION_TYPES;

char* translator(int num){
	switch(num){
		case New_Session:
            return "New session";
		break;
		case Service_FS:
            return "Front server completition";
		break;
		case Service_BES:
            return "Back-end server completition";
		break;
		case Service_C:
            return "Client completition";
		break;
		default:
            return "Error event";
		break;
	}
	return "Wrong code!\n";
}
