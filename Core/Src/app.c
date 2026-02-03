/*
 * logic.c
 *
 *  Created on: 8 Jan 2026
 *      Author: Tommi
 */

#include <app.h>
#include <stdio.h>
#include "input.h"
#include "ui.h"
#include "control.h"
#include "main.h"


static DryerState currState = STATE_MAINMENU;
static int8_t menuSelection = 0;

static bool isFirstEntry = true;
static bool Editing = false;

static float new_temp;
static int new_time;




static void stateChange(DryerState newState) {
	currState = newState;
	isFirstEntry = true;
}


void Run_App() {

	int8_t enc = getRotary();
	uint8_t butt = getButton(); //butt koska mä voin

	if(dryerstatus.errorstatus != ERROR_NONE && currState != STATE_ERROR)
	{
		stateChange(STATE_ERROR);
	}

	switch (currState) {

	case STATE_MAINMENU:

		if (isFirstEntry) {
			UI_DrawMainMenu();
			UI_DrawSelection(currState, menuSelection, false);
			isFirstEntry = false;
		}

		if (enc != 0) {
			menuSelection += enc;
			if (menuSelection < 0)
				menuSelection = 0;
			if (menuSelection > 1)
				menuSelection = 1;

			UI_DrawSelection(currState, menuSelection, false);

		}

		if (butt) {
			if (menuSelection == 0) {
				stateChange(STATE_SETUP);
			} else if (menuSelection == 1) {
				//stateChange(STATE_ERROR);
			}
		}

		UI_UpdateMainMenu(dryerstatus.fanSpeed, dryerstatus.currTemp, dryerstatus.currHumid);
		break;

	case STATE_SETUP:
		if (isFirstEntry) {
			new_temp = dryerstatus.targetTemp;
			new_time = dryerstatus.targetTime;

			UI_DrawSetup(dryerstatus.targetTemp, dryerstatus.targetTime, new_temp, new_time);
			UI_DrawSelection(currState, menuSelection, false);
			isFirstEntry = false;
			Editing = false;
		}

		if (enc != 0) {
			if (Editing) {
				if (menuSelection == 1) {
					new_temp += enc;
					if (new_temp < 30)
						new_temp = 30;
					if (new_temp > 60)
						new_temp = 60;
					UI_UpdateSetup(dryerstatus.targetTemp, dryerstatus.targetTime, new_temp, new_time);
				}

				if (menuSelection == 2) {
					new_time += enc;
					if (new_time < 10)
						new_time = 10;
					if (new_time > 300)
						new_time = 300;
					UI_UpdateSetup(dryerstatus.targetTemp, dryerstatus.targetTime, new_temp, new_time);
				}

			} else {

				menuSelection += enc;
				if (menuSelection < 0)
					menuSelection = 0;
				if (menuSelection > 2)
					menuSelection = 2;

				UI_DrawSelection(currState, menuSelection, false);
			}
		}

		if (butt) {
			if (Editing) {
				if (menuSelection == 1) // save new temp
						{
					dryerstatus.targetTemp = new_temp;
					UI_UpdateSetup(dryerstatus.targetTemp, dryerstatus.targetTime, new_temp, new_time);
				}

				if (menuSelection == 2) {
					dryerstatus.targetTime = new_time;
					UI_UpdateSetup(dryerstatus.targetTemp, dryerstatus.targetTime, new_temp, new_time);
				}

				Editing = false;
				UI_DrawSelection(currState, menuSelection, false);

			} else {

				if (menuSelection == 0) // back button pressed, back to menu
						{
					stateChange(STATE_MAINMENU);
				}
				if (menuSelection == 1 || menuSelection == 2) // Temp or time editing button pressed
						{
					UI_DrawSelection(currState, menuSelection, true);
					Editing = true;
				}

			}
		}

		break;

	case STATE_ERROR:

		if (isFirstEntry) {
			UI_DrawErrorScreen(dryerstatus.errorstatus);
			isFirstEntry = false;
		}




	}
}

