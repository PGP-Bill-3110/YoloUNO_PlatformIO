#ifndef NEO_HUMIDITY_H
#define NEO_HUMIDITY_H

#include "neo_blinky.h"   
#include <Arduino.h>
#include "global.h"       
#include <LiquidCrystal_I2C.h>
#include "DHT20.h"
#include "global.h"
#include <Adafruit_NeoPixel.h>

void neo_humidity_task(void *pvParameters);

#endif // NEO_HUMIDITY_H
