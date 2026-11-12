# Carrier Rev A, dùng được cho cấu hình ECO

- Kích thước 60 x 40 mm; 2 lớp; FR4 0,8 mm; 1 oz; mạ lỗ xuyên; bề mặt HASL không chì hoặc ENIG.
- XIAO hàn castellated trên 14 pad biên. Các pad BAT/debug phía đáy được bỏ có chủ ý. USB quay ra mép ngắn theo silkscreen.
- Module buck gắn ngoài qua J2. J2 không phải footprint trực tiếp của Mini-360 hoặc Pololu.
- ECO **không lắp** J3/J4/J7/Q3/R7/R8, không lắp IMU, RP1 hoặc LED. Giữ tất cả mạch nguồn, PWM và đo pin.
- J1/J2/J5/J6 có thể hàn dây trực tiếp thay header để nhẹ. Dây phải có chống kéo và kiểm tra cực. Không lấy dòng servo qua chân 5V của XIAO.
- Pad vuông là pin 1. Servo: 1=GND, 2=VSERVO, 3=PWM. Pin JST/servo ngoài thực tế phải đối chiếu, không tin màu dây.
- Q1 AO4407A S=1/2/3, G=4, D=5/6/7/8. Q2 2N7002 G=1 S=2 D=3. D1 SS14 pin1 cathode, pin2 anode.
- U2 AHCT2G125DP pin1/7=OE thấp; pin2/5=A; pin6/3=Y; pin4=GND; pin8=5V.
- C5 220uF/16V low ESR: pin1 dương, pin2 âm, đường kính thân <=6,3mm, bước chân 2,5mm.
- Đường power 1mm tối thiểu; các via chuyển lớp chính rail VSERVO được gia cường. DRC không chứng minh khả năng chịu dòng/nhiệt; đo dưới tải servo thật trước bay.
- Không có chống ngược cực. Rút pin để ngắt điện vật lý. D1 chỉ ngăn nguồn USB cấp ngược vào bus 5V bên ngoài.
- File dự án chứa cả vị trí linh kiện mở rộng để có thể dùng khi nghiên cứu PID sau. BOM chính ghi số lượng 0 cho phần bỏ lắp.

`fabrication/` dùng để gửi xưởng khi chọn làm carrier. Chỉ gửi bộ Gerber/Excellon cùng revision; không trộn với Gerber tác giả ESP-FLUTTER.

`design.json` là bảng tham chiếu Refdes/pad/net/placement. Net trong KiCad có tiền tố `/` do net local ở sheet gốc.

Thư viện `Buom.kicad_sym` và `Buom.pretty` cùng `sym-lib-table` / `fp-lib-table` cần giữ cạnh project.
