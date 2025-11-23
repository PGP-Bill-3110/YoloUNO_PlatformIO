#ifndef UART_H
#define UART_H

#include <Arduino.h>

// --- Cấu hình UART2 ---
// Khuyến nghị dùng UART2 để UART0 vẫn dùng cho Serial.begin(115200) debug
#define UART_BAUD_RATE 115200

// Chân GPIO kết nối với Raspberry Pi (RPi)
// Thay đổi các chân này tùy theo kết nối thực tế của bạn
#define RPI_RX_PIN 16 // ESP32-S3 RX2 (Nhận từ RPi TX)
#define RPI_TX_PIN 17 // ESP32-S3 TX2 (Gửi đến RPi RX)

// Khai báo đối tượng Serial cho UART2
extern HardwareSerial SerialPort2;

// --- Giao diện Hàm ---

/**
 * @brief Khởi tạo giao tiếp UART2 với RPi.
 */
void uart_init();

/**
 * @brief Task FreeRTOS để gửi dữ liệu định kỳ qua UART2.
 * @param pvParameters tham số FreeRTOS (NULL)
 */
void task_uart_sender(void *pvParameters);

/**
 * @brief Hàm gửi dữ liệu ra UART.
 * @param data Chuỗi dữ liệu cần gửi.
 */
void uart_send_data(const String& data);

#endif // UART_H