BT4. 

Đếm Số Chẵn và Số Lẻ với Threads
Viết một chương trình để thực hiện các bước sau:
Tạo một mảng ngẫu nhiên chứa 100 số nguyên từ 1 đến 100.
Tạo hai threads: một thread đếm số chẵn và một thread đếm số lẻ trong mảng.
Khi hai threads hoàn tất việc đếm, in ra tổng số số chẵn và tổng số số lẻ.
Sử dụng pthread_join để đảm bảo rằng chương trình chỉ kết thúc khi cả hai threads hoàn thành nhiệm vụ.
Gợi ý: Mỗi thread sẽ xử lý một phần công việc khác nhau, vì vậy không cần mutex trong bài này.


BT5. 

Chia Sẻ Tài Nguyên Với Read-Write Lock
Viết một chương trình mô phỏng tình huống có nhiều thread đọc và ghi dữ liệu:
Tạo một biến data chứa một số nguyên.
Tạo 5 threads đọc và 2 threads ghi:
Các thread đọc sẽ chỉ đọc data mà không thay đổi.
Các thread ghi sẽ tăng giá trị của data lên 1.
Sử dụng read-write lock để đảm bảo nhiều threads có thể đọc cùng lúc, nhưng chỉ một thread được ghi tại bất kỳ thời điểm nào.
In ra giá trị của data sau khi tất cả các threads hoàn thành.
Gợi ý: Sử dụng pthread_rwlock_rdlock và pthread_rwlock_wrlock cho đọc-ghi đồng bộ.

BT6. 

Tính Tổng Mảng với Threads và Mutex
Viết một chương trình tính tổng một mảng lớn gồm 1 triệu số nguyên.
Chia mảng thành 4 phần bằng nhau.
Tạo 4 threads, mỗi thread tính tổng một phần của mảng.
Sử dụng một biến tổng toàn cục và mutex để tổng hợp kết quả từ tất cả các threads.
In ra kết quả tổng của mảng sau khi các threads hoàn thành.
Gợi ý: Sử dụng pthread_mutex_lock để bảo vệ biến tổng khi các threads cộng kết quả của chúng.
