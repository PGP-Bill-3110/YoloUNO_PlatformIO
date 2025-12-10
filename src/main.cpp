#include "global.h"
#include "led_blinky.h"
#include "neo_blinky.h"
#include "DHT_sensor.h"
#include "monitor.h"
#include "queue.h"
#include "button.h"
#include "fsm_ctrl.h"

#include "mainserver.h"
#include "coreiot.h"

#include "task_check_info.h"
#include "task_toogle_boot.h"
#include "task_wifi.h"
#include "task_webserver.h"
#include "task_core_iot.h"

#include "tinyml_anomaly.h"



void setup()
{
  Serial.begin(115200);
  Wire.begin(11, 12);
  check_info_File(0);

  dht20.begin();
  lcd.begin();
  lcd.backlight();

  xTaskCreate(led_blinky, "Task LED Blink", 2048, NULL, 2, NULL);
  xTaskCreate(neo_blinky, "Task NEO Blink", 2048, NULL, 2, NULL);
  xTaskCreate(dht_sensor, "DHT Sensor", 2048, NULL, 2, NULL);
  xTaskCreate(LCD_task, "LCD Task", 2048, NULL, 2, NULL);
  xTaskCreate(getKeyButton, "Button Task", 2048, NULL, 2, NULL);
  xTaskCreate(PrintSerialTask, "Serial Task", 2048, NULL, 2, NULL);
  xTaskCreate(fsmCtrlTask, "FSM Control Task", 2048, NULL, 2, NULL);
  xTaskCreate(main_server_task, "Task Main Server" ,8192  ,NULL  ,2 , NULL);  // Priority 2
  xTaskCreate(task_wifi, "Task WIFI" ,4096  ,NULL  ,3 , NULL);  // Priority 3 = higher
  xTaskCreate(coreiot_task, "CoreIOT Task" ,4096  ,NULL  ,2 , NULL);
  xTaskCreate(Task_Toogle_BOOT, "Task_Toogle_BOOT", 4096, NULL, 2, NULL);
  xTaskCreate(tinyml_anomaly, "TinyML Anomaly Task", 4096, NULL, 2, NULL);
  
  // Initialize semaphore - give it once if WiFi credentials exist
  if (!WIFI_SSID.isEmpty())
  {
    xSemaphoreGive(xBinarySemaphoreInternet);
  }
}

void loop()
{
}