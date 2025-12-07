#include "neo_blinky.h"

// Breathing effect color scaling
uint32_t scaleColor(uint32_t color, float brightness){
    if(brightness < 0) brightness = 0;
    if(brightness > 1) brightness = 1;

    uint8_t r = ((color>>16) & 0xFF) * brightness;
    uint8_t g = ((color>>8) & 0xFF) * brightness;
    uint8_t b = ((color) & 0xFF) * brightness;
    return neoStrip.Color(r,g,b);
}

// HSV to RGB conversion for rainbow effect
// Input: h (hue 0-1), s (saturation 0-1), v (brightness 0-1)
// Output: 32-bit RGB color value
uint32_t rainbow(float h, float s, float v){
    float red, green, blue;
    
    int sector = int(h * 6);
    float fade = h * 6 - sector;
    
    float p = v * (1 - s);
    float q = v * (1 - fade * s);
    float t = v * (1 - (1 - fade) * s);

    // Gán RGB dựa trên sector (khoảng màu)
    switch(sector % 6){
        case 0: red=v; green=t; blue=p; break;      // Đỏ → Vàng
        case 1: red=q; green=v; blue=p; break;      // Vàng → Xanh lá
        case 2: red=p; green=v; blue=t; break;      // Xanh lá → Xanh lam
        case 3: red=p; green=q; blue=v; break;      // Xanh lam → Chàm
        case 4: red=t; green=p; blue=v; break;      // Chàm → Tím
        case 5: red=v; green=p; blue=q; break;      // Tím → Đỏ
        default: red=0; green=0; blue=0; break;
    }

    return neoStrip.Color(int(red*255), int(green*255), int(blue*255));
}

// Use the global neoStrip (initialized in global.cpp)
void neo_blinky(void *pvParameters){

    // Initialize NeoPixel strip in global.cpp
    // Set all pixels to off to start
    neoStrip.begin();
    neoStrip.clear();
    neoStrip.show();

    uint32_t baseColor = neoStrip.Color(0,0,0);// for breathing effect
    float hue = 0; // for rainbow effect

    while(1) {
        xSemaphoreTake(xDataMutex, portMAX_DELAY);
        float humid = glob_humidity;
        xSemaphoreGive(xDataMutex);
        switch(neo_status){
            case 0:
                if(humid < 70) baseColor = neoStrip.Color(0, 255, 0); // Set strip to green
                else if(humid < 80) baseColor = neoStrip.Color(255, 255, 0); // Set strip to yellow
                else baseColor = neoStrip.Color(255, 0, 0); // Set strip to red
                neoStrip.show();
        
                for(float b=0; b<=1; b+=0.05){
                    neoStrip.setPixelColor(0, scaleColor(baseColor, b));
                    neoStrip.show();
                    vTaskDelay(BREATHING_DELAY);
                }
                for(float b=1; b>=0; b-=0.05){
                    neoStrip.setPixelColor(0, scaleColor(baseColor, b));
                    neoStrip.show();
                    vTaskDelay(BREATHING_DELAY);
                }
                break;
            case 1:
                for(hue=0; hue<=1; hue+=0.01){
                    neoStrip.setPixelColor(0, rainbow(hue, 1.0, 1.0));
                    neoStrip.show();
                    vTaskDelay(pdMS_TO_TICKS(RAINBOW_SLOW_DELAY));
                }
                for(hue=1; hue>=0; hue-=0.01){
                    neoStrip.setPixelColor(0, rainbow(hue, 1.0, 1.0));
                    neoStrip.show();
                    vTaskDelay(pdMS_TO_TICKS(RAINBOW_SLOW_DELAY));
                }
                break;
            case 2:
                for(hue=0; hue<=1; hue+=0.01){
                    neoStrip.setPixelColor(0, rainbow(hue, 1.0, 1.0));
                    neoStrip.show();
                    vTaskDelay(pdMS_TO_TICKS(RAINBOW_FAST_DELAY));
                }
                for(hue=1; hue>=0; hue-=0.01){
                    neoStrip.setPixelColor(0, rainbow(hue, 1.0, 1.0));
                    neoStrip.show();
                    vTaskDelay(pdMS_TO_TICKS(RAINBOW_FAST_DELAY));
                }
            default:
                break;
        }
    }
}