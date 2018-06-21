#ifndef REPUBLIC_API_INCLUDED
#define REPUBLIC_API_INCLUDED


#include <republic/republic.h>


/* ------------------------------------------------------- [ Application ] -- */


typedef void(*rep_app_create_fn)();
typedef void(*rep_app_destroy_fn)();


struct rep_app_api {
        rep_app_create_fn create_fn;
        rep_app_destroy_fn destroy_fn;
};


void
rep_api_application_set(
        struct rep_app_api *api);


/* -------------------------------------------------------- [ Window API ] -- */


typedef void(*rep_win_resize_fn)(int width, int height);
typedef void(*rep_win_close_fn)();


struct rep_window_api {
        rep_win_resize_fn resize_fn;
        rep_win_resize_fn resize_cb_fn;

        rep_win_close_fn close_fn;
        rep_win_close_fn close_cb_fn;
};


void
rep_api_window_set(
        struct rep_window_api *api);


/* -------------------------------------------------------------- [ Task ] -- */


typedef REP_TASK_MARKER(*rep_task_submit_fn)(struct *rep_task_desc, int count);
typedef void(*rep_task_wait_fn)(REP_TASK_MARKER marker);


struct rep_task_api {
        rep_task_submit_fn submit_fn;
        rep_task_wait_fn wait_fn;
};


void
rep_api_task_set(
        struct rep_task_api *api);


/* endif */
#endif

