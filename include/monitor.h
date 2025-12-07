#ifndef __MONITOR__
#define __MONITOR__
#include <Arduino.h>
#include "LiquidCrystal_I2C.h"
#include "global.h"

void LCD_UpdateBuffer(float temp, float humid);
void LCD_task(void *pvParameters);

#endif