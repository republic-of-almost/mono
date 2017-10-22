#ifndef APPLICATION_INCLUDED_8A24D651_97EF_4704_B6A6_91C5D4C46A47
#define APPLICATION_INCLUDED_8A24D651_97EF_4704_B6A6_91C5D4C46A47


/* ---------------------------------------------- [ Application Lifetime ] -- */
/*
 *  You must create a repo application before you start interacting with the
 *  engine. The general pattern would be this.
 *
 *  int
 *  main()
 *  {
 *    repo_app_init();
 *    repo_app_tick(NULL, NULL);
 *    repo_app_destroy();
 *  }
 *
 *  You can pass a callback to the tick to allow your gameplay code to execute.
 */


tyepdef void(*repo_tick_callback)(void *user_data);


void      repo_app_init();
void      repo_app_tick(void *user_data, repo_tick_callback cb);
void      repo_app_destroy();


#endif /* inc guard */
