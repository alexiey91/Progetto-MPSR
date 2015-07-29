/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - Tipologia di Eventi
 */

#define MIN_TYPE NEW_SESSION
#define MAX_TYPE CL_COMPL

enum _EVENT_TYPE {
    NEW_SESSION,
    FS_COMPL,
    BES_COMPL,
    CL_COMPL
};

typedef enum _EVENT_TYPE EVENT_TYPE;

struct Event {
	double time;
	EVENT_TYPE type;
	struct Event *next;
};

typedef struct Event Event;

int event_check(double t, EVENT_TYPE type) {
    return t>=0.0 && (type >= MIN_TYPE && type <= MAX_TYPE);
}

void printlist(Event **list) {
	if(*list == NULL) {
		printf("List is empty\n");
		return;
	}
	Event *curr = *list;
	printf("[+] List contains:\n");
	while(curr != NULL) {
		printf("[Type: %d, Time: %6.4f)\n", curr->type, curr->time);
		curr = curr->next;
	}
}

void add_event(Event **list, double x, EVENT_TYPE typ) {
    if(list == NULL) {
        printf("NULL Pointer to list\n");
        return;
    }
    if(!event_check(x, typ)) {
        printf("[+] Event check failed. Please review your data.\n");
        return;
    }
	Event *new_Event = (Event *)malloc(sizeof(Event));
	new_Event->time = x;
	new_Event->type = typ;
	new_Event->next = NULL;
	if(*list == NULL) { // empty list case
	//	printf("[T] Empty List\n");
		*list = new_Event;
		return;
	}
	if((*list)->time >= x) { // first element replacement case
	//	printf("[T] First element replacement\n");
		new_Event->next = *list;
		*list = new_Event;
	}
    //	printf("[T] Default add case\n");
    Event *curr = *list;
    while(curr->time < x) {
    //	printf("\n[T] curr->time: %d", curr->time);
        if(curr->next == NULL || curr->next->time >= x) {
            new_Event->next = curr->next;
            curr->next = new_Event;
            return;
        }
        else curr = curr->next;
    }
}

// Return the first element and remove it
Event* pop_event(Event** head) {
    if(head == NULL || *head == NULL) return NULL;
    Event* temp = *head;
    *head = (*head)->next;
    return temp;
}
