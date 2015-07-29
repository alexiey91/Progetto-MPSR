/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - Gestisce tutto il flusso di dati da scrivere su file
 */

#define MAX_DATETIME 150

FILE *file, *graphic;
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

void print_header(FILE* graphicsFile) {
/*    fprintf(graphicsFile, "%s\t","Pop.Media_FS");
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
    fprintf(graphicsFile, "%s\t","Dropped");
    fprintf(graphicsFile, "%s\t","%_dropped");
    fprintf(graphicsFile, "%s\t","Aborted");
    fprintf(graphicsFile, "%s\t","%_aborted");
    fprintf(graphicsFile, "%s\t","#Medio_richieste/sessione");
    fprintf(graphicsFile, "\n");*/
}

void open_files() {
    char file_name[50], graphic_name[50], *f = file_name, *g = graphic_name;
    char *file_ext = ".txt", *graphic_ext = ".xls";
    char *date = get_date();
    strcpy(file_name, date);
    strcpy(graphic_name, date);
    f+=strlen(date);
    g+=strlen(date);
    switch (type) {
        case FE_EXP:
            strcat(f,"-FE_EXP");
            strcat(g,"-FE_EXP");
            f+=strlen("-FE_EXP");
            g+=strlen("-FE_EXP");
            break;
        case FE_ERL:
            strcat(f,"-FE_ERL");
            strcat(g,"-FE_ERL");
            f+=strlen("-FE_ERL");
            g+=strlen("-FE_ERL");
            break;
        case FE_HYP:
            strcat(f,"-FE_HYP");
            strcat(g,"-FE_HYP");
            f+=strlen("-FE_HYP");
            g+=strlen("-FE_HYP");
            break;
        default:
            printf("Error. No known simulation type.\n");
    }
    strcat(f, file_ext);
    strcat(g, graphic_ext);
    f[strlen(file_ext)] = 0;
    g[strlen(graphic_ext)] = 0;
    free(date);
    file = fopen(file_name, "a");
    graphic = fopen(graphic_name, "a");
}

void close_files() {
    fclose(file);
    fclose(graphic);
}
