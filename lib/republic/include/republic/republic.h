#ifndef REPUBLIC_ENGINE
#define REPUBLIC_ENGINE


/* ----------------------------------------------------------- [ General ] -- */


typedef int REP_RESULT;

#define REP_RESULT_FAIL 0
#define REP_RESULT_SUCCESS 1


/* ------------------------------------------------------- [ Application ] -- */


struct rep_application_desc {
        void * ext;
};


REP_RESULT
rep_application_create();


REP_RESULT
rep_application_destroy();


/* ------------------------------------------------------------ [ Window ] -- */


struct rep_window_desc {
        int width;
        int height;

        void * ext;
};


int
rep_window_width();


int
rep_window_height();


int
rep_window_close();


/* -------------------------------------------------------------- [ Task ] -- */


typedef void(*rep_task_fn)();
typedef int REP_TASK_MARKER;


struct rep_task_desc {
        rep_task_fn task_function;
        void * task_argument;

        void * ext;
};



REP_TASK_MARKER
rep_task_submit(
        struct rep_task_desc task_descs,
        int task_count);


void
rep_task_wait(
        REP_TASK_MARKER marker);


/* inc guard */
#endif

