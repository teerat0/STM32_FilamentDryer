/*
 * ui.h
 *
 *  Created on: 7 Jan 2026
 *      Author: Tommi
 */

#ifndef INC_UI_H_
#define INC_UI_H_

#include <app.h>
#include "ST7735.h"
#include "GFX_FUNCTIONS.h"
#include "fonts.h"

void UI_Init(void);
void UpdateMenu(int8_t index);
void UI_DrawMainMenu(void);
void UI_DrawErrorScreen(int errortype);
void UI_UpdateMainMenu(int fanspd, int temp, float humid);
void UI_DrawSetup(float trg_temp, int trg_time, float new_temp, int new_time);
void UI_UpdateSetup(float trg_temp, int trg_time, float new_temp, int new_time);
void UI_DrawSelection(DryerState state, int8_t sel, bool isEditing);
void UI_DrawBootImage(void);
#endif /* INC_UI_H_ */
