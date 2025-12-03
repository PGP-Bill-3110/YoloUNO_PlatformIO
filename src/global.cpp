#include "global.h"
float glob_temperature = 0;
float glob_humidity = 0;

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
LiquidCrystal_I2C lcd(33, 16, 2);
SemaphoreHandle_t i2cMutex = NULL;
Adafruit_NeoPixel neoStrip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
DHT20 dht20;
// Mutex and mode for controlling neoStrip safely across tasks
SemaphoreHandle_t neoMutex = xSemaphoreCreateMutex();
int neoColorMode = 0; // 0=OFF,1=RED,2=GREEN,3=BLUE