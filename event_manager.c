/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - Processamento degli eventi
 */

long FS_counter, BES_counter, client_counter;
int queue_length_FS, queue_length_BES, busy_FS, busy_BES, active_client;
double average_res_FS, average_res_BES, average_res_client;
int threshold_exceeded;

// NewSession Management
void NewSession(Event* ev) {

    // If current time is not beyond STOP, a NewSession is scheduled
    if(arrivals) {
        double new_time = GetArrival(ev->time); // compute new session arrival time
        if(new_time < FIN) {
            add_event(&ev_list, new_time, NEW_SESSION);   // create NewSession event and schedule it
        }
    }

    if(threshold_flag) {
        if(threshold_exceeded) {
            if(FS_average_utilization <= THRESHOLD_MIN)
                threshold_exceeded = 0;
            else {
                dropped++;
                return;
            }
        }
        // la 10-Erlang è la peggiore implemento quindi il Threshold all' 85%
        else if(FS_average_utilization >= THRESHOLD_MAX) {
            threshold_exceeded = 1;
            dropped++;
            return;
        }
    }
    opened_sessions++;
    enqueue_req(&req_queue, GetRequests());
    arrival_add(&arrival_queue_FS, ev->time);
    if(busy_FS) queue_length_FS++;     // if server is busy, add 1 to its queue
    else {
        busy_FS = 1;
        switch(type) {
            case FE_EXP:
                add_event(&ev_list, GetExponentialServiceFS(ev->time), FS_COMPL); // create FS_COMPL event and schedule it
                break;
            case FE_ERL:
                add_event(&ev_list, GetErlangServiceFS(ev->time), FS_COMPL); // create FS_COMPL event and schedule it
                break;
            case FE_HYP:
                add_event(&ev_list, GetHyperexpServiceFS(ev->time), FS_COMPL); // create FS_COMPL event and schedule it
                break;
        }
    }
}

// FS Completion Management
void FS_Completion(Event* ev) {

    // calculate average residence time in FS center. (Welford + destructive pop)
    FS_counter++;
    double res_time_FS = ev->time - arrival_pop(&arrival_queue_FS); // i-th session's residence time
    average_res_FS = average_res_FS + (res_time_FS - average_res_FS)/FS_counter;
    arrival_add(&arrival_queue_BES, ev->time); // save entrance time into BE center

    // Exiting from FS
    if(queue_length_FS > 0) {
        queue_length_FS--;
        switch(type) {
            case FE_EXP:
                add_event(&ev_list, GetExponentialServiceFS(ev->time), FS_COMPL); // create FS_COMPL event and schedule it
                break;
            case FE_ERL:
                add_event(&ev_list, GetErlangServiceFS(ev->time), FS_COMPL); // create FS_COMPL event and schedule it
                break;
            case FE_HYP:
                add_event(&ev_list, GetHyperexpServiceFS(ev->time), FS_COMPL); // create FS_COMPL event and schedule it
                break;
        }
    }
    else busy_FS = 0;

    // Entering in BES
    if(busy_BES) queue_length_BES++;
    else {
        busy_BES = 1;
        add_event(&ev_list, GetServiceBES(ev->time), BES_COMPL);
    }
}

// BES Completion Management
void BES_Completion(Event* ev) {

    // calculate average residence time in BES center. (Welford + destructive pop)
    BES_counter++;
    double res_time_BES = ev->time - arrival_pop(&arrival_queue_BES); // i-th session's residence time
    average_res_BES = average_res_BES + (res_time_BES - average_res_BES)/BES_counter;
    // Exiting from BES
    if(queue_length_BES > 0) {
        queue_length_BES--;
        add_event(&ev_list, GetServiceBES(ev->time), BES_COMPL);
    }
    else busy_BES = 0;

    //Entering Client
    unsigned int this_requests = dequeue_req(&req_queue);
    if(this_requests <= 0) {
        // session is over. It will move out of the system
        completed_sessions++;
    }
    else {
        // This session still has some requests to be executed. Let's go to the Clients
        requests++;
        client_counter++;
        active_client++; // increase the number of active clients

        // Calculate average residence time in Clients center
        double temp = GetServiceClient(ev->time);

        double time_res_client = temp - ev->time;
        add_event(&ev_list, temp, CL_COMPL);

        average_res_client = average_res_client + (time_res_client - average_res_client)/client_counter;

        // Add this time and this session's remaining requests into ClientOrderList
        add_client_req(&client_req_list, temp, this_requests-1);
    }
}

// Client Completion Management
void Client_Completion(Event* ev) {
    active_client--; // decrease number of Clients
    // Insert the updated value of remaining requests (from THIS session) into the ReqQueue
    ClientReq* coming_back_session = pop_ClientReq(&client_req_list);

    // Gestire l'abort delle sessioni
    if(threshold_flag) {
        if(threshold_exceeded) {
            if(FS_average_utilization <= THRESHOLD_MIN)
                threshold_exceeded = 0;
            else {
                aborted++;
                //sessions++; // FIXME
                free(coming_back_session);
                return;
            }
        }
        else if(FS_average_utilization >= THRESHOLD_MAX) {
            threshold_exceeded = 1;
            aborted++;
            //sessions++; // FIXME
            free(coming_back_session);
            return;
        }
    }

    enqueue_req(&req_queue, coming_back_session->req);
    free(coming_back_session);

    if(busy_FS == 0) {
        busy_FS = 1;
        switch(type) {
            case FE_EXP:
                add_event(&ev_list, GetExponentialServiceFS(ev->time), FS_COMPL); // create FS_COMPL event and schedule it
                break;
            case FE_ERL:
                add_event(&ev_list, GetErlangServiceFS(ev->time), FS_COMPL); // create FS_COMPL event and schedule it
                break;
            case FE_HYP:
                add_event(&ev_list, GetHyperexpServiceFS(ev->time), FS_COMPL); // create FS_COMPL event and schedule it
                break;
        }
    }
    else queue_length_FS++;
    arrival_add(&arrival_queue_FS, ev->time);
}

void manage_event(Event *e) {
    switch(e->type) {
        case NEW_SESSION:
            NewSession(e);
            break;
        case FS_COMPL:
            FS_Completion(e);
            break;
        case BES_COMPL:
            BES_Completion(e);
            break;
        case CL_COMPL:
            Client_Completion(e);
            break;
        default:
            printf("Wrong event!\n");
    }
    free(e);
}
