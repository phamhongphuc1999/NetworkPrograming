# bài tập lớn môn lập trình mạng
## Server có các chức năng sau:
- Nhận yêu cầu tìm kiếm file (theo tên file) của client nào đó
- Gửi lệnh tìm kiếm tới các client khác đang kết nối
- Gửi lại cho client danh sách các client có file mà client tìm kiếm
- Chuyển tiếp file khi client yêu cầu 
## Client có các chức năng sau:
- Gửi yêu cầu tìm kiếm một file lên server
- Nhận danh sách  các client có file mà client yêu cầu tìm kiếm
- Lựa chọn một client trong kết quả server gửi về để yêu cầu download file

## Khuôn dạng gói tin: định dạng mảng char
#### Có bốn trường opcode, length, data
- opcode: 3 phần tử đầu trong mảng, chỉ định chức năng sẽ thực hiện
- offset: 1 phần tử tiếp theo, là phần mở rộng chức năng cho opcode
- length: 10 phần tử tiếp theo trong mảng, chứa kích thức của data
- data: phần còn lại của gói tin gửi đi, chứa dữ liệu cần truyền

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
- 201: tải file chuyển tiếp về client, 
       lần đầu tải tên file có offset bằng 0, 
       các lần sau là dữ liệu của file, lần cuối không mang dữ liệu để thông báo kết thúc có offset bằng 1
- 202: ID do client gửi lên có thể kết nối và được phép chuyển tiếp
- 203: ID do client gửi lên không thể kết nối hoặc bị từ chối chuyển tiếp
#### CLIENT:
- 300: yêu cầu kết nối và gửi ID
- 310: gửi yêu cầu tìm kiếm file
- 311: tải file từ client được chỉ định bởi client khác lên server
- 320: không tìm thấy tên file trong trường data
- 321: tìm thấy tên file trong trường data
- 400: gửi ID của người nhận có offset bằng 0
       sau đó gửi fileName có offset bằng 1
- 401: gửi lần lượt từng gói dữ liệu đến server, lần cuối không có dữ liệu để thông báo kết thúc
- 410: không cho chuyển tiếp file về client
- 411: cho phép chuyển tiếp file về client

## tài liệu tham khảo
- https://docs.microsoft.com/en-us/windows/win32/controls/user-controls-intro
- https://stackoverflow.com/questions/7598067/how-to-create-a-windows-style-textbox-in-a-c-win32-application
- https://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path
- https://stackoverflow.com/questions/2886609/how-to-make-multiple-windows-using-win32-api#:~:text=5%20Answers&text=You%20can%20hit%20CreateWindow(),1st%20one%20if%20you%20want.
