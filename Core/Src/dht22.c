/*
 * dht22.c
 *
 *  Created on: 20 Dec 2025
 *      Author: Tommi Räisänen
 *      https://cdn.sparkfun.com/assets/f/7/d/9/c/DHT22.pdf
 */
#include "dht22.h"

static TIM_HandleTypeDef* _dht_timer = NULL;


static void delay_us(uint16_t us)
{
	if(_dht_timer == NULL)
	{
		return;
	}

	__HAL_TIM_SET_COUNTER(_dht_timer, 0);
	while(__HAL_TIM_GET_COUNTER(_dht_timer) < us);

}


void DHT22_Init(TIM_HandleTypeDef *htim)
{
	_dht_timer = htim;

}



static void set_pin_output(GPIO_TypeDef *GPIOx, uint16_t DHT22_pin)
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  GPIO_InitStruct.Pin = DHT22_pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

}

static void set_pin_input(GPIO_TypeDef *GPIOx, uint16_t DHT22_pin)
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  GPIO_InitStruct.Pin = DHT22_pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

}

DHT22_Data_t read_dht22(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	DHT22_Data_t result = {0.0f, 0.0f};

	uint8_t data[5] = {0};
	uint32_t last_tick = 0;
	uint16_t timeout;

	if(_dht_timer == NULL)
		{
		return result;
		}

	/* From DHT22 datasheet:
	 *
	 * Data-bus's free status is high voltage level. When communication between MCU and DHT22 begin, program of
       MCU will transform data-bus's voltage level from high to low level and this process must beyond at least 1ms to
	   ensure DHT22 could detect MCU's signal, then MCU will wait 20-40us for DHT22's response.
	 * */


	// ** Step 1: initiate communication **
	set_pin_output(GPIOx, GPIO_Pin);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
	delay_us(15000);

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
	delay_us(30);
	// ** Step 1 done **
	// ** Step 2: listen for sensors response **
	set_pin_input(GPIOx, GPIO_Pin);

	//We are now listening to the sensor. It should pull the line DOWN for about 80us, so lets wait until the middle point and read if it has done so.
	//If not, something has gone wrong so return
	delay_us(40);
	if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)
	{
		return result;
	}

	//After, it should pull the line UP for about 80us, so lets wait until the middle point and read if it has done so.
	//If not, something has gone wrong so return
	delay_us(80);
	if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
	{
		return result;
	}


	//Next, actual data trasfer starts when the line is pulled low. Wait until that happens. Shouldnt take loong so timeout variable ensures that its not stuck

	timeout = 10000;

	while(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)
	{
		if(timeout-- == 0)
		{
			return result;
		}
	}

	// ** Step 2 done **



	// ** Step 3: read 40 bits from sensor **

	for(int i = 0; i < 40; i++)
	{
		timeout = 10000;

		// To determine whether the bit coming from DHT22 is a 1 or 0, we have to measure the HIGH pulses duration.
		// if pulse = ~70us = bit 1
		// if pulse = ~26-28 = bit 0

		// Start by waiting for the line to go up.

		while(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET){
			if(timeout-- == 0)
			{
				return result;
			}
		}

		// The line is pulled up. Lets zero the timer counter and wait until it goes down again, then measure how long the pulse was.

		timeout = 10000;
		__HAL_TIM_SET_COUNTER(_dht_timer, 0);

		while(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)
		{
			if(timeout-- == 0)
			{
				return result;
			}
		}

		last_tick = __HAL_TIM_GET_COUNTER(_dht_timer);

		if(last_tick > 50)
		{
			data[i / 8] |= (1 << (7 - (i % 8)));

			//data[i / 8]: which byte of the 5 incoming bytes are we accessing, for example if i = 0, 1, 2... 7, i / 8 = 0 = first byte. if i = 8, 9, 10... 15, i / 8 = 1 = second byte...
			// |=: OR operator
			//1 << (7 - (i % 8): 7- to reverse order (DHT22 sends data MSB first): for example, i = 2, 1 << (7 - 2) = 1 << 5, take 1 and shift it 5 times to the left = 00100000
 		}

		// ** Step 3 done **

		// ** Step 4: Checksum **
		// Verify the data and make sure there are no overflows

	}

	if(data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
	{

		// Join 8 bit high and low bytes together into 16 bit variables
		uint16_t humidity = (data[0] << 8) | data[1];
		uint16_t temperature = data[2] << 8 | data[3];

		result.humidity = humidity / 10.0f;

		//DHT22 sends humidity and temperature data multiplied by 10. For example, 23.3c comes as 233. Divide by 10 to get float

		// check if negative temperature
		if(temperature & 0x8000) //check if first bit of 16 bit temperature variable is 1 or 0, indicating negative or positive value
		{
			//if true (negative value) remove the flag, and * -1 to make negative result
			temperature &= 0x7FFF;
			result.temperature = (temperature / 10.0f) * -1;
		} else {
			result.temperature = temperature / 10.0f;
		}

	}

	return result;
}
