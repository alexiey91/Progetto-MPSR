#include <time.h>

#define MAX_DATETIME 150

time_t get_datetime(){
	time_t time_value;
	time_value = time(NULL);
	return time_value;
}

// Return current time with a standard format
char* get_datetime_string(time_t td){
	struct tm *now = NULL;
	char * buffer = (char *)malloc(sizeof(char)*(MAX_DATETIME+1));
	buffer[MAX_DATETIME] = '\0';
	now = localtime(&td);     /* Get time and date structure */
	strftime (buffer, MAX_DATETIME, "%Y%m%d-%H%M%S",now);
	return buffer;
}

enum _SIMULATION_TYPES {
    FE_EXP,
    FE_ERL,
    FE_HYP
};

typedef enum _SIMULATION_TYPES SIMULATION_TYPES;

// Create and return a pointer to output file
FILE* open_file(SIMULATION_TYPES type, const char* ext, char* nome) {
    char nome_file[40], *t = nome_file; // YearMonthDay HourMinSec AR/TR/AT _ N/B.ext
    char* date = get_datetime_string(get_datetime());
    strcpy(nome_file, date);
    t+=strlen(date);

    switch(type) {
        case FE_EXP:
            strcat(t, "-FE_EXP");
            t += strlen("-FE_EXP");
            break;
        case FE_ERL:
            strcat(t, "-FE_ERL");
            t += strlen("-FE_ERL");
            break;
        case FE_HYP:
            strcat(t, "-FE_HYP");
            t += strlen("-FE_HYP");
            break;
        default:
            printf("Error. No known simulation type.\n");
    }

    strcat(t, ext);
    t[strlen(ext)] = 0;
    free(date);

//    printf("%sIL FILE SI CHIAMERA' %s%s\n", RED, nome_file, STD);
    if(nome != NULL) strcpy(nome, nome_file);
    return fopen(nome_file, "a");
}

void print_header(FILE* graphicsFile) {
    fprintf(graphicsFile, "%s\t","Pop.Media_FS");
    fprintf(graphicsFile, "%s\t","Pop.Media_BES");
    fprintf(graphicsFile, "%s\t","Util_FS");
    fprintf(graphicsFile, "%s\t","Util_BES");
    fprintf(graphicsFile, "%s\t","Residenza_FS");
    fprintf(graphicsFile, "%s\t","Residenza_BES");
    fprintf(graphicsFile, "%s\t","Throughput_(sessioni)");
    fprintf(graphicsFile, "%s\t","Throughput_(richieste)");
    fprintf(graphicsFile, "%s\t","Risposta_media");
    fprintf(graphicsFile, "%s\t","Sessioni");

//FIXME	if() per il simulation type peggiore tra erlang e iper
/*    fprintf(graphicsFile, "%s\t","Dropped");
    fprintf(graphicsFile, "%s\t","%_dropped");
    fprintf(graphicsFile, "%s\t","Aborted");
    fprintf(graphicsFile, "%s\t","%_aborted");
*/    fprintf(graphicsFile, "%s\t","#Medio_richieste/sessione");
    fprintf(graphicsFile, "\n");
}
