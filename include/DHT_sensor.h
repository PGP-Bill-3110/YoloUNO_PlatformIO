#ifndef __DHT_SENSOR__
#define __DHT_SENSOR__
#include <Arduino.h>
#include "DHT20.h"
#include "global.h"
#include "queue.h"
#include <Wire.h>

void dht_sensor(void *pvParameters);

#endif