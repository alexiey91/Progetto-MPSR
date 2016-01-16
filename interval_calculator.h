/**
 *      Progetto MPSR anno 14/15.
 *      Authori: S. Martucci, A. Valenti
 *      Simulatore web - Intervalli di confidenza
 */

#define ALPHA 0.05

static double xm_res_time = 0.0;
static double v_res_time = 0.0;
static double n_res_time = 0.0;
static double incr_res_time = 0.0;

static double xm_t_sess = 0.0;
static double v_t_sess = 0.0;
static double n_t_sess = 0.0;
static double incr_t_sess = 0.0;

void set_ic_res_data(double value) {
    double d = value - xm_res_time;
    n_res_time++;
    v_res_time = v_res_time + d*d*(n_res_time-1)/n_res_time;
    xm_res_time = xm_res_time + d/n_res_time;
}

void set_ic_t_data(double value) {
    double d = value - xm_t_sess;
    n_t_sess++;
    v_t_sess = v_t_sess + d*d*(n_t_sess-1)/n_t_sess;
    xm_t_sess = xm_t_sess + d/n_t_sess;
}

void compute_ic_res() {
    double s = sqrt(v_res_time/n_res_time);
    //Calcolo il critical value
    double t = idfStudent((int) n_res_time-1, 1.0 - (ALPHA/2.0));
    incr_res_time = (t * s) / sqrt((double)(n_res_time-1));
}

void compute_ic_t() {
    double s = sqrt(v_t_sess/n_t_sess);
    //Calcolo il critical value
    double t = idfStudent((int) n_t_sess-1, 1.0 - (ALPHA/2.0));
    incr_t_sess = (t * s) / sqrt((double)(n_t_sess-1));
}

void print_ic_on_file(FILE *g) {
    compute_ic_t();
    compute_ic_res();
    fprintf(g, "\n");
    fprintf(g, "Response time IC = [ %6.8f ; %6.8f ]\n", (xm_res_time - incr_res_time), (xm_res_time + incr_res_time));
    fprintf(g, "Throughput IC = [ %6.8f ; %6.8f ]\n", (xm_t_sess - incr_t_sess), (xm_t_sess + incr_t_sess));
}
