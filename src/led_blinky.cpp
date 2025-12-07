#include "led_blinky.h"

void led_blinky(void *pvParameters){
  pinMode(LED_GPIO, OUTPUT);
  int prevTempState = NORMAL_TEMP;
  
  while(1) {
    //Normal state: blinks every 1 second
    //Medium state: fast blink 3 times (when changed first time), then blinks every 500ms
    //High state: high for 3 seconds (when changed first time), then blinks every 200ms
    //Every button press: blinks fast 5 times

    if(fsm_state != FSM_NORMAL){
      vTaskDelay(100);
      continue;
    }

    if(xSemaphoreTake(xButtonSemaphore, 0) == pdTRUE){
      for(int i=0; i<5; ++i){
        digitalWrite(LED_GPIO, HIGH);
        vTaskDelay(50);
        digitalWrite(LED_GPIO, LOW);
        vTaskDelay(50);
      }
    }

    xSemaphoreTake(xDataMutex, portMAX_DELAY);
    float temp = glob_temperature;
    xSemaphoreGive(xDataMutex);

    int curTempState;

    if(temp <= 30.0) curTempState = NORMAL_TEMP;
    else if(temp <= 40.0) curTempState = MED_TEMP;
    else curTempState = HIGH_TEMP;

    if(curTempState != prevTempState){
      switch(curTempState){
        case MED_TEMP:
          for(int i=0; i<3; ++i){
            digitalWrite(LED_GPIO, HIGH);
            vTaskDelay(50);
            digitalWrite(LED_GPIO, LOW);
            vTaskDelay(50);
          }
          break;
        case HIGH_TEMP:
          digitalWrite(LED_GPIO, HIGH);
          vTaskDelay(2800);
          break;
        default:
          break;
      }
      prevTempState = curTempState;
    }

    switch(curTempState){
      case NORMAL_TEMP:
        digitalWrite(LED_GPIO, HIGH);
        vTaskDelay(1000);
        digitalWrite(LED_GPIO, LOW);
        vTaskDelay(1000);
        break;
      case MED_TEMP:
        digitalWrite(LED_GPIO, HIGH);
        vTaskDelay(500);
        digitalWrite(LED_GPIO, LOW);
        vTaskDelay(500);
        break;
      case HIGH_TEMP:
        digitalWrite(LED_GPIO, HIGH);
        vTaskDelay(200);
        digitalWrite(LED_GPIO, LOW);
        vTaskDelay(200);
        break;
      default:
        break;
    }
  }
}