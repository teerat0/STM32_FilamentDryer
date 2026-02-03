/*
 * input.c
 *
 *  Created on: 7 Jan 2026
 *      Author: Tommi
 */

#include "input.h"
#include <stdint.h>
#include "main.h"

extern TIM_HandleTypeDef htim2;

static volatile int8_t rotary_event = 0;

static uint32_t prev_counter = 0;

static volatile uint8_t button_event = 0;
extern volatile uint32_t rpm_pulses;


int8_t getRotary() {

	uint32_t now = __HAL_TIM_GET_COUNTER(&htim2);

	int32_t diff = (int32_t)(now - prev_counter);

	diff /= 4;

	if(diff != 0)
	{
		prev_counter = now;
	}

	return (int8_t)diff;
}

uint8_t getButton() {
	int8_t v = button_event;
	button_event = 0;
	return v;

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	uint32_t now = HAL_GetTick();

	if (GPIO_Pin == ROT_BUT_Pin) {
		static uint32_t last_but_time = 0;
		if (now - last_but_time >= 150) {
			last_but_time = now;
			button_event++;
		}

	}

	else if (GPIO_Pin == FAN_RPM_Pin)
	{
		rpm_pulses++;
	}
}

