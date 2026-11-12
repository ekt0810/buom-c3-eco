# Bướm C3 ECO - đọc trước khi mua

**Phương án hiện tại: điều khiển tay, dưới 4 triệu đồng.** Dự toán 3.865.000 đồng gồm bướm, tay ESP-NOW tự ráp, sạc và 400.000 đồng dự phòng. Không gồm dụng cụ hàn/đo. Giá chưa phải báo giá chốt.

1. Đọc `HUONG_DAN_CHE_TAO.pdf`.
2. Mở `BOM_Buom_C3.xlsx`, cập nhật giá. Chốt hai servo SDS1601 đúng bản HV và bộ khung/cánh tương thích trước các món khác.
3. Mạch tiết kiệm có thể hàn bảng lỗ theo schematic. `hardware/` có KiCad và Gerber của carrier PCB tùy chọn; không bắt buộc đặt PCB.
4. Nạp mã trong `firmware_eco/`. Máy bay là **XIAO ESP32-C3**, tay phát là **ESP32-WROOM-32 DevKit V1**. Core Arduino-ESP32 3.3.3. Đọc MAC, sửa config và khóa hai bên trước sử dụng. Actuator mặc định bị khóa.
5. Thử nguồn, chiều servo, mất link, dòng/nhiệt và lực nâng theo hướng dẫn rồi mới bay có giữ/bay tự do ngắn.

**Trạng thái:** prototype đã kiểm tra CAD, biên dịch và mô phỏng. Chưa có mẫu bay vật lý; không bảo đảm bay chỉ nhờ lắp đúng BOM. Khối lượng dự kiến bản bảng lỗ 77,5 g, phải cân lại. Mục tiêu sàng lọc lực nâng khoảng 0,99 N cho khối lượng này.

File khung STL có phí của tác giả ESP-FLUTTER **không nằm trong gói**; hướng dẫn có link chính chủ và khoản dự toán. Giữ hình học cơ sở đã bay. Ảnh và mô hình Three.js là minh họa, không dùng làm bản cắt cánh.

## File cần dùng

| Thư mục / file | Chức năng |
|---|---|
| BOM_Buom_C3.xlsx / BOM.csv | Danh sách mua, giá dự toán, khối lượng |
| HUONG_DAN_CHE_TAO.pdf / .md | Đấu dây, cơ khí, nạp, trim, thử nghiệm, PID tham khảo |
| hardware/buom_c3.kicad_sch / .kicad_pcb | Thiết kế điện sửa được bằng KiCad 10 |
| hardware/schematic.pdf | Schematic để đọc/in; connector đánh số theo pad |
| hardware/fabrication/ | Gerber và Excellon, PCB carrier tùy chọn |
| firmware_eco/aircraft/ | Firmware chính của bướm |
| firmware_eco/transmitter/ | Firmware tay phát |
| pid_reference/ | PID thuần C++ tham khảo; không có IMU trong BOM chính |
| matlab/run_all.m | Mô phỏng nghiên cứu với định vị lý tưởng |
| matlab/results/ | Kết quả đã chạy trên Octave, đồ thị và dữ liệu |
| viewer/ | Three.js/WebGL phát lại dữ liệu mô phỏng |
| start_viewer.py | Chạy web cục bộ, tự mở trình duyệt |
| verification/ | Log compile, ERC/DRC, đối chiếu netlist và kiểm tra điều khiển |
| measurement_log.csv | Mẫu ghi số đo mẫu thật |

MATLAB: mở thư mục `matlab`, chạy `run_all`. Không cần toolbox. Đã chạy thực trên Octave 11.3.0; chưa chạy trực tiếp MATLAB tại máy tạo gói. PID và bám đường tự động chỉ là nghiên cứu nâng cấp, không có trên bản mua tiết kiệm.

Viewer: cài Python 3, chạy `python start_viewer.py`; truy cập `http://127.0.0.1:8765/viewer/`. Không mở trực tiếp index.html qua file:// vì trình duyệt chặn nạp dữ liệu. Toàn bộ thư viện Three.js đi kèm, không cần Internet khi xem.

Không có thao tác publish/deploy hoặc đặt mua linh kiện tự động.
