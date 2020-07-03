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
- Yêu cầu server chuyển tiếp file đến client khác
- Tìm kiếm file mà client khác yêu cầu(do là chương trình demo nên client sẽ tìm kiếm file trong folder Data nằm cùng thư mục với file exe của chương trình): nó sẽ tìm kiếm lần lượt các file trong folder Data, nếu trong Data còn chứa thư mục con, chương trình sẽ thực hiện tìm kiếm đệ quy vào các thư mục con đó

<pre> mỗi client có sau khi kết nối thành công đến server sẽ được server tạo một ID và gửi về cho client
 => nâng cấp chương trình bằng việc thay thế ID bằng username, password </pre>

## Khuôn dạng gói tin: định dạng struct
 <pre> struct Message {
   int type;
   int odcode;
   char fileName[BUFF_SIZE];
   char ID[BUFF_SIZE];
   char data[BUFF_SIZE + 1];
   }; </pre>
#### các trường trong struct có thể thay đổi chức năng tùy thuộc vào chức năng client hay server yêu cầu
#### Các chức năng chính:
- type: định nghĩa chức năng mà client, server phải thục hiện
- odcode: trường dự bị(hiện chỉ có chức năng chứa kích thức của trường data)
- fileName: lưu tên file
- ID: lưu ID của client
- data: hiện chỉ có chức năng lưu dữ liệu của file

### Định nghĩa type
#### 1xx, 2xx: server gửi tín hiệu cho client
#### 3xx, 4xx: client gửi tín hiệu cho server
#### SERVER:
- 100: kết nối thành công, gửi ID lại client
- 110: gửi danh sách các client đang kết nối
- 111: gửi danh sách các client có file yêu cầu trong chức năng tìm kiếm file
- 112: tải từng gói của file tìm kiếm về client đi kèm với kích thước của gói đó
- 1120: giống 112 nhưng là dấu hiệu kết thúc tải file về client
- 120: gửi yêu cầu tìm kiếm file đến client
- 121: gửi yêu cầu tải file lên server trong chức năng tìm kiếm file
- 200: yêu cầu chuyển tiếp file về client
- 201: tải từng gói file chuyển tiếp về client đi kềm với kích thước của gói đó
- 2010: giống 201 nhưng là dấu hiệu kết thúc tải file về client
- 202: ID do client gửi lên có thể kết nối và được phép chuyển tiếp
- 203: ID do client gửi lên không thể kết nối hoặc bị từ chối chuyển tiếp
#### CLIENT:
- 300: yêu cầu kết nối và yêu cầu server gửi ID
- 310: gửi yêu cầu tìm kiếm file
- 311: tải từng gói file từ client được chỉ định bởi client khác lên server đi kèm với kích thức gói đó
- 3110: giống 311 nhưng là dấu hiệu kết thúc tải file lên server
- 312: yêu cầu tải file từ client có ID trong phần data
- 320: không tìm thấy tên file trong trường data
- 321: tìm thấy tên file trong trường data
- 400: gửi ID và fileName để chuyển tiếp file đến client tương ứng với ID được gửi
- 401: gửi lần lượt từng gói file chuyển tiêp lên server đi kèm với kích thước của gói đó
- 4010: giống 401 nhưng là dấu hiệu kết thúc tải file lên server
- 410: không cho chuyển tiếp file về client
- 411: cho phép chuyển tiếp file về client

## tài liệu tham khảo
- https://docs.microsoft.com/en-us/windows/win32/controls/user-controls-intro
- https://stackoverflow.com/questions/7598067/how-to-create-a-windows-style-textbox-in-a-c-win32-application
- https://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path
- https://stackoverflow.com/questions/2886609/how-to-make-multiple-windows-using-win32-api#:~:text=5%20Answers&text=You%20can%20hit%20CreateWindow(),1st%20one%20if%20you%20want.
- https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox
- https://stackoverflow.com/questions/42046666/test-project-exe-has-triggered-a-breakpoint
- https://stackoverflow.com/questions/42438135/c-winapi-listbox-getting-selected-item-using-lb-getsel-lb-getcursel
- https://docs.microsoft.com/en-us/windows/win32/learnwin32/closing-the-window
