#include <stdlib.h>
#include "menu.h"
#include "key.h"
#include "OLED.h"

uint8_t menu_cursor = 0; 				//当前光标行

uint8_t speed = 0;
uint8_t direction = 0; 
float kp = 0.8f;
float ki = 0.1f;
float kd = 0.5f;

void PIDstring (int row, float num)		//转换PID参数为字符串
{
    int ten = (int)(num * 10 + 0.5f);   //四舍五入，确保精度
    char str[5];
    str[0] = '0' + ten / 10;
    str[1] = '.';
    str[2] = '0' + ten % 10;
    str[3] = '\0';
    OLED_ShowString(row, 12, str, OLED_8X16);
}

void Menu_MoveCursor(int8_t dir, uint8_t min_row, uint8_t max_row) {		//移动光标
    uint8_t new_cursor = menu_cursor;

    if (dir == -1) { 
        if (new_cursor > min_row) {
            new_cursor--;
        } else {
            new_cursor = max_row;
        }
    } else if (dir == 1) { 
        if (new_cursor < max_row) {
            new_cursor++;
        } else {
            new_cursor = min_row;
        }
    }
    OLED_ShowChar(menu_cursor, 1, ' ', OLED_8X16);      //擦除旧光标
    OLED_ShowChar(new_cursor, 1, '>', OLED_8X16);       //绘制新光标
    menu_cursor = new_cursor;
}
void DrawPID() {
    OLED_Clear();
    OLED_ShowString(1, 1, "PID", OLED_8X16);              	 
    OLED_ShowString(2, 1, ">kp", OLED_8X16);                PIDstring(2, kp);
    OLED_ShowString(3, 1, " ki", OLED_8X16);                PIDstring(3, ki);
    OLED_ShowString(4, 1, " kd", OLED_8X16);                PIDstring(4, kd);
    menu_cursor = 2;
}
static uint8_t edit = 0; //编辑模式
void Menu_Update(void) {
    if (Key_Check(0, KEY_EVENT_SINGLE)) {	  //上
        if (edit) {							  //已进入编辑模式
			if (menu_cursor == 2) { kp += 0.1f; PIDstring(2, kp); }
            else if (menu_cursor == 3) { ki += 0.1f; PIDstring(3, ki); }
            else if (menu_cursor == 4) { kd += 0.1f; PIDstring(4, kd); }
        }else{							   //未进入编辑模式：移动光标
				Menu_MoveCursor(-1, 2, 4);
        }
    }
	if (Key_Check(0, KEY_EVENT_REPEAT)){
		if(edit)
		{if (menu_cursor == 2) { kp += 0.1f; PIDstring(2, kp); }
         else if (menu_cursor == 3) { ki += 0.1f; PIDstring(3, ki); }
         else if (menu_cursor == 4) { kd += 0.1f; PIDstring(4, kd); }
		}
	}

    if (Key_Check(1, KEY_EVENT_SINGLE)) {		    //下
        if (edit) {									//已进入编辑模式
                if (menu_cursor == 2) { kp -= 0.1f; PIDstring(2, kp); }
                else if (menu_cursor == 3) { ki -= 0.1f; PIDstring(3, ki); }
                else if (menu_cursor == 4) { kd -= 0.1f; PIDstring(4, kd); }
        } else {									//未进入编辑模式：移动光标
                Menu_MoveCursor(1, 2, 4);
        }
    }
	if (Key_Check(1, KEY_EVENT_REPEAT)){			//REPEAT状态处理
		if(edit)
		{if (menu_cursor == 2) { kp -= 0.1f; PIDstring(2, kp); }
         else if (menu_cursor == 3) { ki -= 0.1f; PIDstring(3, ki); }
         else if (menu_cursor == 4) { kd -= 0.1f; PIDstring(4, kd); }
		}
	}
    //确认键
    if (Key_Check(2, KEY_EVENT_SINGLE)) {
            edit = !edit;
            OLED_ShowChar(1, 16, edit ? 'E' : ' ', OLED_8X16);
    }

}
void Menu_Init(void) {		//菜单初始化
	DrawPID();
}
