#ifndef _IEEE11073_USER_CONFIG_H 
#define _IEEE11073_USER_CONFIG_H 

/* User Defined MACRO to set number of Timer Objects */
#define MAX_TIMER_OBJECTS		    10

#if MAX_TIMER_OBJECTS
/* When Enabled Timer Callback is invoked with an argument */

#define TIMER_CALLBACK_ARG 

#endif
#endif /* _IEEE11073_USER_CONFIG_H */