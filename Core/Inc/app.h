/*
 * logic.h
 *
 *  Created on: 8 Jan 2026
 *      Author: Tommi
 */

#ifndef INC_APP_H_
#define INC_APP_H_

typedef enum {
	STATE_MAINMENU, STATE_SETUP, STATE_DRYING, STATE_ERROR
} DryerState;


void Run_App();

#endif /* INC_APP_H_ */
