/*
 * dht22.h
 *
 *  Created on: 20 Dec 2025
 *      Author: Tommi
 *      https://cdn.sparkfun.com/assets/f/7/d/9/c/DHT22.pdf
 */

#ifndef INC_DHT22_H_
#define INC_DHT22_H_
#include "main.h"
#include <stdio.h>

typedef struct {

	float temperature;
	float humidity;

} DHT22_Data_t;

DHT22_Data_t read_dht22(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
//static void set_pin_input(GPIO_TypeDef *GPIOx, uint16_t DHT22_pin);
//static void set_pin_output(GPIO_TypeDef *GPIOx, uint16_t DHT22_pin);
void DHT22_Init(TIM_HandleTypeDef *htim);
//static void delay_us(uint16_t us);
#endif /* INC_DHT22_H_ */
