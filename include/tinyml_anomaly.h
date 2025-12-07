#ifndef TINYML_ANOMALY_H
#define TINYML_ANOMALY_H

#include <Arduino.h>
#include "global.h"
#include "dht_anomaly_model.h"
#include "neo_blinky.h"   
 
#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

extern tflite::ErrorReporter* anomaly_error_reporter;
extern tflite::MicroInterpreter* anomaly_interpreter;
extern TfLiteTensor* anomaly_input;
extern TfLiteTensor* anomaly_output;

void setupTinyML_Anomaly();
void tinyml_anomaly(void *pvParameters);

#endif

