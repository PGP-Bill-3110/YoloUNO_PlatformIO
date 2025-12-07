#include "button.h"

void getKeyButton(void *pvParameters){
    pinMode(BUTTON, INPUT_PULLUP);
    int lastState = HIGH;

    while(1){
        int state = digitalRead(BUTTON);
        if(state == LOW && lastState == HIGH){
            vTaskDelay(pdMS_TO_TICKS(50));
            state = digitalRead(BUTTON);
            if(state == LOW){
                neo_status = (neo_status+1) % 3;
                xSemaphoreGive(xButtonSemaphore);

                while(digitalRead(BUTTON) == LOW){
                    vTaskDelay(pdMS_TO_TICKS(10));
                }
            }
        }

        lastState = state;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}