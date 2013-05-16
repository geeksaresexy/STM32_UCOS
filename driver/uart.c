/*
 * =====================================================================================
 *
 *       Filename:  uart.c
 *
 *    Description:  ���ļ�ʵ�ִ��ڵ��������룬������ST�ٷ��⣬�ж�ʵ�֡�
 *
 *        Version:  1.0
 *        Created:  2013/2/3 17:29:58
 *       Revision:  none
 *       Compiler:  ARMCC
 *
 *         Author:  ���� , killkeil54@gmail.com
 *        Company:  HBUT
 *
 *         Target:  STM32F103 @8MHZ
 * =====================================================================================
 */

#include<uart.h>

INT8U    RxBuffer[RX_BUFFER_SIZE];              //Rx buffer
INT8U    TxBuffer[TX_BUFFER_SIZE];              //Tx buffer
INT8U    RxBufferOutPtr;                        //Rx buffer Out Pointer
INT8U    RxBufferInPtr;                         //Rx buffer In Pointer
INT8U    RxBufferCtr;                           //Rx Buffer Counter
INT8U    TxBufferOutPtr;                        //Tx buffer Out Pointer
INT8U    TxBufferInPtr;                         //Tx buffer In Pointer
OS_EVENT    *RxEventPtr;                        //Rx Event Pointer

INT8U    RxTaskStk[100];                        //Stack for the RxTask


/**********************************************************************************************************
*                                             INITIALIZATION
* Description: This function is used to initialize the software layer and must be called prior to SerialCfgPort()
* Arguments  : none
* Returns    : none
**********************************************************************************************************/
void SerialInit(void)
{
    RxEventPtr=OSSemCreate(0);
    TxEventPtr=OSSemCreate(TX_BUFFER_SIZE-1);    //create the semaphore with the cnt value of TX_BUFFER_SIZE -1
    if(RxEventPtr==(void *)0)
        return;
    RxBufferInPtr=0;
    RxBufferOutPtr=0;
    TxBufferInPtr=0;
    TxBufferOutPtr=0;
    RxBufferCtr=0;
}

/**********************************************************************************************************
*                                             Configuration
* Description: This function is used to configure the USART Port and MUST be called before other service
* Arguments  : none
* Returns    : none
**********************************************************************************************************/
void SerialCfgPort(void)
{
    INT8U t;
    //Set baud rate at Bt=115200bps ,SCI0BD=fBUS/(16*Bt)
    SCI0BDL = 0x2B;    
    SCI0BDH = 0x00;    
    SCI0CR1 = 0x00;    //Set frame format: 8data, 1stop bit
    t = SCI0SR1;  
    t = SCI0DRL;       //clear re   
    SCI0CR2 = 0x0C;    //Enable receiver and transmitter  

}


/**********************************************************************************************************
*                                           Serial Port Get Data
* Description: This function gets the data for rx buffer
* Arguments  : none
* Returns    : rx data
**********************************************************************************************************/
INT8U SerialGetChar(void)
{
    INT8U ErrMsg;
    INT8U rxdata;
    OSSemPend(RxEventPtr,0,&ErrMsg);
    rxdata=RxBuffer[RxBufferOutPtr];
    if(++RxBufferOutPtr == RX_BUFFER_SIZE){
        RxBufferOutPtr=0;    
    }
    if(RxBufferCtr > 0){
        RxBufferCtr--;
    }
    return (rxdata);
}



/**********************************************************************************************************
*                                           Serial Put Rx Char into the RX Buffer
* Description: This puts the Rx data into the RX Buffer
* Arguments  :  status      store the value of UCSR0A status register
                        data        store the value of UDR0 register
* Returns    : none
**********************************************************************************************************/
static void SerialPutRxChar(INT8U status)
{
    if((status &((1<<FE)|(1<<OR)) )==0){//SericalPutRxChar
        if(RxBufferCtr < RX_BUFFER_SIZE){
            RxBuffer[RxBufferInPtr]=SCI0DRL;        //store the data to the rx buffer ,receive data full flag clear 
            if(++RxBufferInPtr == RX_BUFFER_SIZE){    //Pointer Increment
                RxBufferInPtr=0;        //ring buffer
            }
            RxBufferCtr++;
            OSSemPost(RxEventPtr);                //POST TO A SEMAPHORE
        }
    }
}



/**********************************************************************************************************                                          
* Description: This function creates a rx task
* Arguments  : pdata         this pointer points to the string  to be transmitted
* Returns    : OS_NO_ERR    Transmission succeed
                             err            Transmission failed
**********************************************************************************************************/
INT8U SerialPutChar(INT8U pdata)
{
        INT8U err;

        OSSemPend(TxEventPtr,0,&err);
        if(err!=OS_NO_ERR){
            return err;
        }        
        TxBuffer[TxBufferInPtr]=pdata;
        if(TxEventPtr->OSEventCnt == (TX_BUFFER_SIZE-2)){//SerialTxIntEn();
            err = SCI0SR1 ;               // read SCI0SR1 clear flag 
            SCI0DRL=pdata;
            SCI0CR2|=(1<<TCIE0);        //Enable TX interrupt
            
        }
        if(++TxBufferInPtr == TX_BUFFER_SIZE){
            TxBufferInPtr=0;
        }        
    
        return OS_NO_ERR;
}



/**********************************************************************************************************
*                                           Serial Get Tx Char to Transmit
* Description: This puts the Tx data into the transmit shift register
* Arguments  :  none
* Returns    : if 0     the last byte to send
*                others   the value of sending byte
**********************************************************************************************************/
static INT8U SerialGetTxChar(void)
{
    INT8U RetValue;
    OSSemPost(TxEventPtr);
    if(TxEventPtr->OSEventCnt < (TX_BUFFER_SIZE-1)){
        if(++TxBufferOutPtr == TX_BUFFER_SIZE){        //Out Pointer Increment
            TxBufferOutPtr=0;
        }        
        RetValue=TxBuffer[TxBufferOutPtr];
        SCI0DRL=RetValue;            // TCI cleared 
    }else{
        if(++TxBufferOutPtr == TX_BUFFER_SIZE){        //Out Pointer Increment
            TxBufferOutPtr=0;
        }        
        RetValue=0;
        SCI0CR2&=~(1<<TCIE0);            //Disable TX interrupt
    }
    return RetValue;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  SCIISR_Handler
 *  Description:  ���ڷ������̣����ݴ���״̬�Ĵ����ж��Ƿ��ͻ��ǽ����жϲ�������Ӧ����
 *                ���������˴������ڲ����Ƿ��ͺͽ����ж�״̬�Ĵ���ֵ��ͬ�������Ǵ�����
 *                x x x x bug......
 * =====================================================================================
 */
void SCIISR_Handler (void) 
{
 INT8U status  ; 
 status = SCI0SR1  ; 
 if(status&(1<<TCIE0))  // transmit complete 
 {
  SerialGetTxChar();  
 }
  else                  // receive full
 {
  SerialPutRxChar(status); 
 }
}

void SendHex(unsigned char hex) {
  unsigned char temp;
  temp = hex >> 4;
  if(temp < 10) {
    SerialPutChar(temp + '0');
  } else {
    SerialPutChar(temp - 10 + 'A');
  }
  temp = hex & 0x0F;
  if(temp < 10) {
    SerialPutChar(temp + '0');
  } else {
    SerialPutChar(temp - 10 + 'A');
  }
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  RxTask
 *  Description:  ���ݷ������񣬼��һ��ʱ������λ������ϵͳ״̬
 * =====================================================================================
 */

void RxTask(void *pdata)
{
    INT8U *ImageData ; 
   // ImageData = pdata ; 
    SerialInit();
    SerialCfgPort();
    for(;;)
    {
        OSTimeDly(50);
        #ifdef DEBUG_TSL 
        ImageData = ADV  ; 
        
        SerialPutChar('*');
        SerialPutChar('L');
        SerialPutChar('D');
        SendHex(0);
        SendHex(0);
        SendHex(0);
        SendHex(0);
        for(i=0; i<128; i++) {
          SendHex(*ImageData++);
        }
        SerialPutChar(0);
        SerialPutChar('#');
        #endif 
        
        SendHex(SENSORV.SPEED_R)  ; 
        SerialPutChar('#');
    }
}







