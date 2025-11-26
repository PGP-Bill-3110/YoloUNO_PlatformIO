#include "temp_humi_monitor.h"
#include "global.h"
#include <Wire.h>
#include "DHT20.h"

extern DHT20 dht20;
extern LiquidCrystal_I2C lcd;
extern SemaphoreHandle_t i2cMutex;
extern float glob_temperature;
extern float glob_humidity;

void temp_humi_monitor(void *pvParameters) {

    while (true) {
        float temperature = -1;
        float humidity = -1;

        // ===== LOCK I2C trước khi đọc DHT20 =====
        if (xSemaphoreTake(i2cMutex, portMAX_DELAY)) {
            dht20.read();
            temperature = dht20.getTemperature();
            humidity = dht20.getHumidity();
            xSemaphoreGive(i2cMutex);
        } else {
            Serial.println("[TEMP_HUMI] Failed to acquire I2C mutex");
        }

        // Kiểm tra đọc thành công
        if (isnan(temperature) || isnan(humidity)) {
            Serial.println("[TEMP_HUMI] Failed to read from DHT sensor!");
            temperature = humidity = -1;
        }

        // Cập nhật biến toàn cục
        glob_temperature = temperature;
        glob_humidity = humidity;

        // ===== LOCK I2C trước khi cập nhật LCD =====
        if (xSemaphoreTake(i2cMutex, portMAX_DELAY)) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Hu:"); lcd.print(humidity,1); lcd.print("% T:");
            lcd.print(temperature,1); lcd.print(" C");
            xSemaphoreGive(i2cMutex);
        }

        // Serial output (không cần mutex)
        Serial.print("[TEMP_HUMI] Temp: "); Serial.print(temperature);
        Serial.print(" C  Hum: "); Serial.print(humidity); Serial.println("%");

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
