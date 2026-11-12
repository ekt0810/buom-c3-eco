# PID để nghiên cứu sau, không nằm trong bản bay ECO

`control.h` chứa RatePID và mixer thuần C++ đã kiểm tra bằng test native. Đầu vào là rad/s, timestep giây. Vòng ngoài dùng K_angle=3 1/s, giới hạn rate +/-1,2 rad/s. Gains xem hướng dẫn và matlab/parameters.m.

Không có IMU trong BOM chính, vì vậy firmware_eco không gọi PID này. Để đưa lên máy bay cần thêm IMU, code thu nhận/fusion và nhận dạng cơ cấu thật. Hai servo không tạo ba mô-men điều khiển độc lập; không có yaw PID trực tiếp hoặc giữ vị trí ở đây.

Mixer: tần số 1,5..3,8Hz; biên độ cơ sở25 độ; chênh tối đa6 độ; tâm chung tối đa5 độ. Mapping servo phải hiệu chuẩn theo horn/hành trình thật.

Các hệ số lực/mô-men trong MATLAB là giả định. Kết quả mô phỏng không dùng làm chứng nhận ổn định hay khả năng mang tải.
