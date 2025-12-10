#include "tinyml_anomaly.h"

tflite::ErrorReporter* anomaly_error_reporter = nullptr;
tflite::MicroInterpreter* anomaly_interpreter = nullptr;
TfLiteTensor* anomaly_input = nullptr;
TfLiteTensor* anomaly_output = nullptr;

constexpr int ANOMALY_TENSOR_ARENA_SIZE = 8 * 1024;
static uint8_t anomaly_tensor_arena[ANOMALY_TENSOR_ARENA_SIZE];

void setupTinyML_Anomaly() {
    static tflite::MicroErrorReporter anomaly_micro_error_reporter;
    anomaly_error_reporter = &anomaly_micro_error_reporter;

    const tflite::Model* model = tflite::GetModel(dht_anomaly_model_tflite);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        anomaly_error_reporter->Report("Model schema version mismatch.");
        return;
    }

    static tflite::AllOpsResolver resolver;

    static tflite::MicroInterpreter static_anomaly_interpreter(
        model,
        resolver,
        anomaly_tensor_arena,
        ANOMALY_TENSOR_ARENA_SIZE,
        anomaly_error_reporter
    );

    anomaly_interpreter = &static_anomaly_interpreter;

    if (anomaly_interpreter->AllocateTensors() != kTfLiteOk) {
        anomaly_error_reporter->Report("AllocateTensors failed");
        return;
    }

    anomaly_input = anomaly_interpreter->input(0);
    anomaly_output = anomaly_interpreter->output(0);

    Serial.println("TinyML Anomaly initialized.");
}

void tinyml_anomaly(void* pvParameters) {

    // Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    // strip.begin();
    // strip.clear();
    // strip.show();

    setupTinyML_Anomaly();

    while (1) {
        if(fsm_state != FSM_TINYML) {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        xSemaphoreTake(xDataMutex, portMAX_DELAY);
        float temperature = glob_temperature;
        float humidity = glob_humidity;
        xSemaphoreGive(xDataMutex);

        if (xSemaphoreTake(xI2CMutex, portMAX_DELAY)) 
        {
            anomaly_input->data.f[0] = temperature;
            anomaly_input->data.f[1] = humidity;

            if (anomaly_interpreter->Invoke() != kTfLiteOk) {
                anomaly_error_reporter->Report("Invoke failed");
                xSemaphoreGive(xI2CMutex);
                vTaskDelay(pdMS_TO_TICKS(1000));
                continue;
            }

            float ir_value = anomaly_output->data.f[0];
            int anomaly = (ir_value > 0.65f);

            // Update global neoStrip safely
            if (neoMutex && xSemaphoreTake(neoMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
                neoStrip.setPixelColor(0, anomaly ? neoStrip.Color(255,0,0) : neoStrip.Color(0,0,255));
                neoStrip.show();
                xSemaphoreGive(neoMutex);
            }

            static int page = 0;

            lcd.clear();
            lcd.setCursor(0,0);

            if (page == 0) {
                lcd.print("Temp: "); lcd.print(temperature,1); lcd.print("C");
                lcd.setCursor(0,1);
                lcd.print("Humid: "); lcd.print(humidity,1); lcd.print("%");
            } else if (page == 1) {
                lcd.print("Humid: "); lcd.print(humidity,1); lcd.print("%");
                lcd.setCursor(0,1);
                lcd.print("IR:"); lcd.print(ir_value,2); lcd.print(" -> ");
                lcd.print(anomaly ? "Anomaly" : "Normal");
            } else if (page == 2) {
                lcd.print("IR:"); lcd.print(ir_value,2); lcd.print(" -> ");
                lcd.print(anomaly ? "Anomaly" : "Normal");
                lcd.setCursor(0,1);
                lcd.print("Temp: "); lcd.print(temperature,1); lcd.print("C");
            }
             
            page = (page + 1) % 3;

            xSemaphoreGive(xI2CMutex);
        }

        Serial.print("Temp: "); Serial.print(temperature);
        Serial.print(" Hum: "); Serial.print(humidity);
        Serial.print(" IR: "); Serial.print(anomaly_output->data.f[0]);
        Serial.print(" -> "); Serial.println(anomaly_output->data.f[0] > 0.65f ? "Anomaly" : "Normal");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
