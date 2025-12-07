#include "DHT_sensor.h"

void dht_sensor(void *pvParameters){

    while (1){
        SensorData_t sensorPkt;
        sensorPkt.temperature = 0;
        sensorPkt.humidity = 0;

        if(xSemaphoreTake(xI2CMutex, portMAX_DELAY)){
            if(dht20.read()==0){
                sensorPkt.temperature = dht20.getTemperature();
                sensorPkt.humidity = dht20.getHumidity();
                xSemaphoreGive(xI2CMutex);
            } else Serial.println("[TEMP_HUMI] Failed to acquire I2C mutex");
        }

        if (isnan(sensorPkt.temperature) || isnan(sensorPkt.humidity)) {
            Serial.println("[TEMP_HUMI] Failed to read from DHT sensor!");
            sensorPkt.temperature = sensorPkt.humidity = -1;
        }

        xSemaphoreTake(xDataMutex, portMAX_DELAY);
        glob_temperature = sensorPkt.temperature;
        glob_humidity = sensorPkt.humidity;
        xSemaphoreGive(xDataMutex);

        xQueueSend(xSensorQueue, &sensorPkt, portMAX_DELAY);

        vTaskDelay(5000);
    }
}