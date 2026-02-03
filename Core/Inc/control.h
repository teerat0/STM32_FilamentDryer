/*
 * control.h
 *
 *  Created on: 13 Jan 2026
 *      Author: Tommi
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#include <stdint.h>
#include <stdbool.h>

void settings_init(void);
void readTemp(void);

typedef enum {
	ERROR_NONE = 0,
	ERROR_FAN,
	ERROR_OVERHEAT,
	ERROR_SENSOR_LOST
} SysError;

typedef struct {

	volatile bool isHeaterOn;
	volatile bool isFanOn;

	volatile int currTemp;
	volatile float currHumid;
	volatile float targetTemp;
	volatile int targetTime;
	volatile int fanSpeed;

	volatile SysError errorstatus;
} DryerStatus;


extern DryerStatus dryerstatus;

#endif /* INC_CONTROL_H_ */
