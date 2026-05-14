# Report 1 page - Lab 4 DES / TripleDES

## Mục tiêu
Xây dựng và hoàn thiện chương trình mã hóa/giải mã DES và TripleDES (3DES), cho phép nhập liệu từ bàn phím, xử lý đa khối (multi-block) với Zero padding.

## Cách làm / Method
Tiếp nhận mã nguồn cơ bản, tích hợp chức năng chọn chế độ (mode 1-4). Bổ sung hàm giải mã DES bằng cách đảo ngược thứ tự khóa vòng (round keys). Kế thừa DES để triển khai 3DES theo chuẩn mã hóa E-D-E và giải mã D-E-D. Tích hợp đọc xuất dữ liệu chuẩn nhị phân.

## Kết quả / Result
Chương trình thực thi thành công các ca kiểm thử: mã hóa/giải mã DES 1 block, DES đa khối có padding, và 3DES. Các test case tự động đều vượt qua.

## Kết luận / Conclusion
Hiểu rõ hơn về cấu trúc mạng Feistel, cách sinh khóa và thứ tự áp dụng khóa trong quá trình giải mã. Nhận thấy điểm yếu của Zero padding và sự cần thiết của các chuẩn padding an toàn hơn trong thực tế.