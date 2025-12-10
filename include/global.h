#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <LiquidCrystal_I2C.h>
#include "DHT20.h"
#include <Adafruit_NeoPixel.h>

#define LED_GPIO            48
#define NORMAL_TEMP         0
#define MED_TEMP            1
#define HIGH_TEMP           2

#define NEO_PIN             45
#define LED_COUNT           1
#define BREATHING_DELAY     20
#define RAINBOW_SLOW_DELAY  100
#define RAINBOW_FAST_DELAY  20

#define BUTTON              GPIO_NUM_0
#define BOOT_PIN            0

#define FSM_NORMAL          0
#define FSM_CONFIG          1
#define FSM_TINYML          2


extern float glob_temperature;
extern float glob_humidity;
extern char lcdBuffer[3][16];
typedef struct {
    float temperature;
    float humidity;
} SensorData_t;

extern int neo_status;

extern bool fsm_mode;
extern bool tinyml_mode;
extern int fsm_state;

extern float api_temperature;
extern float api_humidity;

extern String WIFI_SSID;
extern String WIFI_PASS;
extern String CORE_IOT_TOKEN;
extern String CORE_IOT_SERVER;
extern String CORE_IOT_PORT;

extern boolean isWifiConnected;

extern String ssid;
extern String password; 
extern String wifi_ssid;
extern String wifi_password;

extern DHT20 dht20;
extern LiquidCrystal_I2C lcd;
extern Adafruit_NeoPixel neoStrip;

extern SemaphoreHandle_t xButtonSemaphore;
extern SemaphoreHandle_t xDataMutex;
extern SemaphoreHandle_t xI2CMutex;
extern QueueHandle_t xSensorQueue;

extern SemaphoreHandle_t xBinarySemaphoreInternet;
extern SemaphoreHandle_t neoMutex;

#endif