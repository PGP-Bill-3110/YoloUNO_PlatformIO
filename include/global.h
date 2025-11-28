#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <LiquidCrystal_I2C.h>
#include "DHT20.h"
extern float glob_temperature;
extern float glob_humidity;

extern float api_temperature;
extern float api_humidity;


extern String WIFI_SSID;
extern String WIFI_PASS;
extern String CORE_IOT_TOKEN;
extern String CORE_IOT_SERVER;
extern String CORE_IOT_PORT;

extern boolean isWifiConnected;
extern SemaphoreHandle_t xBinarySemaphoreInternet;
extern SemaphoreHandle_t i2cMutex;

extern LiquidCrystal_I2C lcd;

extern DHT20 dht20;
extern String ssid;
extern String password; 
extern String wifi_ssid;
extern String wifi_password;

#endif