/*
 * control.c
 *
 *  Created on: 13 Jan 2026
 *      Author: Tommi
 */

#include "control.h"
#include "main.h"
#include "dht22.h"
#define FAN_GRACE_PERIOD 20//timer ticks
#define SENSOR_GRACE_PERIOD 50//timer ticks
DryerStatus dryerstatus;

static int FanSafetyCounter = 0;
static int SensorSafetyCounter = 0;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
volatile uint32_t rpm_pulses = 0;
volatile int curr_RPM = 0;
volatile int curr_Temp= 0;


static int getFan()
{
	static int rpm = 0;
	uint32_t now = HAL_GetTick();
	static uint32_t prev = 0;

	if (now - prev >= 1000) {
		prev = now;
		rpm = rpm_pulses * 60 / 2;
		rpm_pulses = 0;

	}

	return rpm;
}

static void setFan(uint16_t speed)
{
	if(speed > 100) speed = 100;
	if(speed < 20) speed = 20;
	uint16_t pwm = speed * 36;
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm);

}

static int getTemp()
{

	return dryerstatus.currTemp;
}

static void setHeater(uint16_t power)
{
	if(power <= 0) power = 0;
	if(power > 100) power = 100;
	uint16_t pwm = 1000 - 10 * power;
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwm);

}


void readTemp(void)
{

	uint32_t now = HAL_GetTick();
	static uint32_t prev = 0;

	if (now - prev >= 2000) {
		prev = now;
		DHT22_Data_t dht22 = read_dht22(GPIOB, GPIO_PIN_0);
		dryerstatus.currTemp = dht22.temperature;
		dryerstatus.currHumid = dht22.humidity;
	}



}

void settings_init(void) {

	dryerstatus.isHeaterOn = false;
	dryerstatus.isFanOn = false;
	dryerstatus.targetTemp = 50.0f;
	dryerstatus.targetTime = 60;
	dryerstatus.errorstatus = ERROR_NONE;
	setFan(40);
	setHeater(0);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM10) {

		curr_RPM = getFan();
		curr_Temp = getTemp();
		dryerstatus.fanSpeed = curr_RPM;


		if(curr_RPM < 600)
		{

			if(FanSafetyCounter < FAN_GRACE_PERIOD)
			{
				FanSafetyCounter++;
			}
			else {
					dryerstatus.errorstatus = ERROR_FAN;
				}

			}
		else {
			FanSafetyCounter = 0;
		}


		if(curr_Temp > 40)
		{
			dryerstatus.errorstatus = ERROR_OVERHEAT;
		}


		if(curr_Temp <= 0)
		{
			if(SensorSafetyCounter < SENSOR_GRACE_PERIOD)
			{
				SensorSafetyCounter++;
			} else {


			dryerstatus.errorstatus = ERROR_SENSOR_LOST;
			}
		} else SensorSafetyCounter = 0;


		if(dryerstatus.errorstatus != ERROR_NONE)
		{

			setFan(100);
			setHeater(0);
		}


	}

}

