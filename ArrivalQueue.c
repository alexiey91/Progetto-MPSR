/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - Coda di arrivi
 */

struct ArrivalNode{
    double time;
    struct ArrivalNode* next;
};

typedef struct ArrivalNode ArrivalNode;

void arrival_add(ArrivalNode** list, double t){
    if(list == NULL) {
        printf("Errore: la lista è NULL\n");
        return;
    }
    ArrivalNode* temp = (ArrivalNode*) malloc(sizeof(ArrivalNode));
    temp->time = t;
    temp->next = NULL;
    if(*list == NULL){
        *list = temp;
        return;
    }

    ArrivalNode* curr = *list;
    while(curr->next != NULL){
        curr = curr->next;
    }
    curr->next = temp;
}

double arrival_pop(ArrivalNode** list){
    if(list == NULL || *list ==NULL) {
        printf("ArrivalQueue is empty\n");
        return -1.0;
    }
    ArrivalNode* curr = *list;
    *list = (*list)->next;

    double t = curr->time;
    free(curr);

    return t;
}

void arrival_print(ArrivalNode** list){
    if(list == NULL || *list ==NULL) {
        printf("ArrivalQueue is empty\n");
        return;
    }
    ArrivalNode *curr=*list;
    printf("[+] Arrival Queue:\n");
    while(curr != NULL){
        printf("[Time: %6.4f]  -- ",curr->time);
		curr = curr->next;
    }
}
