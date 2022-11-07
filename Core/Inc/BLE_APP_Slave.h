/*
 * BLE_APP_Slave.h V2
 *
 *For Chip: RYB080E
 *
 *  Created on: Jun 18, 2021
 *      Author: Wayne Wu
 */

#ifndef INC_BLE_APP_SLAVE_H_
#define INC_BLE_APP_SLAVE_H_



#endif /* INC_BLE_APP_SLAVE_H_ */

/* Exported types ------------------------------------------------------------*/
#define Uart_Biffer_Size 65
typedef struct
{
	_Bool sendflag;
	_Bool Revflag;
	//usart transfermit
	unsigned char __attribute__ ((aligned (32))) buffer[Uart_Biffer_Size];
	//char __attribute__ ((aligned (32))) buffer[Uart_Biffer_Size];
	//unsigned char __attribute__ ((aligned (32))) Rbuffer[Uart_RBiffer_Size];
	uint8_t Rbuffer;
	uint8_t RevData[24];
	uint8_t ConnCheck[5];
	uint8_t ACKCheck[6];
	uint8_t RxCount;
	uint32_t sendTimeout;
	uint32_t RevTimeout;
	uint32_t bufferSize;
	char Status;
} USART_BLE;

/* Exported constants --------------------------------------------------------*/
enum
{
	WaitConnecting,
	WaitConnectACK,
	SendData,
};

/* Exported macro ------------------------------------------------------------*/
#define min(a, b) ((a) < (b)) ? (a) : (b)
#define max(a, b) ((a) > (b)) ? (a) : (b)

/* Exported functions prototypes ---------------------------------------------*/
void BLE_USART(UART_HandleTypeDef *huart, ADC_HandleTypeDef *hadc);
void Connect_Rep_Check(void);
void Master_ACK_Check(void);
void Connect_ACK_Check(void);
void Transmit_ADC_data(UART_HandleTypeDef *huart);
void BLE_Status_Init(UART_HandleTypeDef *huart);

/* Private defines -----------------------------------------------------------*/
#define GLED_Port GPIOD
#define RLED_Port GPIOD
#define GLED GPIO_PIN_12
#define RLED GPIO_PIN_13
