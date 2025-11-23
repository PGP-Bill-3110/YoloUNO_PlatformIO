#include "neo_humidity.h"

extern LiquidCrystal_I2C lcd;
extern SemaphoreHandle_t i2cMutex;   // <- thêm dòng này

void neo_humidity_task(void *pvParameters) {

    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.clear();
    strip.show();

    while (true) {
        float humidity = glob_humidity;
        float temperature = glob_temperature;

        // NeoPixel (không dùng I2C, không cần semaphore)
        if (humidity < 70.0) {
            strip.setPixelColor(0, strip.Color(0, 255, 0)); // green
        } 
        else if (humidity >= 70.0 && humidity < 80.0) {
            strip.setPixelColor(0, strip.Color(0, 0, 255)); // blue
        } 
        else {
            strip.setPixelColor(0, strip.Color(255, 0, 0)); // red
        }
        strip.show();


        // -----------------------------
        //   BẢO VỆ I2C CHO LCD
        // -----------------------------
        if (xSemaphoreTake(i2cMutex, portMAX_DELAY)) {

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Hum: ");
            lcd.print(humidity, 1);
            lcd.print("%");

            // Nếu cần dùng dòng 2 thì uncomment
            /*
            lcd.setCursor(0, 1);
            lcd.print("Temp: ");
            lcd.print(temperature, 1);
            lcd.print(" C");
            */

            xSemaphoreGive(i2cMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
