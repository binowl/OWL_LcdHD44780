/*
 * OWL_LcdHD44780.c
 *
 *  Copyright BinOwl 2018
 *
 *  Created on: 07.09.2018
 *      Author: BinOwl
 */

#include "OWL_LcdHD44780.h"

__STATIC_INLINE void _OWL_LcdHD44780Write4bit(LCDHD44780_t *lcd, uint8_t data) {
	HAL_GPIO_WritePin(lcd->_LCD_D4_GPIO_Port, lcd->_LCD_D4_Pin, data & 0x1);
	HAL_GPIO_WritePin(lcd->_LCD_D5_GPIO_Port, lcd->_LCD_D5_Pin, data & 0x2);
	HAL_GPIO_WritePin(lcd->_LCD_D6_GPIO_Port, lcd->_LCD_D6_Pin, data & 0x4);
	HAL_GPIO_WritePin(lcd->_LCD_D7_GPIO_Port, lcd->_LCD_D7_Pin, data & 0x8);
	HAL_GPIO_WritePin(lcd->_LCD_E_GPIO_Port, lcd->_LCD_E_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(lcd->_LCD_E_GPIO_Port, lcd->_LCD_E_Pin, GPIO_PIN_RESET);
	OWL_DelayMicroseconds(50); //Command execution
}

__STATIC_INLINE void _OWL_LcdHD44780Write8bit(LCDHD44780_t *lcd, uint8_t data) {
	_OWL_LcdHD44780Write4bit(lcd, data >> 4);
	_OWL_LcdHD44780Write4bit(lcd, data);
}

/**
 * Initialize LCD HD44780 Display 4bit
 *
 * @param lcd
 */
void OWL_LcdHD44780Init(LCDHD44780_t *lcd) {
	/*Pin reset*/
	HAL_GPIO_WritePin(lcd->_LCD_E_GPIO_Port, lcd->_LCD_E_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->_LCD_RS_GPIO_Port, lcd->_LCD_RS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->_LCD_D4_GPIO_Port, lcd->_LCD_D4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->_LCD_D5_GPIO_Port, lcd->_LCD_D5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->_LCD_D6_GPIO_Port, lcd->_LCD_D6_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->_LCD_D7_GPIO_Port, lcd->_LCD_D7_Pin, GPIO_PIN_RESET);

	/*Initializing by instruction*/
	HAL_Delay(100);
	_OWL_LcdHD44780Write4bit(lcd, 0x3);
	HAL_Delay(10);
	_OWL_LcdHD44780Write4bit(lcd, 0x3);
	_OWL_LcdHD44780Write4bit(lcd, 0x3);
	_OWL_LcdHD44780Write4bit(lcd, 0x2);
	_OWL_LcdHD44780Write8bit(lcd, LCDI_FUNCTION_SET | LCDI_FUNCTION_SET_N); //Set display parameters
	_OWL_LcdHD44780Write8bit(lcd, LCDI_DISPLAY_ON_OFF | LCDI_DISPLAY_ON_OFF_D); //Turn display On
	_OWL_LcdHD44780Write8bit(lcd, LCDI_DISPLAY_CLEAR); //Clear display - This command fills all screen with 0x20 (space) and sets cursor to 0
	HAL_Delay(2);
	_OWL_LcdHD44780Write8bit(lcd, LCDI_ENTRY_MODE | LCDI_ENTRY_MODE_ID); //Set entry mode
}

/**
 * Clear entire display
 *
 * @param lcd
 */
void OWL_LcdHD44780Clear(LCDHD44780_t *lcd) {
	_OWL_LcdHD44780Write8bit(lcd, LCDI_DISPLAY_CLEAR);
	HAL_Delay(2);
}

/**
 * Display string on LCD on current cursor position
 *
 * @param lcd
 * @param str
 */
void OWL_LcdHD44780DisplayString(LCDHD44780_t *lcd, char *str) {
	HAL_GPIO_WritePin(lcd->_LCD_RS_GPIO_Port, lcd->_LCD_RS_Pin, GPIO_PIN_SET);
	do {
		_OWL_LcdHD44780Write8bit(lcd, (uint8_t) *str++);
	} while (*str != '\0');
	HAL_GPIO_WritePin(lcd->_LCD_RS_GPIO_Port, lcd->_LCD_RS_Pin, GPIO_PIN_RESET);
}

/**
 * Display character on LCD on current cursor position
 *
 * @param lcd
 * @param c
 */
void OWL_LcdHD44780DisplayChar(LCDHD44780_t *lcd, char c) {
	HAL_GPIO_WritePin(lcd->_LCD_RS_GPIO_Port, lcd->_LCD_RS_Pin, GPIO_PIN_SET);
	_OWL_LcdHD44780Write8bit(lcd, c);
	HAL_GPIO_WritePin(lcd->_LCD_RS_GPIO_Port, lcd->_LCD_RS_Pin, GPIO_PIN_RESET);
}

/**
 * Set cursor position
 *
 * @param lcd
 * @param x
 * @param y
 */
void OWL_LcdHD44780SetCursorPos(LCDHD44780_t *lcd, uint8_t x, uint8_t y) {
	uint8_t base_addr = 0;
	switch (lcd->_LCDHD44780_LCDType) {
	case LCDHD44780_2x16:
		switch (y) {
		case 0:
			base_addr = 0;
			break;
		case 1:
			base_addr = 0x40;
			break;
		}
		break;
	case LCDHD44780_4x20:
		switch (y) {
		case 0:
			base_addr = 0;
			break;
		case 1:
			base_addr = 0x40;
			break;
		case 2:
			base_addr = 0x14;
			break;
		case 3:
			base_addr = 0x54;
			break;
		}
		break;
	}
	_OWL_LcdHD44780Write8bit(lcd, LCDI_DDRAM_ADDRESS_SET | (base_addr + x));
}

/**
 * Create new character
 *
 * @param lcd
 * @param charCode
 * @param charDef[8]
 */
void OWL_LcdHD44780CreateChar(LCDHD44780_t *lcd, uint8_t charCode,
		uint8_t charDef[8]) {
	_OWL_LcdHD44780Write8bit(lcd, LCDI_CGRAM_ADDRESS_SET | charCode << 3);
	HAL_GPIO_WritePin(lcd->_LCD_RS_GPIO_Port, lcd->_LCD_RS_Pin, GPIO_PIN_SET);
	for (uint8_t line = 0; line < 8; line++) {
		_OWL_LcdHD44780Write8bit(lcd, charDef[line]);
	}
	HAL_GPIO_WritePin(lcd->_LCD_RS_GPIO_Port, lcd->_LCD_RS_Pin, GPIO_PIN_RESET);
	_OWL_LcdHD44780Write8bit(lcd, LCDI_DISPLAY_CLEAR);
	HAL_Delay(2);
}
