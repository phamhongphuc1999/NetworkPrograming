# bài tập lớn môn lập trình mạng
## Server có các chức năng sau:
- Nhận yêu cầu tìm kiếm file (theo tên file) của client A nào đó
- Gửi lệnh tìm kiếm tới các client khác đang kết nối
- Gửi lại cho client A danh sách các client có file mà client A tìm kiếm
- Chuyển tiếp file khi client A yêu cầu 
## Client có các chức năng sau:
- Gửi yêu cầu tìm kiếm một file lên server
- Nhận danh sách  các client có file mà client yêu cầu tìm kiếm
- Lựa chọn một client trong kết quả server gửi về để yêu cầu download file

## Khuôn dạng gói tin: định dạng mảng char
#### Có ba trường opcode, length, data
- opcode: 3 phần tử đầu trong mảng, chỉ định chức năng sẽ thực hiện
- length: 10 phần tử tiếp theo trong mảng, chứa kích thức của data
- data: phần còn lại của gói tin gửi đi, chứa dữ liệu cần truyền
### mỗi client có ít nhất 1 kết nối(TCP) đến server
- Luồng nhận dữ liệu: có một luồng nhận chính chỉ lắng nghe yêu cầu của server, nếu có yêu cầu mới thì tạo luồng khác xử lý yêu cầu đó
- Luồng gửi dữ liệu: nếu tác vụ gửi dữ liệu kéo dài(như gửi file lên server) thì tạo luồng khác để xử lý yêu cầu đó
### Định nghĩa opcode
#### 1xx, 2xx: server gửi tín hiệu cho client
#### 3xx, 4xx: client gửi tín hiệu cho server
#### SERVER:
- 100: kết nối thành công, gửi ID lại client
- 110: gửi danh sách các client đang kết nối
- 111: gửi danh sách các client có file yêu cầu
- 112: tải file từ server về client
- 120: gửi yêu cầu tìm kiếm file đến client
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
- 400: gửi ID của người cần chuyển tiếp file đến, chuẩn bị truyền file lên server
- 401: yêu cầu chuyển tiếp file và tải file lên server, lần tải đầu tiên sẽ tải tên file lên server
- 410: không cho chuyển tiếp file về client
- 411: cho phép chuyển tiếp file về client

## tài liệu tham khảo
- https://docs.microsoft.com/en-us/windows/win32/controls/user-controls-intro
- https://stackoverflow.com/questions/7598067/how-to-create-a-windows-style-textbox-in-a-c-win32-application
