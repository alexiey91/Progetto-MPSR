/**---------------------------------------------------------------------------------------------
 *        MPSR Project. ArrivalQueue                                                            *
 * Authors: G. Montenero, L. Paoli, R. Petrillo                                                 *
 * Last Modified: 16/7/2014                                                                     *
 * Developed with: Code::Blocks 13.12                                                           *
 * Compiled with: GNU GCC Compiler 4.8.1                                                        *
  ----------------------------------------------------------------------------------------------*/

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
