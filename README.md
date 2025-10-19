
<!DOCTYPE html>
<html lang="vi">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Thao tác với luồng - Thread</title>
<style>
    body {
        font-family: Arial, Helvetica, sans-serif;
        line-height: 1.6;
        color: #222;
        background-color: #f9f9f9;
        margin: 40px;
    }
    h1, h2, h3 {
        color: #004aad;
    }
    h1 {
        border-bottom: 3px solid #004aad;
        padding-bottom: 8px;
    }
    pre, code {
        background-color: #f0f0f0;
        padding: 6px 10px;
        border-radius: 5px;
        font-family: Consolas, monospace;
    }
    pre {
        overflow-x: auto;
    }
    img {
        display: block;
        margin: 20px auto;
        border-radius: 6px;
        box-shadow: 0 0 6px rgba(0,0,0,0.1);
        max-width: 100%;
        height: auto;
    }
    ul, ol {
        margin-left: 30px;
    }
    p {
        margin: 10px 0;
    }
    .note {
        background: #fff8c4;
        border-left: 5px solid #ffb800;
        padding: 10px;
        margin: 15px 0;
    }
</style>
</head>
<body>

<h1>Thao tác với luồng - Thread</h1>

<h2>Chủ đề 1: Tạo và chạy luồng cơ bản</h2>

<p>Chương trình <strong>createThread.c</strong> tạo ra 2 luồng là <code>thread_name</code> và <code>thread_number</code> và in ra mỗi ID của nó.</p>

<h3>Vấn đề 1.1: Giải thích vai trò và các tham số chính của pthread_create() và pthread_join()</h3>

<p>-Hàm <code>pthread_create()</code> cho phép ta tạo một luồng mới trong chương trình</p>

<p>Cấu trúc của hàm <code>pthread_create()</code> là</p>

<pre><code>int pthread_create(
    pthread_t *thread,               // (1)
    const pthread_attr_t *attr,      // (2)
    void *(*start_routine)(void *),  // (3)
    void *arg                        // (4)
);
</code></pre>

<p>Tham số <strong>pthread_t *thread:</strong> con trỏ đến biến kiểu pthread_t; đây là biến lưu định danh(ID) của thread được tạo ra. Sau khi gọi thành công, ID của thread mới sẽ được ghi vào biến này. Có thể dùng ID để join và hủy thread</p>

<p>Tham số <strong>const_thread_attr_t *attr:</strong> Dùng để thiết lập các thuộc tính cho thread, như kích thước stack, mức độ ưu tiên, trạng thái tách rời (detached / joinable)</p>

<p>Tham số <strong>void *(start_routine)(void*):</strong> Hàm mà thread mới sẽ bắt đầu thực thi. Hàm có dạng là <code>void *function_name(void *arg)</code>, nó có dạng như vậy là vì:</p>

<ul>
<li>Hàm sẽ trả về kiểu giá trị con trỏ</li>
<li>Tham số truyền vào hàm là <code>(void *arg)</code> là để cho phép tất cả các loại kiểu dữ liệu mà không phân biệt</li>
</ul>

<p>Tham số <strong>void *arg:</strong> là 1 tham số truyền vào thread, ta có thể truyền bất kì kiểu dữ liệu nào bằng cách <code>(*void)variables</code> hoặc không truyền gì (<code>NULL</code>)</p>

<p>Cấu trúc của hàm <code>pthread_join()</code> là :</p>

<pre><code>int pthread_join(pthread_t thread //(1) , void **retval // (2)l)
</code></pre>

<p>Tham số <strong>pthread_t thread:</strong> ID của luồng đang chờ</p>

<p>Tham số <strong>void **retval:</strong> là 1 con trỏ trỏ đến 1 con trỏ để lưu kết quả về từ luồng mục tiêu. Nếu ta set NULL, kết quả sẽ không được lấy.</p>

<h2>Chủ đề 2: Vấn đề “Race condition” và giải pháp Mutex</h2>

<p>Chương trình cho phép ta quan sát kết quả khi chạy 1, 2 vả 3 luồng cùng lúc vào 1 biến global được khởi tạo là <code>long long counter = 0</code> và quan sát hiện tượng race condition</p>

<p>Hiện tượng race condition là 1 hiện tượng khi mà các luồng khác nhau đều cùng thực thi hay can thiệp vào 1 dữ liệu mà không có cơ chế đồng bộ từ đó dẫn đến kết quả mong muốn bị thay đổi, giống như 1 cuộc đua giữa các xe đua khi chúng va chạm với nhau thì kết quả sẽ rất tệ.</p>

<h3>Vấn đề 2.1: Tại sao phải cần phải sử dụng Mutex trong bài toán này ?</h3>

<p>Như ta đã đọc về khái niệm race condition thì ta thấy rằng lý do kết quả khác với mong muốn đó là do sự can thiệp không kiểm soát giữa các luồng khác nhau, vậy Mutex làm nhiệm vụ giống như 1 cái khóa do 1 “người canh giữ” phát cho mỗi luồng và chỉ cho phép truy cập tại 1 thời điểm, chỉ khi luồng thực hiện xong thì khóa mới được cấp cho luồng khác thực hiện. Như bạn có thể thấy ở hình 2.1.1 dưới đây, với mỗi luồng thư i thì sẽ được cấp khóa và sau khi thực hiện xong nhiệm vụ tăng biến đếm counter lên 1 thì nó mới unlock khóa.</p>

<img width="897" height="360" alt="Screenshot from 2025-10-19 15-09-11" src="https://github.com/user-attachments/assets/e2990638-cf49-4fc4-829c-e4fdcb222d18" />

<p>Đó là kết quả sau khi ép khóa mutex cho mỗi luồng như hình 2.1.2</p>

<img width="981" height="52" alt="Screenshot from 2025-10-19 15-11-31" src="https://github.com/user-attachments/assets/41448f0a-c0c7-452b-8f95-6b10344d39d1" />

<h3>Vấn đề 2.2: Điều gì sẽ xảy ra nếu chúng ta bỏ qua việc sử dụng mutex? Giải thích tại sao kết quả cuối cùng có thể không chính xác và không ổn định.</h3>

<p>Đối với ví dụ trong chương trình này thì ta có thể thấy rằng nếu không sử dụng mutex thì các luồng sẽ không được tách biệt ra mà cố gắng làm nhiệm vụ tăng biến counter với mỗi i (1,2,3,  …, 1000000) và chúng xen lẫn với nhau và không thể hoàn thành nhiệm vụ của mỗi luồng mà đáng lẽ ra là mỗi luồng phải tăng 1 triệu lần biến counter.Hệ điều hành có thể thực hiện chuyển đổi thực thi giữa các luồng bất kì khi nào khiến kết quả không được như ý muốn của người viết chương trình. Dưới đây là kết quả khi không sử dụng mutex (hình 2.2.1)</p>

<img width="713" height="52" alt="Screenshot from 2025-10-19 14-55-34" src="https://github.com/user-attachments/assets/6079a7a4-0ccc-46df-bc8a-104a71e14b2d" />

<h2>Chủ đề 3: Đồng bộ hóa với Condition Variables (Mô hình Producer-Consumer)</h2>

<p>Chương trình xây dựng 2 luồng producer và consumer như 2 hình 3.1 và 3.2 mà các bạn thấy ở dưới đây sẽ cho thấy cách hoạt động hiệu quả của 1 luồng mà không cần phải chờ liên tục (busy-waiting) nếu chỉ áp dụng mỗi phương pháp Mutex.</p>

<img width="948" height="705" alt="Screenshot from 2025-10-19 16-40-47" src="https://github.com/user-attachments/assets/458e025b-dd86-4c5a-8db3-0ec6d5512a0c" />
<img width="947" height="570" alt="Screenshot from 2025-10-19 16-40-58" src="https://github.com/user-attachments/assets/eb1431c5-79bb-4fa5-9a74-a9ba1a50826b" />

<p>Phương pháp Condition variable cho phép ta có thể đặt 1 “interrupt” vào vòng lặp để biết khi nào dữ liệu được truyền xong giống như hình 3.2 ở trên. Lệnh pthread_cond_wait(&cond, &lock) cho phép consumer nhận tín hiệu từ producer để biết là hàm producer đã “produce” xong data chưa. Để có thể mô tả chính xác logic của từng hàm thì ta sẽ đề cập về thứ tự hoạt động của hàm producer:</p>

<p>+Bắt đầu từ phần tử i = 0,  ta gọi hàm pthread_mutex_lock(&lock) để được cấp khóa để tránh context switch(sự chồng chéo luồng)<br>
+data sẽ được rand(ngẫu nhiên) và tín hiệu cờ (flag) sẽ được xác nhận<br>
+ Hàm pthread_cond_signal sẽ trỏ đến địa chỉ của biến cond cũng chính giá trị điều kiện khởi tạo của luồng để phát tín hiệu<br>
+ Vì nó đã thực hiện xong, ta cần mở khóa bằng lệnh pthread_mutex_unlock(&lock)</p>

<p>Thứ tự hoạt động của hàm consumer là:<br>
	+ Bắt đầu từ phần tử i = 0; ta gọi hàm pthread_mutex_lock(&lock) để được cấp khóa để tránh context switch(sự chồng chéo luồng)<br>
	+ Ta đưa nó vào 1 vòng lặp, (ở đây ta phải sử dụng while để xác nhận tín hiệu cờ mà producer đã đưa cho hàm consumer vì nếu gặp phải trường hợp mà tín hiệu được gửi nhưng chưa thỏa mãn tín hiệu cờ = 1 (sử dụng for) thì kết quả mong muốn có thể sai). Trong vòng lặp ta sử dụng lệnh pthread_cond_wait(&cond, &wait) để trỏ tới địa chỉ của biến cond và wait được khởi tạo toàn cục. Nếu nhận được cùng với tín hiệu cờ (data_ready = 1) thì sẽ thoát khỏi vòng lặp<br>
	+ Hàm pthread_cond_signal(&cond) sẽ tạo lại tín hiệu của consumer đã xác định để gửi lại cho producer<br>
	+ Sau khi đã hoàn thành nhiệm vụ thì sủ dụng hàm pthread_mutex_unlock(&lock) để mở khóa để thực hiện tiếp nhiệm vụ.</p>

<p>Kết quả sẽ giống như hình 3.3 dưới đây:</p>

<img width="974" height="664" alt="Screenshot from 2025-10-19 16-33-52" src="https://github.com/user-attachments/assets/2124af49-8968-4eb3-84e6-7e9dcc6fb5d4" />

<p>*Chú ý: Trong hàm producer ta cần chú ý 1 hiện tượng deadlock trong khi viết chương trình, deadlock xảy ra nếu bạn gỡ comment ở đoạn vòng lặp while thì bạn sẽ gặp phải trường hợp mà lúc đó hàm producer sẽ khóa hàm consumer vì điều kiện data_ready thì nó sẽ luôn chờ, trong khi bên consumer cũng đang khóa hàm producer bằng điều kiện cờ và pthread_mutex_lock() . Kết quả của deadlock sẽ như hình 3.4 dưới đây</p>

<img width="760" height="178" alt="Screenshot from 2025-10-19 17-15-41" src="https://github.com/user-attachments/assets/681cdd2f-8463-41f3-88ee-f50ecd32943a" />

<h2>Chủ đề 4: Tối ưu hóa truy cập với Read-Write Lock</h2>

<p>Trong các hệ thống có tần suất đọc dữ liệu cao hơn nhiều so với ghi, sử dụng Mutex có thể gây tắc nghẽn không cần thiết. Read-Write Lock là một cơ chế khóa chuyên biệt, cho phép nhiều luồng đọc đồng thời nhưng vẫn đảm bảo tính độc quyền cho luồng ghi.<br>
Read-Write Lock là một cơ chế khóa cao cấp nó cho phép nhiều luồng được phép đọc đồng thời nhưng chỉ một luồng được ghi để tránh xung đột dữ liệu. Điều đặc biệt là có nhiều luồng đọc cùng lúc nhưng không gây ra xung đột (context switch). Điều làm nó tối ưu hơn Mutex đó chính là Mutex thì chỉ 1 luồng được đọc và những luồng khác phải chờ nó khiến việc xử lí trở nên chậm chạp hơn. Nếu gặp 1 cơ sở dữ liệu hay 1 cấu trúc dữ liệu lớn ví dụ như bảng tra cứu thì nó sẽ rất chậm vì phải đọc từng cái. Nói đơn giản Reader chạy song song, nhưng Writer phải là độc quyền. Đoạn chương trình sẽ cho ra kết quả dưới đây: </p>

<img width="773" height="711" alt="Screenshot from 2025-10-19 18-21-00" src="https://github.com/user-attachments/assets/3c289cdb-41dd-4db5-a454-8166121386b1" />

</body>
</html>

