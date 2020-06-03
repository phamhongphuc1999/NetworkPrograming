==================================================================================================
                                   Khuôn dạng gói tin
==================================================================================================
=============Định dạng mảng char(char*)
--- Có ba trường opcode, length, data
opcode: 3 phần tử đầu trong mảng, chỉ định chức năng sẽ thực hiện
length: 10 phần tử tiếp theo trong mảng, chứa kích thức của data
data: phần còn lại của gói tin gửi đi, chứ dữ liệu cần truyền
--- mỗi client có ít nhất 2 kết nối(TCP) đến server
*Kết nối 1: chỉ dùng để nhận yêu cầu mới của server
   --: có một luồng chính, khi có yêu cầu tới, tạo mới một luồng riêng xử lý yêu cầu đó
       luồng chính tiếp tục đợi yêu cầu từ server
*Kết nối 2: chỉ dùng để gửi yêu cầu tới server, sử dụng thread nếu có nhiều yêu cầu gửi tới server
--- Định nghĩa opcode
1, 2: server gửi tín hiệu cho client
3, 4: client gửi tín hiệu cho server
*SERVER:
100: kết nối thành công, gửi ID lại client
110: gửi danh sách các client đang kết nối
111: gửi danh sách các client có file yêu cầu
112: tải file từ server về client
120: gửi yêu cầu tìm kiếm file đến client
200: yêu cầu chuyển tiếp file về client
201: tải file chuyển tiếp về client
202: ID do client gửi lên có thể kết nối và được phép chuyển tiếp
203: ID do client gửi lên không thể kết nối hoặc bị từ chối chuyển tiếp
*CLIENT:
300: yêu cầu kết nối và gửi ID
310: gửi yêu cầu tìm kiếm file
311: tải file từ client được chỉ định bởi client khác lên server
320: không tìm thấy tên file trong trường data
321: tìm thấy tên file trong trường data
400: gửi ID của người cần chuyển tiếp file đến, chuẩn bị truyền file lên server
401: yêu cầu chuyển tiếp file và tải file lên server, lần tải đầu tiên sẽ tải tên file lên server
410: không cho chuyển tiếp file về client
411: cho phép chuyển tiếp file về client
