/**---------------------------------------------------------------------------------------------
 *        MPSR Project. Client Order List                                                       *
 * Authors: G. Montenero, L. Paoli, R. Petrillo                                                 *
 * Last Modified: 16/7/2014                                                                     *
 * Developed with: Code::Blocks 13.12                                                           *
 * Compiled with: GNU GCC Compiler 4.8.1                                                        *
  ----------------------------------------------------------------------------------------------*/

struct ClientReq {
	double time; // time of Client completion
	unsigned int req; // number of remaining requests
	struct ClientReq *next;
};

typedef struct ClientReq ClientReq;

void print_client_req(ClientReq **list) {
	if(*list == NULL) {
		printf("ClientOrderList is empty\n");
		return;
	}
	ClientReq *curr = *list;
	printf("[+] ClientOrderList contains:\n");
	while(curr != NULL) {
		printf("[Time: %6.4f, Rem_Req: %d]\n", curr->time, curr->req);
		curr = curr->next;
	}
}

void add_client_req(ClientReq **list, double x, unsigned int r) {
    if(list == NULL) {
        printf("NULL Pointer to ClientOrderList\n");
        return;
    }

	ClientReq *new_ClientReq = (ClientReq *)malloc(sizeof(ClientReq));
	new_ClientReq->time = x;
	new_ClientReq->req = r;
	new_ClientReq->next = NULL;
	if(*list == NULL) { // empty list case
	//	printf("[T] Empty List\n");
		*list = new_ClientReq;
		return;
	}
	if((*list)->time >= x) { // first element replacement case
	//	printf("[T] First element replacement\n");
		new_ClientReq->next = *list;
		*list = new_ClientReq;
	}
    //	printf("[T] Default add case\n");
    ClientReq *curr = *list;
    while(curr->time < x) {
    //	printf("\n[T] curr->time: %d", curr->time);
        if(curr->next == NULL || curr->next->time >= x) {
            new_ClientReq->next = curr->next;
            curr->next = new_ClientReq;
            return;
        }
        else curr = curr->next;
    }
}

// Return the first element and remove it
ClientReq* pop_ClientReq(ClientReq** head) {
    if(head == NULL || *head == NULL) return NULL;
    ClientReq* temp = *head;
    *head = (*head)->next;
    return temp;
}
