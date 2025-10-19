<!DOCTYPE html>
<html lang="vi">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>README - Thread Operations (Luồng trong C)</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            line-height: 1.6;
            background-color: #f9f9f9;
            color: #222;
            margin: 20px;
        }
        h1, h2, h3 {
            color: #0056b3;
        }
        code {
            background-color: #eee;
            padding: 3px 6px;
            border-radius: 4px;
            font-family: Consolas, monospace;
        }
        pre {
            background-color: #eee;
            padding: 10px;
            border-radius: 6px;
            overflow-x: auto;
        }
        .note {
            background-color: #ffffcc;
            border-left: 4px solid #ffcc00;
            padding: 10px;
            margin: 15px 0;
        }
    </style>
</head>
<body>

<h1>Thao tác với luồng - Thread</h1>

<h2>Chủ đề 1: Tạo và chạy luồng cơ bản</h2>

<p>Chương trình <strong>createThread.c</strong> tạo ra 2 luồng là <code>thread_name</code> và <code>thread_number</code> và in ra mỗi ID của nó.</p>

<h3>Vấn đề 1.1: Giải thích vai trò và các tham số chính của <code>pthread_create()</code> và <code>pthread_join()</code></h3>

<p><strong>Hàm <code>pthread_create()</code></strong> cho phép ta tạo một luồng mới trong chương trình.</p>

<pre><code>int pthread_create(
    pthread_t *thread,               // (1)
    const pthread_attr_t *attr,      // (2)
    void *(*start_routine)(void *),  // (3)
    void *arg                        // (4)
);
</code></pre>

<ul>
    <li><strong>pthread_t *thread:</strong> con trỏ đến biến kiểu <code>pthread_t</code>; đây là biến lưu định danh (ID) của thread được tạo ra. Sau khi gọi thành công, ID của thread mới sẽ được ghi vào biến này. Có thể dùng ID để join và hủy thread.</li>
    <li><strong>const pthread_attr_t *attr:</strong> Dùng để thiết lập các thuộc tính cho thread, như kích thước stack, mức độ ưu tiên, trạng thái tách rời (detached / joinable).</li>
    <li><strong>void *(*start_routine)(void *):</strong> Hàm mà thread mới sẽ bắt đầu thực thi. Hàm có dạng <code>void *function_name(void *arg)</code>. Hàm trả về kiểu con trỏ và tham số là <code>(void *arg)</code> để có thể truyền mọi loại dữ liệu.</li>
    <li><strong>void *arg:</strong> là 1 tham số truyền vào thread, có thể truyền bất kì kiểu dữ liệu nào bằng cách ép kiểu hoặc để <code>NULL</code>.</li>
</ul>

<p><strong>Cấu trúc của hàm <code>pthread_join()</code>:</strong></p>

<pre><code>int pthread_join(pthread_t thread, void **retval);
</code></pre>

<ul>
    <li><strong>pthread_t thread:</strong> ID của luồng đang chờ.</li>
    <li><strong>void **retval:</strong> là con trỏ trỏ đến một con trỏ để lưu kết quả trả về từ luồng mục tiêu. Nếu đặt <code>NULL</code>, kết quả sẽ không được lấy.</li>
</ul>

<hr>

<h2>Chủ đề 2: Vấn đề “Race condition” và giải pháp Mutex</h2>

<p>Chương trình cho phép ta quan sát kết quả khi chạy 1, 2 và 3 luồng cùng lúc vào một biến global được khởi tạo là <code>long long counter = 0</code> và quan sát hiện tượng race condition.</p>

<p><strong>Hiện tượng race condition</strong> là hiện tượng khi các luồng khác nhau cùng thực thi hay can thiệp vào một dữ liệu mà không có cơ chế đồng bộ, từ đó dẫn đến kết quả sai lệch. Giống như một cuộc đua xe khi các xe va chạm với nhau thì kết quả sẽ rất tệ.</p>

<h3>Vấn đề 2.1: Tại sao phải cần phải sử dụng Mutex trong bài toán này?</h3>

<p>Như ta đã đọc về khái niệm race condition, nguyên nhân là do sự can thiệp không kiểm soát giữa các luồng. <strong>Mutex</strong> làm nhiệm vụ giống như một cái khóa mà chỉ một luồng được quyền nắm giữ tại một thời điểm. Khi một luồng thực hiện xong, khóa mới được mở cho luồng khác.</p>

<p>Như bạn có thể thấy ở hình 2.1.1, với mỗi luồng thứ i thì sẽ được cấp khóa và sau khi thực hiện xong nhiệm vụ tăng biến đếm <code>counter</code> lên 1 thì nó mới <em>unlock</em> khóa.</p>

<p>Đó là kết quả sau khi ép khóa mutex cho mỗi luồng như hình 2.1.2.</p>

<h3>Vấn đề 2.2: Điều gì sẽ xảy ra nếu chúng ta bỏ qua việc sử dụng mutex?</h3>

<p>Nếu không sử dụng mutex thì các luồng sẽ không được tách biệt và cố gắng cùng tăng biến <code>counter</code>. Các luồng xen lẫn với nhau, không thể hoàn thành nhiệm vụ của mỗi luồng là tăng đúng 1 triệu lần biến <code>counter</code>. Hệ điều hành có thể chuyển đổi giữa các luồng bất kỳ lúc nào khiến kết quả cuối cùng sai lệch, không ổn định.</p>

<p>Dưới đây là kết quả khi không sử dụng mutex (hình 2.2.1).</p>

<hr>

<h2>Chủ đề 3: Đồng bộ hóa với Condition Variables (Mô hình Producer-Consumer)</h2>

<p>Chương trình xây dựng 2 luồng <code>producer</code> và <code>consumer</code> như ở hình 3.1 và 3.2, thể hiện cách hoạt động hiệu quả của luồng mà không cần chờ liên tục (busy-waiting) như khi chỉ dùng Mutex.</p>

<p><strong>Phương pháp Condition Variable</strong> cho phép đặt một “interrupt” vào vòng lặp để biết khi nào dữ liệu được truyền xong. Lệnh <code>pthread_cond_wait(&cond, &lock)</code> cho phép consumer nhận tín hiệu từ producer để biết rằng dữ liệu đã sẵn sàng.</p>

<h3>Thứ tự hoạt động của hàm producer:</h3>

<ol>
    <li>Bắt đầu từ phần tử <code>i = 0</code>, gọi <code>pthread_mutex_lock(&lock)</code> để cấp khóa.</li>
    <li>Dữ liệu được sinh ngẫu nhiên (<code>rand()</code>) và tín hiệu cờ được xác nhận.</li>
    <li>Gọi <code>pthread_cond_signal(&cond)</code> để phát tín hiệu cho consumer.</li>
    <li>Mở khóa bằng <code>pthread_mutex_unlock(&lock)</code>.</li>
</ol>

<h3>Thứ tự hoạt động của hàm consumer:</h3>

<ol>
    <li>Bắt đầu từ <code>i = 0</code>, gọi <code>pthread_mutex_lock(&lock)</code> để được cấp khóa.</li>
    <li>Đưa vào vòng lặp <code>while</code> để chờ tín hiệu từ producer. Dùng <code>pthread_cond_wait(&cond, &wait)</code> để chờ tín hiệu và kiểm tra <code>data_ready == 1</code>.</li>
    <li>Gọi <code>pthread_cond_signal(&cond)</code> để gửi lại tín hiệu cho producer.</li>
    <li>Sau khi hoàn thành nhiệm vụ, mở khóa bằng <code>pthread_mutex_unlock(&lock)</code>.</li>
</ol>

<div class="note">
    <strong>Chú ý:</strong> Trong hàm producer có thể xảy ra hiện tượng <em>deadlock</em> nếu không cẩn thận. Deadlock xảy ra khi producer và consumer cùng chờ nhau do khóa chưa được giải phóng. Nếu gỡ comment đoạn <code>while</code> trong producer, chương trình có thể rơi vào trạng thái deadlock như hình 3.3.
</div>

<hr>

<h2>Chủ đề 4: Tối ưu hóa truy cập với Read-Write Lock</h2>

<p>Trong các hệ thống có tần suất đọc cao hơn ghi, sử dụng Mutex có thể gây tắc nghẽn không cần thiết. <strong>Read-Write Lock</strong> là cơ chế khóa cho phép nhiều luồng đọc đồng thời nhưng chỉ một luồng ghi.</p>

<p>Read-Write Lock cho phép nhiều luồng đọc cùng lúc mà không gây xung đột. Điều làm nó tối ưu hơn Mutex là Mutex chỉ cho một luồng hoạt động tại một thời điểm, khiến xử lý bị chậm nếu dữ liệu lớn.</p>

<p>Ví dụ, khi thao tác trên một cơ sở dữ liệu lớn (bảng tra cứu), Read-Write Lock giúp các luồng đọc chạy song song, còn luồng ghi thì vẫn giữ quyền độc quyền. Nói cách khác, <strong>Reader chạy song song, nhưng Writer là độc quyền.</strong></p>

<p>Đoạn chương trình sẽ cho ra kết quả như hình minh họa cuối cùng.</p>

</body>
</html>
