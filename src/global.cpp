#include "global.h"

float glob_temperature = 0;
float glob_humidity = 0;
char lcdBuffer[3][16] = {"Temp: 0.00C", "Humid: 0.00%", "State: NORMAL"};

int neo_status = 0;

bool fsm_mode = false;
bool tinyml_mode = false;
int fsm_state = FSM_NORMAL;

float api_temperature = 0;
float api_humidity = 0;

// Default values - will be overwritten by config file (/littlefs/info.dat)
String WIFI_SSID = "";           // Will load from file or be set via /connect endpoint
String WIFI_PASS = "";           // Will load from file or be set via /connect endpoint
String CORE_IOT_TOKEN = "yr1akiur4otyf6wzqa94"; // Will load from file
String CORE_IOT_SERVER = "app.coreiot.io"; // Will load from file
String CORE_IOT_PORT = "1883";

// AP Mode credentials (fallback when WiFi unavailable)
boolean isWifiConnected = false;
String ssid = "ESP32-YOUR NETWORK HERE!!!";
String password = "12345678";

DHT20 dht20;
LiquidCrystal_I2C lcd(33, 16, 2);
Adafruit_NeoPixel neoStrip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);

SemaphoreHandle_t xButtonSemaphore = xSemaphoreCreateBinary();
SemaphoreHandle_t xDataMutex = xSemaphoreCreateMutex();
SemaphoreHandle_t xI2CMutex = xSemaphoreCreateMutex();
QueueHandle_t xSensorQueue = xQueueCreate(10, sizeof(SensorData_t));

SemaphoreHandle_t xBinarySemaphoreInternet = xSemaphoreCreateBinary();
SemaphoreHandle_t neoMutex = xSemaphoreCreateMutex();