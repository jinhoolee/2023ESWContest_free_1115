#include "custom_delay.h"
#include "main.h"
#include <stdbool.h>
#include <stdio.h>

// Commands
#define LCD_CLEAR_DISPLAY	0x01
#define LCD_DISPLAY_CONTROL	0x08
#define LCD_FUNCTION_SET	0x20

// Bit fields
#define LCD_DISPLAY_D	0x04	// Display control
#define LCD_FUNCTION_N	0x08	// Function set control

// Public functions
void Custom_LCD_Init();
void Custom_LCD_Printf(const char *format, ...);
void Custom_LCD_SetCursor(uint8_t row, uint8_t col);
void Custom_LCD_Clear(void);
void Custom_LCD_ShiftToRight(uint8_t num);
void Custom_LCD_ShiftToLeft(uint8_t num);
void Custom_LCD_Backlight(bool turnOn);
