/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - Gestisce tutto il flusso di dati da scrivere su file
 */

#define MAX_DATETIME 150
extern SIMULATION_TYPES type;

char* get_date() {
    time_t td = time(NULL);
	struct tm *now = NULL;
	char * buffer = (char *)malloc(sizeof(char)*(MAX_DATETIME+1));
	buffer[MAX_DATETIME] = '\0';
	now = localtime(&td);     /* Get time and date structure */
	strftime (buffer, MAX_DATETIME, "%Y%m%d-%H%M%S",now);
	return buffer;
}

FILE *open_file() {
    char graphic_name[50], *g = graphic_name, *graphic_ext = ".xls";
    char *date = get_date();
    strcpy(graphic_name, date);
    g+=strlen(date);
    switch (type) {
        case FE_EXP:
            strcat(g,"-FE_EXP");
            g+=strlen("-FE_EXP");
            break;
        case FE_ERL:
            strcat(g,"-FE_ERL");
            g+=strlen("-FE_ERL");
            break;
        case FE_HYP:
            strcat(g,"-FE_HYP");
            g+=strlen("-FE_HYP");
            break;
        default:
            printf("Error. No known simulation type.\n");
    }
    if(threshold_flag) {
        strcat(g,"_OM");
        g+=strlen("_OM");
    }
    strcat(g, graphic_ext);
    g[strlen(graphic_ext)] = 0;
    free(date);
    return fopen(graphic_name, "a");
}

void close_file(FILE *g) {
    fclose(g);
}
