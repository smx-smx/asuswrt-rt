#ifndef _TC_TIMER_H 
#define _TC_TIMER_H

typedef unsigned int UInt32;
typedef int	Int;
typedef int Int32;
typedef unsigned int Bool;


typedef int(*TimerCallback)(void *param);

#define OK (0)
#define ERROR (-1)

#define TRUE 1
#define FALSE 0

#define timerStartS(tm_out, func, param) \
	AddTimer(NULL, tm_out, func, param, FALSE)

#define timerPeriodicStartS(tm_out, func, param) \
	AddTimer(NULL, tm_out, func, param, TRUE)
	
//#define timerStopS(tm_id) \
//	RemoveTimer(tm_id)
	
#define timerPeriodicStop(tm_id) \
	RemoveTimer(tm_id)


int timer_init(void);
void timer_exit(void);
int AddTimer(char *timer_name, UInt32 interval, TimerCallback callback, void *param, Bool PeriodFlag);
int RemoveTimer(int timer_id);

#endif
