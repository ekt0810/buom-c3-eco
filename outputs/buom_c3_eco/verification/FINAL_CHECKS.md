# Kiểm tra bản bàn giao ECO — 12/11/2026

Đây là hồ sơ prototype để chế tạo và thử nghiệm, chưa phải mẫu bay đã nghiệm thu.

| Hạng mục | Kết quả thực tế |
|---|---|
| BOM XLSX | 3 sheet, không có ô lỗi; tổng công thức E4 = 3.865.000 đồng |
| BOM CSV | 18 dòng mua; tổng khớp XLSX |
| KiCad 10.0.8 ERC | 0 lỗi, 0 cảnh báo theo cấu hình kiểm tra đi kèm |
| KiCad 10.0.8 DRC | 0 vi phạm, 0 pad chưa nối, 0 lỗi footprint theo cấu hình đi kèm |
| Kết nối điện | 89 chân đã đối chiếu schematic / PCB / design.json |
| Nguồn servo trên PCB | Track nguồn tối thiểu 1 mm, 2 via trên tuyến nguồn chuyển lớp; cần đo nhiệt và sụt áp thật |
| Gerber / drill | Xuất lại sau sửa tuyến nguồn; drill có via 0,4 và 0,6 mm |
| Firmware aircraft | Arduino-ESP32 3.3.3, XIAO_ESP32C3: compile thành công, flash 952.486 byte, RAM tĩnh 34.248 byte |
| Firmware transmitter | Arduino-ESP32 3.3.3, ESP32 Dev Module: compile thành công, flash 913.315 byte, RAM tĩnh 43.728 byte |
| Kiểm tra C++ | CRC vector chuẩn, phát hiện gói lỗi, khóa 16 byte, PID saturation/recovery, giới hạn mixer: PASS |
| Mã MATLAB | Đã chạy bằng GNU Octave 11.3.0; chưa chạy trực tiếp bằng MATLAB |
| Viewer | Edge WebGL2; tự chạy, tạm dừng, đổi camera; không có lỗi JavaScript |
| PDF | 9 trang, đã render và xem bố cục từng trang |

Kết quả mô phỏng RMS sai số vị trí: danh định 0,265461 m; khối lượng tăng 20% là 0,574696 m; trễ 40 ms là 0,265819 m; gió mạnh là 1,083481 m. Hệ số khí động là giả định; mô phỏng có thông tin vị trí/độ cao lý tưởng mà máy bay ECO không có. Các số này không dự báo độ chính xác hay khả năng bay của mẫu thật.

Các kiểm tra bị bỏ qua trong ERC/DRC được liệt kê nguyên văn trong hai file .rpt. DRC không thay thế kiểm tra công suất, chế tạo và thử phần cứng.

Chưa kiểm chứng bằng phần cứng: hiệu chuẩn xung–góc, mô-men servo khi mang cánh, lực nâng, sụt nguồn, nhiễu, nhiệt, tầm RF, độ bền, ổn định và thời gian bay. Firmware giao mặc định khóa actuator; phải hoàn tất hiệu chuẩn và thử trên bàn theo hướng dẫn.

Tổng 3.865.000 đồng là dự toán, gồm nguồn cho sạc và 400.000 đồng dự phòng; không gồm dụng cụ hàn/đo. File cơ khí STL có phí của tác giả ESP-FLUTTER không được phân phối trong gói. Đọc hướng dẫn và chốt đúng servo/khung trước khi mua.
