
#include <includes.h>

void		USART1_Config( void ) ; 

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

static  OS_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void    AppTaskStart(void *p_arg);



/*
*********************************************************************************************************
*                                             C ENTRY POINT
*********************************************************************************************************
*/

int  main (void)
{
    INT8U  err;
    
    IntDisAll();                                                    		/* Disable ALL interrupts to the interrupt controller       */
		
    OSInit();                                                           /* Initialize uC/OS-II                                      */

                                                                        /* Create start task                                        */
    err = OSTaskCreateExt(AppTaskStart,
                        NULL,
                        (OS_STK *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1],
                        APP_TASK_START_PRIO,
                        APP_TASK_START_PRIO,
                        (OS_STK *)&AppTaskStartStk[0],
                        APP_TASK_START_STK_SIZE,
                        NULL,
                        OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

                                                                        /* Assign names to created tasks                            */
#if OS_TASK_NAME_SIZE > 11
    OSTaskNameSet(APP_TASK_START_PRIO, (INT8U *)"Start Task", &err);
#endif

    OSStart();                                                          /* Start uC/OS-II                                           */
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                              STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
* Arguments   : p_arg is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*               2) Interrupts are enabled once the task start because the I-bit of the CCR register was
*                  set to 0 by 'OSTaskCreate()'.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
 
    
     OS_CPU_SysTickInit();                                              /* Initialize the systick                                      */
    

    (void)p_arg;                                                        /* Prevent compiler warning                                 */
    

#if OS_TASK_STAT_EN > 0
    OSStatInit();                                                       /* Start stats task                                         */
#endif
		USART1_Config() ; 
	
    for(;;) {
    /*  user code ... */
    OSTimeDly(1000);
		printf("\r\n ucos!!");  
    }
    
    
}


