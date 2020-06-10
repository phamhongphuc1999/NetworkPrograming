# bài tập lớn môn lập trình mạng
## môi trường phát triển: visual studio 2015, ngôn ngữ c++
## miêu tả solution: có hai project SERVER, CLIENT là empty project
- project SERVER: là console(sử dụng hàm main), không có giao diện
- project CLIENT: sử dụng WinMain để tạo giao diện
## Server có các chức năng sau:
- Nhận yêu cầu tìm kiếm file (theo tên file) của client nào đó
- Gửi lệnh tìm kiếm tới các client khác đang kết nối
- Gửi lại cho client danh sách các client có file mà client tìm kiếm
- Chuyển tiếp file khi client yêu cầu 
## Client có các chức năng sau:
- Gửi yêu cầu tìm kiếm một file lên server
- Nhận danh sách  các client có file mà client yêu cầu tìm kiếm
- Lựa chọn một client trong kết quả server gửi về để yêu cầu download file
##### mỗi client có sau khi kết nối thành công đến server sẽ được server tạo một ID và gửi về cho client
##### => nâng cấp chương trình bằng việc thay thế ID bằng username, password

## Khuôn dạng gói tin: định dạng struct
#### Có 2 trường: type, data
- type: kiểu int, định nghĩa chức năng cần thực hiện
- data: dữ liệu gửi kèm theo

### Định nghĩa opcode
#### 1xx, 2xx: server gửi tín hiệu cho client
#### 3xx, 4xx: client gửi tín hiệu cho server
#### SERVER:
- 100: kết nối thành công, gửi ID lại client
- 110: gửi danh sách các client đang kết nối
- 111: gửi danh sách các client có file yêu cầu
- 112: tải file từ server về client
- 120: gửi yêu cầu tìm kiếm file đến client
- 121: gửi yêu cầu tải file lên server
- 200: yêu cầu chuyển tiếp file về client
- 201: tải file chuyển tiếp về client
- 202: ID do client gửi lên có thể kết nối và được phép chuyển tiếp
- 203: ID do client gửi lên không thể kết nối hoặc bị từ chối chuyển tiếp
#### CLIENT:
- 300: yêu cầu kết nối và gửi ID
- 310: gửi yêu cầu tìm kiếm file
- 311: tải file từ client được chỉ định bởi client khác lên server
- 320: không tìm thấy tên file trong trường data
- 321: tìm thấy tên file trong trường data
- 400: gửi ID và fileName
- 401: gửi lần lượt từng gói dữ liệu đến server, lần cuối không có dữ liệu để thông báo kết thúc
- 410: không cho chuyển tiếp file về client
- 411: cho phép chuyển tiếp file về client

## tài liệu tham khảo
- https://docs.microsoft.com/en-us/windows/win32/controls/user-controls-intro
- https://stackoverflow.com/questions/7598067/how-to-create-a-windows-style-textbox-in-a-c-win32-application
- https://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path
- https://stackoverflow.com/questions/2886609/how-to-make-multiple-windows-using-win32-api#:~:text=5%20Answers&text=You%20can%20hit%20CreateWindow(),1st%20one%20if%20you%20want.
- https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox
- https://stackoverflow.com/questions/42046666/test-project-exe-has-triggered-a-breakpoint
