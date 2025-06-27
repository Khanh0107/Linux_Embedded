# Question 1: Khởi tạo và Theo dõi Process
Mô tả: Viết một chương trình C tạo một process con bằng cách sử dụng fork(). Sau khi tạo process, tiến trình con sẽ in ra PID của nó và tiến trình cha in ra PID của tiến trình con.

# Question 2: Sử dụng exec và Biến Môi Trường
Mô tả: Viết một chương trình trong đó tiến trình con sẽ thay thế chương trình đang chạy bằng một chương trình khác, ví dụ ls hoặc date, sử dụng hàm execlp() hoặc execvp().

**Yêu cầu:**

- Truyền các biến môi trường vào chương trình và thực hiện các công việc khác nhau (ví dụ biến môi trường được truyền vào là 1 thì thực hiện lệnh ls, nếu là 2 thì thực hiện lệnh date).

- Giải thích điều gì xảy ra với tiến trình ban đầu sau khi exec được gọi
