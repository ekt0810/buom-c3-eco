# Firmware chính - ECO điều khiển tay

Hai sketch độc lập, Arduino-ESP32 **3.3.3**, không cần thư viện Arduino ngoài core.

| Thiết bị | Board Arduino | Sketch |
|---|---|---|
| Bướm | XIAO_ESP32C3 | aircraft/aircraft.ino |
| Tay phát | ESP32 Dev Module (WROOM-32) | transmitter/transmitter.ino |

Nạp lần đầu khi chưa gắn cánh và servo. Trong Serial Monitor115200, mỗi board sẽ in MAC lặp lại khi chưa cấu hình peer. Ví dụ MAC tay phát là AA:BB:CC:DD:EE:FF thì sửa aircraft/config.h thành:

```cpp
constexpr uint8_t TX_MAC[6] = {0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};
```

Làm ngược lại với AIR_MAC trong tay phát. Không dùng MAC ví dụ ở trên làm địa chỉ thật. Đổi PMK/LMK trong cả hai protocol.h thành hai chuỗi ASCII đúng16ký tự; hai board phải dùng cùng giá trị. Cùng CHANNEL=6. Không kết nối thêm vào Wi-Fi AP khác làm đổi channel.

Sau khi đo nguồn và kiểm tra đầu nối, đặt RELEASE_ACTUATORS=true trên bướm. Giữ USE_POWER_SWITCH=true nếu đã lắp Q1/Q2 đúng schematic. Không bỏ mạch ngắt nguồn rồi kỳ vọng firmware có thể cắt nguồn servo.

Hiệu chuẩn ADC của tay phát, chiều joystick, tâm/pulse endpoints và US_PER_DEG của servo theo hướng dẫn PDF. 11,11us/độ là giả định, phải đo trên servo thực khi chưa gắn cánh. Tắt ARM và hạ ga trước cấp pin. Các sketch không tự giữ thăng bằng, cao độ hoặc vị trí.

Lệnh Arduino CLI để kiểm tra tương đương:

```text
arduino-cli compile --fqbn esp32:esp32:XIAO_ESP32C3 aircraft
arduino-cli compile --fqbn esp32:esp32:esp32 transmitter
```

Log biên dịch đã chạy nằm trong verification. Chưa nạp hoặc đo trên phần cứng thật trong lần bàn giao này.
