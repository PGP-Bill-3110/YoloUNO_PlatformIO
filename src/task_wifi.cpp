#include "task_wifi.h"
#include "coreiot.h"
#include "task_webserver.h"
#include "global.h"
#include "task_core_iot.h"

void startAP()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(String(SSID_AP), String(PASS_AP));
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
}

void startSTA()
{
    if (WIFI_SSID.isEmpty())
    {
        Serial.println("[WiFi] SSID is empty, cannot connect");
        return;
    }

    Serial.printf("[WiFi] Connecting to %s...\n", WIFI_SSID.c_str());
    
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true); // Disconnect and turn off AP

    if (WIFI_PASS.isEmpty())
    {
        WiFi.begin(WIFI_SSID.c_str());
    }
    else 
    {
        WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());
    }

    unsigned long start_time = millis();
    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt < 30) // 30 * 500ms = 15s timeout
    {
        vTaskDelay(500 / portTICK_PERIOD_MS);
        Serial.print(".");
        attempt++;
    }
    
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("");
        Serial.println("[WiFi] WiFi Connected!");
        Serial.print("[WiFi] IP Address: ");
        Serial.println(WiFi.localIP());
        isWifiConnected = true;
        xSemaphoreGive(xBinarySemaphoreInternet);
    }
    else
    {
        Serial.println("");
        Serial.println("[WiFi] Failed to connect to WiFi");
        isWifiConnected = false;
    }
}

bool Wifi_reconnect()
{
    const wl_status_t status = WiFi.status();
    if (status == WL_CONNECTED)
    {
        return true;
    }
    startSTA();
    return false;
}

void task_wifi(void *pvParameters)
{
    // Initial connection attempt if SSID is available
    if (!WIFI_SSID.isEmpty())
    {
        Serial.println("[WiFi Task] Starting initial WiFi connection...");
        startSTA();
    }

    for (;;)
    {
        if (xSemaphoreTake(xBinarySemaphoreInternet, pdMS_TO_TICKS(5000)) == pdTRUE)
        {
            // Semaphore given - try to connect
            if (!WIFI_SSID.isEmpty())
            {
                Serial.println("[WiFi Task] Connecting to WiFi...");
                startSTA();
            }
        }
        
        // Check connection status periodically
        if (WiFi.status() == WL_CONNECTED)
        {
            isWifiConnected = true;
        }
        else
        {
            isWifiConnected = false;
        }
        
        vTaskDelay(5000); // Check every 5 seconds
    } 
}