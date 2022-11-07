/*
 * BLE_APP_Slave.h V2.1
 *
 *For Chip: RYB080E
 *
 *  Created on: Jun 24, 2021
 *      Author: Wayne Wu
 *
 20210624----V2.1
 *Add 1 channel ADC for second sensor.
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "stdio.h"

/* Private typedef -----------------------------------------------------------*/
USART_BLE USARTBLE;
ADC1_t ADC_1;

/* Private variables ---------------------------------------------------------*/
char Rev_SecTimeout_count;


//void BLE_USART(UART_HandleTypeDef *huart)
void BLE_USART(UART_HandleTypeDef *huart, ADC_HandleTypeDef *hadc)
{
	switch(USARTBLE.Status)
	    {

	    	case WaitConnecting:
	    		HAL_GPIO_WritePin(RLED_Port, RLED , GPIO_PIN_SET);
	    		while(!USARTBLE.Revflag);
	    		Connect_Rep_Check();
	    		break;

	    	case WaitConnectACK:
	    		while(!USARTBLE.Revflag);
	    		Connect_ACK_Check();
	    		USARTBLE.sendflag = 0;
	    		break;


	    	case SendData:           
            	if(USARTBLE.sendflag ==1)
            	{
            		Transmit_ADC_data(huart);

					//Check the connect is keep by Master_ACK_Check
					while(!USARTBLE.Revflag)
					{
						if(Rev_SecTimeout_count==5)
						{
							Rev_SecTimeout_count=0;
							//When 5 second timeout that not receive any data, then turn ststus to WaitConnecting.
							USARTBLE.Status = WaitConnecting;
						}
					}
					Master_ACK_Check();
            	}
	    		break;
	    }
}

void Connect_Rep_Check(void)
{
	memset(USARTBLE.ConnCheck, 0, strlen(USARTBLE.ConnCheck) ); //clear connect response string
	strncpy(USARTBLE.ConnCheck, USARTBLE.RevData, 5); //copy receive data at front 5 byte
	char ConSucceed[] = "+MTU:";
	if(strcmp( USARTBLE.ConnCheck, ConSucceed ) == 0)
	{
		USARTBLE.Status = WaitConnectACK;
	}
	else
	{
		USARTBLE.Status = WaitConnecting;
	}
	memset( USARTBLE.RevData, 0, strlen(USARTBLE.RevData) ); //clear Receive data
	USARTBLE.Revflag = 0;
}
void Connect_ACK_Check(void)
{
	memset(USARTBLE.ConnCheck, 0, strlen(USARTBLE.ConnCheck) ); //clear connect response string
	strncpy(USARTBLE.ConnCheck, USARTBLE.RevData, 6); //copy receive data at front 5 byte
	char ConSucceed[] = "ConACK";
	if(strcmp( USARTBLE.ConnCheck, ConSucceed ) == 0)
	{
		USARTBLE.Status = SendData;
	}
	else
	{
		USARTBLE.Status = WaitConnecting;
	}
	memset( USARTBLE.RevData, 0, strlen(USARTBLE.RevData) ); //clear Receive data
	USARTBLE.Revflag = 0;
}

void Master_ACK_Check(void)
{
	Rev_SecTimeout_count=0; //reset receive timeout
	memset(USARTBLE.ACKCheck, 0, strlen(USARTBLE.ACKCheck) ); //clear connect response string
	strncpy(USARTBLE.ACKCheck, USARTBLE.RevData, 6); //copy receive data at front 5 byte
	char MasterACK[] = "RevACK";
	if(strcmp( USARTBLE.ACKCheck, MasterACK ) == 0)
	{
		USARTBLE.Status = SendData;
	}
	else
	{
		USARTBLE.Status = WaitConnecting;
	}
	memset( USARTBLE.RevData, 0, strlen(USARTBLE.RevData) ); //clear Receive data
	USARTBLE.Revflag = 0;
}

void Transmit_ADC_data(UART_HandleTypeDef *huart)
{
    // ADC data is ready, start transmit data , flash Green LED
    HAL_GPIO_WritePin(GLED_Port, GLED , GPIO_PIN_SET);
    HAL_Delay(0.1);
    HAL_GPIO_WritePin(GLED_Port, GLED , GPIO_PIN_RESET);
    
//    if(ADC_1.Vol[0]>99 && ADC_1.Vol[0]<1000)
//    	sprintf(USARTBLE.buffer,"0%d\r\n",ADC_1.Vol[0]);
//    else if(ADC_1.Vol[0]>9 && ADC_1.Vol[0]<100)
//    	sprintf(USARTBLE.buffer,"00%d\r\n",ADC_1.Vol[0]);
//    else if(ADC_1.Vol[0]<10)
//        sprintf(USARTBLE.buffer,"000%d\r\n",ADC_1.Vol[0]);
//    else
    	sprintf(USARTBLE.buffer,"%d, %d\r\n",ADC_1.Vol[0], ADC_1.Vol[1]);
    
    USARTBLE.bufferSize = min(Uart_Biffer_Size, strlen(USARTBLE.buffer));
    USARTBLE.sendTimeout = 50 ;
    /**
    * @param huart   UART handle.
    * @param pData   Pointer to data buffer.
    * @param Size    Amount of data to be received.
    * @param Timeout Timeout duration.(ms)
    */
    while(HAL_UART_Transmit(huart, &USARTBLE.buffer, USARTBLE.bufferSize, USARTBLE.sendTimeout )!=HAL_OK);
    memset( USARTBLE.buffer, 0, strlen(USARTBLE.buffer) ); //clear send data
    //transmit LED status
    HAL_GPIO_WritePin(RLED_Port, RLED , GPIO_PIN_SET);
    HAL_Delay(0.1);
    HAL_GPIO_WritePin(RLED_Port, RLED , GPIO_PIN_RESET);
    
    USARTBLE.sendflag = 0;
    Rev_SecTimeout_count++;
}

void BLE_Status_Init(UART_HandleTypeDef *huart)
{
	//init BLE Status
	USARTBLE.Status = WaitConnecting;

  	//initial BLE receive interrupt flag
  	while(HAL_UART_Receive_IT(huart ,&USARTBLE.Rbuffer,1)!=HAL_OK);
}
