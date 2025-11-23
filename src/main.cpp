#include "global.h"

#include "led_blinky.h"
#include "neo_blinky.h"
#include "temp_humi_monitor.h"
//#include "mainserver.h"
// #include "tinyml.h"
 #include "coreiot.h"


#include "task_check_info.h"
#include "task_toogle_boot.h"
#include "task_wifi.h"
#include "task_webserver.h"
#include "task_core_iot.h"

#include "uart.h"
#include "neo_humidity.h"
#include "tinyml_anomaly.h"
void setup()
{
  Serial.begin(115200);
  check_info_File(0);

    Wire.begin(11, 12);
    lcd.begin();
    lcd.backlight();

i2cMutex = xSemaphoreCreateMutex();

  xTaskCreate(led_blinky, "Task LED Blink", 2048, NULL, 2, NULL);
  //xTaskCreate(neo_blinky, "Task NEO Blink", 2048, NULL, 2, NULL);
  //xTaskCreate(temp_humi_monitor, "Task TEMP HUMI Monitor", 2048, NULL, 2, NULL);
   //xTaskCreate(main_server_task, "Task Main Server" ,8192  ,NULL  ,2 , NULL);
  // xTaskCreate( tiny_ml_task, "Tiny ML Task" ,2048  ,NULL  ,2 , NULL);
  xTaskCreate(coreiot_task, "CoreIOT Task" ,4096  ,NULL  ,2 , NULL);
   xTaskCreate(Task_Toogle_BOOT, "Task_Toogle_BOOT", 4096, NULL, 2, NULL);
   //xTaskCreate(task_uart_sender, "Task UART Sender", 2048, NULL, 2, NULL);
 xTaskCreate(task_wifi, "Task WiFi", 4096, NULL, 3, NULL);
//xTaskCreate(neo_humidity_task, "Neo Humidity Task", 2048, NULL, 2, NULL);
xTaskCreate(tinyml_anomaly, "TinyML Anomaly Task", 4096, NULL, 2, NULL);
}

void loop()
{
  /*
  if (check_info_File(1))
  {
    if (!Wifi_reconnect())
    {
      Webserver_stop();
    }
    else
    {
      CORE_IOT_reconnect();
    }
  }
  Webserver_reconnect();
  */
}