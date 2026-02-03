/*
 * ui.c
 *
 *  Created on: 7 Jan 2026
 *      Author: Tommi
 */

#include "ui.h"
#include <stdint.h>
#include <string.h>
#include "bitmaps.h"
#include "control.h"

#define SCREEN_WIDTH 160
#define FONT_W 7
#define FONT_H 10

#define RAD 8
#define MID 80

void UI_Init() {

	ST7735_Init(3);
	fillScreen(BLACK);

	//box height - font height / 2 + start pos
}

static void UI_DrawButton(uint16_t x, uint16_t y, uint8_t padding, char *text,
		uint16_t color, uint16_t textcolor) {

	uint8_t len = strlen(text);
	uint8_t txtWidth = len * FONT_W;

	uint8_t boxWidth = txtWidth + (2 * padding);
	uint8_t boxHeight = FONT_H + (2 * padding);

	drawRoundRect(x, y, boxWidth, boxHeight, 8, color);
	fillRoundRect(x, y, boxWidth, boxHeight, 8, color);
	ST7735_WriteString(x + padding, y + padding, text, Font_7x10, textcolor,
			color);
}

void UI_DrawBootImage(void) {
	ST7735_DrawImage(30, 18, 100, 93, pepo);
}

void UI_DrawMainMenu(void) {

	fillScreen(BLACK2);
	UI_DrawButton(10, 10, 16, "SETUP", JET, WHITE);
	UI_DrawButton(81, 10, 16, "START", JET, WHITE);
	fillRoundRect(10, 60, 140, 60, 8, JET);
	drawRoundRect(10, 60, 140, 60, 8, JET);

	ST7735_WriteString(16, 70, "Fan", Font_7x10, WHITE, JET);
	ST7735_WriteString(60, 70, "Temp", Font_7x10, WHITE, JET);
	ST7735_WriteString(108, 70, "Humid", Font_7x10, WHITE, JET);

}

void UI_UpdateMainMenu(int fanspd, int temp, float humid) {

	char fanbuf[13];
	char tempbuf[13];
	char humidbuf[13];

	sprintf(fanbuf, "%-5d", fanspd);
	sprintf(tempbuf, "%d", temp);
	sprintf(humidbuf, "%.1f", humid);

	ST7735_WriteString(14, 84, fanbuf, Font_7x10, WHITE, JET);
	ST7735_WriteString(70, 84, tempbuf, Font_7x10, WHITE, JET);
	ST7735_WriteString(110, 84, humidbuf, Font_7x10, WHITE, JET);


}

void UI_DrawSetup(float trg_temp, int trg_time, float new_temp, int new_time) {

	char trg_tempbuf[13];
	char trg_timebuf[13];
	char new_tempbuf[13];
	char new_timebuf[13];


	fillScreen(BLACK2);
	fillRoundRect(4, 4, 38, 44, 8, JET);
	ST7735_WriteString(9, 22, "BACK", Font_7x10, WHITE, JET);

	fillRoundRect(48, 4, 108, 44, 8, JET);
	drawRoundRect(48, 4, 108, 44, 8, WHITE);

	ST7735_WriteString(56, 8, "Curr. targets", Font_7x10, WHITE, JET);
	ST7735_WriteString(56, 22, "Temp:", Font_7x10, WHITE, JET);
	ST7735_WriteString(56, 34, "Time:", Font_7x10, WHITE, JET);

	fillRoundRect(4, 54, 152, 72, 8, JET);
	drawRoundRect(4, 54, 152, 72, 8, WHITE);

	ST7735_WriteStringMiddle(58, "New targets", Font_7x10, WHITE, JET);
	ST7735_WriteString(16, 80, "Temp: ", Font_7x10, WHITE, JET);
	ST7735_WriteString(16, 100, "Time: ", Font_7x10, WHITE, JET);
	fillRoundRect(55, 78, 30, 14, 8, eBLACK);
	fillRoundRect(55, 98, 30, 14, 8, eBLACK);

	UI_UpdateSetup(trg_temp, trg_time, new_temp, new_time);


}

void UI_UpdateSetup(float trg_temp, int trg_time, float new_temp, int new_time) {

	char trg_tempbuf[13];
	char trg_timebuf[13];
	char new_tempbuf[13];
	char new_timebuf[13];

	sprintf(trg_tempbuf, "%.1f C", trg_temp);
	sprintf(trg_timebuf, "%d min", trg_time);
	sprintf(new_tempbuf, "%.0f", new_temp);
	sprintf(new_timebuf, "%d", new_time);

	ST7735_WriteString(92, 22, trg_tempbuf, Font_7x10, WHITE, JET);
	ST7735_WriteString(92, 34, trg_timebuf, Font_7x10, WHITE, JET);
	ST7735_WriteString(63, 81, new_tempbuf, Font_7x10, WHITE, eBLACK);
	ST7735_WriteString(63, 101, new_timebuf, Font_7x10, WHITE, eBLACK);

}

void UI_UpdateSetupTargets(float trg_temp, int trg_time, float new_temp, int new_time) {



	fillRoundRect(55, 78, 30, 14, 8, eBLACK);
	fillRoundRect(55, 98, 30, 14, 8, eBLACK);

}

void UI_DrawErrorScreen(int errortype) {
	fillScreen(BLUESCR_BLUE);
	ST7735_WriteStringMiddle(66, ":(", Font_16x26, WHITE, BLUESCR_BLUE);
	ST7735_WriteString(1, 1, "Your device ran into a problem and needs a restart. We're not collecting any info as this is a very dumb device. Good luck troubleshooting!", Font_7x10, WHITE, BLUESCR_BLUE);

	switch(errortype) {
	case 1:
		ST7735_WriteStringMiddle(100, "FAN_ERROR", Font_11x18, WHITE, BLUESCR_BLUE);
		break;

	case 2:
		ST7735_WriteStringMiddle(100, "OVERHEAT", Font_11x18, WHITE, BLUESCR_BLUE);
		break;

	case 3:
		ST7735_WriteStringMiddle(100, "TEMP_SENSOR", Font_11x18, WHITE, BLUESCR_BLUE);
	}

}

void UI_DrawSelection(DryerState state, int8_t sel, bool isEditing) {

	switch (state) {
	case STATE_MAINMENU:
		drawRoundRect(10, 10, 67, 42, RAD, JET);
		drawRoundRect(81, 10, 67, 42, RAD, JET);

		if (sel == 0) {
			drawRoundRect(10, 10, 67, 42, RAD, WHITE);

		} else if (sel == 1)

		{
			drawRoundRect(81, 10, 67, 42, RAD, WHITE);
		}
		break;

	case STATE_SETUP:
		drawRoundRect(4, 4, 38, 44, RAD, JET);
		drawRoundRect(55, 78, 30, 14, RAD, JET);
		drawRoundRect(55, 98, 30, 14, RAD, JET);

		if (sel == 0)
			drawRoundRect(4, 4, 38, 44, RAD, WHITE);

		if (sel == 1) {
			if (isEditing) {
				drawRoundRect(55, 78, 30, 14, RAD, GREEN);

			} else
				drawRoundRect(55, 78, 30, 14, RAD, WHITE);

		}

		if (sel == 2) {
			if (isEditing) {
				drawRoundRect(55, 98, 30, 14, RAD, GREEN);

			} else
				drawRoundRect(55, 98, 30, 14, RAD, WHITE);

		}

		break;

	}

}



