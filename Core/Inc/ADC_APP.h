/*
 * ADC_APP.h
 *
 *  Created on: Jun 18, 2021
 *      Author: Wayne Wu
 */

#ifndef INC_ADC_APP_H_
#define INC_ADC_APP_H_



#endif /* INC_ADC_APP_H_ */

/* Exported types ------------------------------------------------------------*/
#define ADC_Buffer_Ch 2
typedef struct
{
	uint16_t BufferValue[ADC_Buffer_Ch];
	uint32_t BufferValue_Sum[ADC_Buffer_Ch];
	uint16_t BufferValue_Avg[ADC_Buffer_Ch];
	uint16_t Vol[ADC_Buffer_Ch];
} ADC1_t;

/* Exported functions prototypes ---------------------------------------------*/
void ADC1_Conv(ADC_HandleTypeDef *hadc);
