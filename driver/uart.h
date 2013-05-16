#ifndef SCI_H
#define SCI_H
#include <includes.h>
//串行通信寄存器及标志位定义
#define FE 1
#define OR 3
#define TCIE0 6
#define RIE0 5
#define RX_BUFFER_SIZE        50
#define TX_BUFFER_SIZE        50

/**********************************************************************************************************
*                                       GLOBAL VARIABLES
*********************************************************************************************************
*/
OS_EXT  INT8U    RxBuffer[RX_BUFFER_SIZE];        //Rx buffer
OS_EXT  INT8U    TxBuffer[TX_BUFFER_SIZE];        //Tx buffer
OS_EXT  INT8U    RxBufferOutPtr;                             //Rx buffer Out Pointer
OS_EXT  INT8U    RxBufferInPtr;                                //Rx buffer In Pointer
OS_EXT  INT8U    RxBufferCtr;                                  //Rx Buffer Counter
OS_EXT  INT8U    TxBufferOutPtr;                             //Tx buffer Out Pointer
OS_EXT  INT8U    TxBufferInPtr;                                //Tx buffer In Pointer
OS_EXT  OS_EVENT    *RxEventPtr;                         //Rx Event Pointer
OS_EXT  OS_EVENT    *TxEventPtr;                         //Tx Event Pointer

OS_EXT    INT8U      RxTaskStk[100];                         //Stack for the RxTask

/**********************************************************************************************************
*                                       FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void    SerialCfgPort(void);                   //USART Configuration
void    SerialInit(void);                          //Init the USART software module, and it must be called before any service provided by the module
//RX Module
INT8U    SerialGetChar(void);                                         //Get data from the ring RX Buffer,No Timeout
//INT8U    SerialRxBufEmpty(void);                                   //Ensure whether the Rx ring buffer is empty or not.

//TX Module
INT8U    SerialPutChar(INT8U pdata);                          //Enable app to send data to USART.
//INT8U    SerialTxBufFull(void);                                      //Ensure whether the Tx ring buffer is full or not.

void    RxTask(void *pdata);                                          //Serial Port Receive task

#endif 








