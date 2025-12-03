#include "neo_blinky.h"
#include "global.h"

void neo_blinky(void *pvParameters){

    // Use the global neoStrip (initialized in global.cpp)
    // Ensure neoStrip is ready
    if (neoMutex && xSemaphoreTake(neoMutex, pdMS_TO_TICKS(200)) == pdTRUE) {
        neoStrip.begin();
        neoStrip.clear();
        neoStrip.show();
        xSemaphoreGive(neoMutex);
    } else {
        // fallback: try to init without mutex
        neoStrip.begin();
        neoStrip.clear();
        neoStrip.show();
    }

    while(1) {
        if (neoMutex && xSemaphoreTake(neoMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            neoStrip.setPixelColor(0, neoStrip.Color(255, 0, 0)); // Red
            neoStrip.show();
            xSemaphoreGive(neoMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(500));

        if (neoMutex && xSemaphoreTake(neoMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            neoStrip.setPixelColor(0, neoStrip.Color(0, 255, 0)); // Green
            neoStrip.show();
            xSemaphoreGive(neoMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(500));

        if (neoMutex && xSemaphoreTake(neoMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            neoStrip.setPixelColor(0, neoStrip.Color(0, 0, 255)); // Blue
            neoStrip.show();
            xSemaphoreGive(neoMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}