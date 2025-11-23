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
        vTaskDelete(NULL);
    }

    WiFi.mode(WIFI_STA);

    if (WIFI_PASS.isEmpty())
    {
        WiFi.begin(WIFI_SSID.c_str());
    }
    else
    {
        WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());
    }

    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    //Give a semaphore here
    xSemaphoreGive(xBinarySemaphoreInternet);
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
    // Kết nối lần đầu
    startSTA();

    for (;;)
    {
        if (!Wifi_reconnect())
        {
            Webserver_stop();
        }
        else
        {
            CORE_IOT_reconnect();
        }

        Webserver_reconnect();

        vTaskDelay(1000); // chạy 1 lần mỗi giây
    }
}