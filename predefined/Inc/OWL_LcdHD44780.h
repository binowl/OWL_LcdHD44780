/*
 * OWL_LcdHD44780.h
 *
 *  Copyright BinOwl 2018
 *
 *  Created on: 07.09.2018
 *      Author: BinOwl
 */

/* Change this include to your MCU */
#include "stm32f3xx_hal.h"

#include "OWL_DelayMicroseconds.h"

/* PINs */
#define LCD_E_Pin GPIO_PIN_5
#define LCD_E_GPIO_Port GPIOC
#define LCD_D5_Pin GPIO_PIN_6
#define LCD_D5_GPIO_Port GPIOC
#define LCD_D6_Pin GPIO_PIN_8
#define LCD_D6_GPIO_Port GPIOC
#define LCD_D7_Pin GPIO_PIN_9
#define LCD_D7_GPIO_Port GPIOC
#define LCD_D4_Pin GPIO_PIN_8
#define LCD_D4_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_9
#define LCD_RS_GPIO_Port GPIOB

/* Instructions */
#define LCDI_DISPLAY_CLEAR 0x1
#define LCDI_RETURN_HOME 0x2

#define LCDI_ENTRY_MODE 0x4
#define LCDI_ENTRY_MODE_ID 0x2 //1 - Increment; 0 - Decrement
#define LCDI_ENTRY_MODE_S 0x1 //1 - Display shift

#define LCDI_DISPLAY_ON_OFF 0x8
#define LCDI_DISPLAY_ON_OFF_D 0x4 //1 - On; 0 - Off Entire Display
#define LCDI_DISPLAY_ON_OFF_C 0x2 //Cursor On Off
#define LCDI_DISPLAY_ON_OFF_B 0x1 //Blinking of cursor position

#define LCDI_CURSOR_DISPLAY_SHIFT 0x10
#define LCDI_CURSOR_DISPLAY_SHIFT_SC 0x8 //1 - Display shift; 0 - Cursor move
#define LCDI_CURSOR_DISPLAY_SHIFT_RL 0x4 //1 - Right; 0 - Left

#define LCDI_FUNCTION_SET 0x20
#define LCDI_FUNCTION_SET_DL 0x10 //1 - 8 bits; 0 - 4 bits
#define LCDI_FUNCTION_SET_N 0x8 //1 - 2 lines; 0 - 1 line
#define LCDI_FUNCTION_SET_F 0x4 //1 - 5x10 dots; 0 - 5x8 dots

#define LCDI_CGRAM_ADDRESS_SET 0x40

#define LCDI_DDRAM_ADDRESS_SET 0x80

/* typedefs */
typedef enum {
	LCDHD44780_2x16 = 0U, LCDHD44780_4x20,
} LCDHD44780_LCDType;

/* Definitions */
LCDHD44780_LCDType _LCDHD44780_LCDType;

void OWL_LcdHD44780Init(LCDHD44780_LCDType lcd_type);
void OWL_LcdHD44780Clear();
void OWL_LcdHD44780DisplayString(char *str);
void OWL_LcdHD44780DisplayChar(char c);
void OWL_LcdHD44780SetCursorPos(uint8_t x, uint8_t y);
void OWL_LcdHD44780CreateChar(uint8_t charCode, uint8_t charDef[8]);
