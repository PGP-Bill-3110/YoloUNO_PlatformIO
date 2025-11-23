#include "uart.h"
#include "global.h"   // để đọc glob_temperature và glob_humidity

HardwareSerial SerialPort2(2);

void uart_init() {
  SerialPort2.begin(UART_BAUD_RATE, SERIAL_8N1, RPI_RX_PIN, RPI_TX_PIN);
  Serial.println("[UART] UART2 Initialized for RPi communication.");
}

void uart_send_data(const String& data) {
  SerialPort2.println(data);
}

void task_uart_sender(void *pvParameters) {
  // Delay ban đầu để cảm biến ổn định
  vTaskDelay(pdMS_TO_TICKS(3000));

  // ---------------------------------------
  //  Gửi HELLO đúng 1 lần duy nhất
  // ---------------------------------------
  uart_send_data("**Hello from ESP32**");
  Serial.println("[UART] Sent once: **Hello from ESP32**");

  // ------------------------------------
  // ---
  //  Sau đó CHỈ gửi độ ẩm liên tục
  // ---------------------------------------
  while (true) {
    String msg = "HUMIDITY=" + String(glob_humidity, 2) + "%";

    uart_send_data(msg);

    Serial.println("[UART] Sent humidity: " + msg);

    // Gửi mỗi 3 giây
    vTaskDelay(pdMS_TO_TICKS(3000));
  }
}
