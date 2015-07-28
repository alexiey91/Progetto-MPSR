/**---------------------------------------------------------------------------------------------
 *        MPSR Project. Request Queue                                                           *
 * Authors: G. Montenero, L. Paoli, R. Petrillo                                                 *
 * Last Modified: 16/7/2014                                                                     *
 * Developed with: Code::Blocks 13.12                                                           *
 * Compiled with: GNU GCC Compiler 4.8.1                                                        *
  ----------------------------------------------------------------------------------------------*/

struct Request{
    unsigned int n;
    struct Request* next;
};

typedef struct Request Request;

void enqueue_req(Request** list, unsigned int t){
    if(list == NULL) {
        printf("ReqQueue is NULL\n");
        return;
    }
    Request* temp = (Request*) malloc(sizeof(Request));
    temp->n = t;
    temp->next = NULL;
    if(*list == NULL){
        *list = temp;
        return;
    }

    Request* curr = *list;
    while(curr->next != NULL){
        curr = curr->next;
    }
    curr->next = temp;
}

unsigned int dequeue_req(Request** list){
    if(list == NULL || *list ==NULL) {
        printf("ReqQueue is empty\n");
        return 0.0;
    }
    Request* curr = *list;
    *list = (*list)->next;

    unsigned int t = curr->n;
    free(curr);

    return t;
}

void print_req(Request** list){
    if(list == NULL || *list ==NULL) {
        printf("ReqQueue is empty\n");
        return;
    }
    Request *curr=*list;
    printf("[+] ReqQueue:\n");
    while(curr != NULL){
        printf("[n: %d]  -- ",curr->n);
		curr = curr->next;
    }
    printf("\n");
}
