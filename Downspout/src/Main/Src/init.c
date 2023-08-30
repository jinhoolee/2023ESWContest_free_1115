#include "init.h"
#include "motor.h"

#include "custom_lcd.h"
#include "custom_switch.h"
#include "custom_filesystem.h"

void Init() {
	Custom_LCD_Init();	// LCD 사용 전 초기화 함수

	Custom_FileSystem_Load();	// 플래시 정보 호출

	Custom_LCD_Backlight(true);
	Custom_LCD_Clear();
	Custom_LCD_Printf(" WELCOME!");
	Custom_Delay_ms(500);
	Custom_LCD_Clear();

	int8_t number = 0;

	while (true) {
		Custom_LCD_Clear();
		switch (number) {
			case 0:
				Custom_LCD_Printf("START/1RAIN");
				break;
		}

		uint8_t sw;
		while (!(sw = Custom_Switch_Read()));

		if (sw == CUSTOM_SW_1) number--;
		if (sw == CUSTOM_SW_2) number++;
		if (number > 0) number = 0;
		if (number < 0) number = 0;
		if (sw == CUSTOM_SW_BOTH) {
			Custom_LCD_Clear();
			switch (number) {
				case 0:
					Downspout();
					break;

			}
		}
	}
}
