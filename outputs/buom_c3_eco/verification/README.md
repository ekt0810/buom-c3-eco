# Phạm vi kiểm tra

- KiCad 10.0.8: ERC/DRC; đối chiếu 89 chân kết nối giữa schematic, board và design.json.
- Arduino-ESP32 3.3.3: compile aircraft cho XIAO_ESP32C3; transmitter cho ESP32 Dev Module. Đây không phải kiểm thử chạy trên chip thật.
- C++ native: CRC16/mutation, chiều dài khóa 16 byte, anti-windup/recovery, giới hạn mixer 36 độ.
- Octave11.3.0: run_all(false), 4 kịch bản; PID saturation/recovery. Matplotlib vẽ dữ liệu MAT thực.
- Edge/Playwright: Three.js/WebGL2, auto-running, pause, camera, không có lỗi JS.
- Spreadsheet: kiểm tra công thức, không có lỗi ô; tăng một XIAO làm tổng tăng đúng170.000 rồi phục hồi.
- PDF: render từng trang và kiểm tra bố cục bằng ảnh.

Chưa thử: điện áp/xung thật, EMI, nhiệt servo/buck/FET, lực nâng, độ mỏi cánh, tầm RF thực, endurance hoặc bay.
