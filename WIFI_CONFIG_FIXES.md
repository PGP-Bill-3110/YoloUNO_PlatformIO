# WiFi Configuration Issues - Diagnosis & Fixes

## Problems Found (Các vấn đề tìm thấy):

### 1. **Semaphore không được khởi tạo (Uninitialized Semaphore)**
   - **Vấn đề**: `xBinarySemaphoreInternet` được tạo nhưng không được "give" lần đầu
   - **Kết quả**: `wifi_task()` bị chặn indefinitely chờ semaphore
   - **Sửa**: Thêm `xSemaphoreGive()` trong `setup()` nếu WiFi credentials tồn tại

### 2. **Luôn khởi động AP Mode (Always Start in AP Mode)**
   - **Vấn đề**: Trong `main_server_task()`, code luôn gọi `startAP_main()` trước, ngay cả khi WiFi credentials đã được cấu hình
   - **Kết quả**: Không thể kết nối WiFi trạng thái STA
   - **Sửa**: Kiểm tra xem WIFI_SSID có tồn tại không - nếu có, hãy chờ kết nối WiFi trước khi khởi động AP mode

### 3. **Task Priority không đúng (Wrong Task Priority)**
   - **Vấn đề**: `main_server_task` có priority = 1 (thấp), `wifi_task` có priority = 3 (cao)
   - **Kết quả**: `wifi_task` có thể bị chặn do `main_server_task` chạy trước
   - **Sửa**: Thay đổi `main_server_task` priority thành 2 để cân bằng

### 4. **Hàm `startAP()` không tồn tại (Missing Function)**
   - **Vấn đề**: `check_info_File()` gọi `startAP()` nhưng hàm này không được định nghĩa
   - **Kết quả**: Lỗi compile/link
   - **Sửa**: Loại bỏ lệnh gọi `startAP()` khỏi hàm này

### 5. **Save_info_File() gây ra ESP.restart() ngay lập tức**
   - **Vấn đề**: `Save_info_File()` gọi `ESP.restart()` ở cuối, nên client không nhận được phản hồi đầy đủ
   - **Kết quả**: User không biết rằng cấu hình đã được lưu hay kết nối có đang xảy ra không
   - **Sửa**: Gợi ý phía client rằng ESP sẽ restart, sau đó mới gọi `ESP.restart()` sau 2 giây

### 6. **WiFi Task startup không đúng (WiFi Task Startup Issues)**
   - **Vấn đề**: `task_wifi()` gọi `startSTA()` mà không kiểm tra WIFI_SSID, dẫn đến vòng lặp vô tận
   - **Kết quả**: Hành vi không xác định
   - **Sửa**: Kiểm tra WIFI_SSID.isEmpty() trước khi kết nối

## Files Modified (Các file đã sửa):

1. **src/main.cpp**
   - Thêm semaphore initialization trong setup()
   - Thay đổi priority của main_server_task từ 1 → 2

2. **src/mainserver.cpp**
   - Sửa `main_server_task()` để kiểm tra WiFi credentials trước
   - Thêm logic chờ WiFi connection trước khi setup webserver
   - Sửa `handleConnect()` để không restart ngay lập tức
   - Thêm handler `handleWiFiStatus()` để kiểm tra trạng thái WiFi

3. **src/task_wifi.cpp**
   - Sửa `startSTA()` với timeout tốt hơn (15 giây)
   - Sửa `task_wifi()` để kiểm tra WIFI_SSID và xử lý semaphore đúng
   - Thêm logging tốt hơn

4. **src/task_check_info.cpp**
   - Loại bỏ lệnh gọi `startAP()` không tồn tại

## Testing (Hướng dẫn kiểm tra):

### Scenario 1: Lần đầu khởi động (No WiFi Credentials)
1. Upload code
2. ESP32 khởi động vào AP Mode (SSID: `ESP32-YOUR NETWORK HERE!!!`)
3. Kết nối với AP
4. Vào `http://192.168.4.1/settings`
5. Nhập SSID và password WiFi
6. Nhấn "Kết nối"
7. ESP nên restart và kết nối WiFi trong 15 giây

### Scenario 2: Đã có WiFi Credentials (After Configuration)
1. Sau khi cấu hình, ESP khởi động
2. Serial output sẽ hiển thị "[WiFi Task] Connecting to WiFi..."
3. Nếu kết nối thành công: "[WiFi] WiFi Connected!" + IP address
4. Nếu kết nối thất bại: Chuyển về AP Mode sau 15 giây

### Scenario 3: Kiểm tra trạng thái WiFi
- Gọi `http://<ESP_IP>/wifistatus` để xem trạng thái hiện tại
- Response JSON: `{"status":"Connected|AP Mode|Not connected","ip":"...","ssid":"..."}`

## Serial Output Expected (Expected Serial Messages):

```
[Config] WiFi credentials loaded: SSID=E2 1421
[Main Server] WiFi credentials found, waiting for connection...
[WiFi Task] Starting initial WiFi connection...
[WiFi] Connecting to E2 1421...
.........................
[WiFi] WiFi Connected!
[WiFi] IP Address: 192.168.x.x
```

Hoặc (nếu thất bại):
```
[Config] WiFi credentials loaded: SSID=E2 1421
[Main Server] WiFi credentials found, waiting for connection...
[Main Server] WiFi connection timeout, starting AP mode...
[Main Server] AP IP address: 192.168.4.1
```

## Recommendations (Đề xuất):

1. **Thêm OTA Updates**: Để dễ dàng cập nhật firmware qua WiFi
2. **Persistent WiFi Retry**: Thử lại kết nối WiFi theo chu kỳ (e.g., mỗi 5 phút)
3. **Status LED**: Thêm LED để chỉ ra trạng thái WiFi (blinking = connecting, solid = connected)
4. **Timeout Configuration**: Cho phép user cấu hình timeout kết nối qua web interface
5. **Password Reset Button**: Giữ BOOT button trong 5 giây để xóa WiFi credentials và reset về AP mode
