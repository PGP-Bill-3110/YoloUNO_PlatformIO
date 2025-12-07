#include "global.h"
//for queue.h
QueueHandle_t xSensorQueue = xQueueCreate(10, sizeof(SensorData_t));

//for led_blinky.h

//for neo_blinky.h
int neo_status = 0;
Adafruit_NeoPixel neoStrip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);

//for button.h
SemaphoreHandle_t xButtonSemaphore = xSemaphoreCreateBinary();

//for DHT_sensor.h
float glob_temperature = 0;
float glob_humidity = 0;
DHT20 dht20;
SemaphoreHandle_t xDataMutex = xSemaphoreCreateMutex();
SemaphoreHandle_t xI2CMutex = xSemaphoreCreateMutex();

//for monitor.h
LiquidCrystal_I2C lcd(33, 16, 2);
char lcdBuffer[3][16] = {"Temp: 0.00C", "Humid: 0.00%", "State: NORMAL"};

//

float api_temperature = 0;
float api_humidity = 0;


String WIFI_SSID = "E2 1421";
String WIFI_PASS = "phuc3110";
String CORE_IOT_TOKEN = "yr1akiur4otyf6wzqa94"; // phải chính xác
String CORE_IOT_SERVER = "app.coreiot.io"; // phải chính xác
String CORE_IOT_PORT;

String ssid = "ESP32-YOUR NETWORK HERE!!!";
String password = "12345678";
String wifi_ssid = "E2 1421";
String wifi_password = "phuc3110";
boolean isWifiConnected = false;

SemaphoreHandle_t xBinarySemaphoreInternet = xSemaphoreCreateBinary();
SemaphoreHandle_t i2cMutex = xSemaphoreCreateMutex();

// Mutex and mode for controlling neoStrip safely across tasks
SemaphoreHandle_t neoMutex = xSemaphoreCreateMutex();
int neoColorMode = 0; // 0=OFF,1=RED,2=GREEN,3=BLUE