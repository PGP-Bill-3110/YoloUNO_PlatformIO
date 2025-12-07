#include "queue.h"

void PrintSerialTask(void *pvParameters) {
    SensorData_t recv;

    while (1)
    {
        if (xQueueReceive(xSensorQueue, &recv, portMAX_DELAY))
        {
            if (recv.temperature > 30.0){
                Serial.print("[Warning] High temperature: ");
                Serial.print(recv.temperature);
                Serial.print("°C");
            } else if (recv.humidity > 80.0){
                Serial.print("[Warning] High humidity: ");
                Serial.print(recv.temperature);
                Serial.print("%");
            } else {
                Serial.print("Temp=");
                Serial.print(recv.temperature);
                Serial.print("°C Humidity=");
                Serial.print(recv.humidity);
                Serial.print("%");
            }
        }
    }
}