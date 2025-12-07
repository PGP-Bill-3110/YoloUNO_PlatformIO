#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <LiquidCrystal_I2C.h>
#include "DHT20.h"
#include <Adafruit_NeoPixel.h>

//for queue.h
typedef struct {
    float temperature;
    float humidity;
} SensorData_t;
extern QueueHandle_t xSensorQueue;

//for led_blinky.h
#define LED_GPIO    48
#define NORMAL_TEMP 0
#define MED_TEMP    1
#define HIGH_TEMP   2

//for neo_blinky.h
#define NEO_PIN             45
#define LED_COUNT           1
#define BREATHING_DELAY     20
#define RAINBOW_SLOW_DELAY  100
#define RAINBOW_FAST_DELAY  20
extern int neo_status;
extern Adafruit_NeoPixel neoStrip;

//for button.h
#define BUTTON      GPIO_NUM_0
#define BOOT_PIN    0
extern SemaphoreHandle_t xButtonSemaphore;

//for DHT_sensor.h
extern SemaphoreHandle_t xDataMutex;
extern SemaphoreHandle_t xI2CMutex;
extern float glob_temperature;
extern float glob_humidity;
extern DHT20 dht20;

//for monitor.h
extern char lcdBuffer[3][16];
extern LiquidCrystal_I2C lcd;

//for mainserver.h
extern bool fsm_mode;
extern bool tinyml_mode;
// extern volatile bool fsm_mode;
// extern volatile bool tinyml_mode;

//for fsm_ctrl.h
#define FSM_NORMAL  0
#define FSM_CONFIG  1
#define FSM_TINYML  2
extern int fsm_state;
// extern volatile int fsm_state;

//

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
extern SemaphoreHandle_t neoMutex;
extern int neoColorMode;

extern String ssid;
extern String password; 
extern String wifi_ssid;
extern String wifi_password;

#endif