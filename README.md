# SỬ DỤNG:
* Mở terminal dùng lệnh `cd /` để di chuyển tới thư mục cần chứa file code, input.txt, output.txt.
* Ví dụ trên `Linux` hay `macOS`: 
```
cd home/quangmanh/Documents/c++
```
* Dùng lệnh sau tải tòan bộ(máy phải có git):
```
git clone https://github.com/QuangManhAI/Elearning
```
* Kiểm tra version c++ `linux`:
```
g++ --version
```
rồi quy đổi mã phiên bản:
| Phiên bản GCC | Chuẩn C++ được hỗ trợ ổn định       |
|----------------|-------------------------------------|
| GCC 4.8 – 4.9  | C++11                              |
| GCC 5.x – 6.x  | C++14                              |
| GCC 7.x – 9.x  | C++17                              |
| GCC 10.x – 11.x| C++20 (đa số tính năng)            |
| GCC 12.x – 14.x| C++23 (rất đầy đủ)                 |

* Chạy chường trình tại thư mục hiện hành `pwd`:
```
g++ E_2.cpp -o E_2
```
**và** 
```
./E_2
```
# Traveling Salesman Problem (TSP)

## Mục tiêu

Tìm hành trình có tổng chi phí nhỏ nhất đi qua **mỗi đỉnh đúng một lần** và quay lại **điểm xuất phát (đỉnh 0)**. Tài liệu này **tập trung mô tả chiến lược thuật toán** (ý tưởng, cách vận hành, phân tích độ phức tạp, bẫy thường gặp) và đưa **ví dụ minh họa**. Phần code chỉ trích dẫn rất ngắn khi cần làm rõ ý tưởng.

---

## Đầu vào/Đầu ra (tóm tắt)

* **Input**: nhiều test case. Mỗi test: số đỉnh `n` (≤ 15), sau đó là ma trận chi phí `n×n` (0 trên đường chéo).
* **Output**: cho mỗi test case, in kết quả ba phương pháp: **Backtracking**, **Greedy**, **Dynamic Programming (Held–Karp)** – gồm **chi phí**, **lộ trình**, **thời gian**.

---

## Chiến lược 1 — Backtracking (Quay lui toàn không gian nghiệm)

### Ý tưởng

* Xây dựng tuyến theo từng vị trí 1→2→…→n.
* Ở vị trí `i`, chọn một đỉnh **chưa thăm** để nối tiếp từ đỉnh trước đó.
* Khi đủ `n` đỉnh, cộng chi phí **quay về 0** để khép vòng; cập nhật lời giải tốt nhất.

### Trạng thái & chuyển

* **Trạng thái**: `(i, visited, last)` — đã đặt `i` đỉnh, tập `visited` các đỉnh đã dùng, và đang đứng tại `last`.
* **Chuyển**: thử từng `next` chưa có trong `visited`, cộng chi phí `cost(last, next)` và đệ quy.

### Cắt tỉa (pruning) quan trọng

1. **Cận trên tốt nhất** (best-so-far): nếu chi phí tích lũy hiện tại ≥ chi phí tốt nhất, **ngừng nhánh**.
2. **Cận dưới đơn giản**: cộng thêm **ước lượng tối thiểu** để đi nốt các đỉnh chưa thăm (ví dụ: mỗi đỉnh dùng cạnh rẻ nhất của nó). Nếu `chi_phí_hiện_tại + cận_dưới ≥ best`, cắt nhánh.
3. **Thứ tự mở rộng**: thử các đỉnh theo **cạnh rẻ trước** để nhanh có best nhỏ → tăng hiệu quả cắt tỉa.

> Gợi ý cận dưới nhanh: `LB = sum(min_out[u] cho u∈(đỉnh chưa thăm ∪ {last})) + min_in[0]` — đủ đơn giản, dễ tính.

### Độ phức tạp

* Trường hợp xấu: **O(n!)**. Với cắt tỉa tốt, chạy được n≈11–12 (tùy dữ liệu).

### Ví dụ nhỏ (n=4)

```
0 10 15 20
10 0 35 25
15 35 0 30
20 25 30 0
```

* Nhánh 0→1→3→2→0: chi phí = 10 + 25 + 30 + 15 = **80** (tối ưu).
* Nhánh 0→2… sẽ bị loại sớm nếu cận dưới + chi phí hiện tại ≥ 80.

---

## Chiến lược 2 — Greedy (Heuristic gần nhất kế tiếp)

### Ý tưởng (Nearest Neighbor)

* Bắt đầu ở đỉnh 0.
* Ở mỗi bước, **chọn đỉnh chưa thăm gần nhất** từ vị trí hiện tại.
* Khi thăm đủ, **đóng vòng** quay về 0.

### Quy tắc ràng buộc & xử lý hòa (tie-breaking)

* Bỏ qua cạnh không khả dụng (hoặc chi phí 0 ngoài đường chéo).
* Nếu nhiều đỉnh cùng khoảng cách tối thiểu: chọn theo **chỉ số nhỏ nhất** hoặc **ổn định** theo thứ tự ban đầu để kết quả tái lập.

### Ưu/nhược điểm

* **Ưu**: đơn giản, **rất nhanh** (O(n²)); phù hợp để dựng nghiệm khởi tạo hoặc bối cảnh thời gian thực.
* **Nhược**: không đảm bảo tối ưu; dễ **mắc kẹt cục bộ** (chọn sớm cạnh rẻ làm phần còn lại đắt đỏ).

### Ví dụ phản ví (n=5)

```
0  2  9 10  1
2  0  6  4  3
9  6  0  8  7
10 4  8  0  5
1  3  7  5  0
```

* Greedy từ 0: chọn 4 (1), rồi 1 (3), … → tổng **có thể** lớn hơn lời giải tối ưu.
* Đây là tình huống điển hình cho **bẫy cục bộ**.

### Biến thể gợi ý (nâng chất)

* **Nearest Insertion / Cheapest Insertion**: xây vòng dần bằng cách chèn đỉnh mới tại vị trí tăng chi phí nhỏ nhất.
* **2-Opt/3-Opt**: hoán đổi cạnh để **cải thiện sau** (local search), thường giảm mạnh chi phí so với Greedy thuần.

---

## Chiến lược 3 — Dynamic Programming (Held–Karp)

### Ý tưởng

* Dùng **bitmask** để mã hóa tập đỉnh đã thăm.
* `dp[mask][i]`: **chi phí tối thiểu** đi từ 0 → … → i, thăm đúng các đỉnh trong `mask` (mask luôn chứa bit 0).

### Công thức quy hoạch

* **Khởi tạo**: `dp[1<<0][0] = 0`.
* **Chuyển trạng thái**:

  `dp[mask|1<<v][v] = min_{u∈mask} dp[mask][u] + cost(u,v)`

  với mọi `v` chưa thuộc `mask`.
* **Kết thúc**:

  `ans = min_{i>0} dp[(1<<n)-1][i] + cost(i,0)`

### Truy vết lời giải

* Lưu mảng `prev[mask][i]` khi cập nhật `dp`, lần ngược từ `(mask_full, i*)` về `0` để thu lộ trình.

### Độ phức tạp & bộ nhớ

* Thời gian: **O(n²·2ⁿ)**; Bộ nhớ: **O(n·2ⁿ)**. Với n≤15 có thể chấp nhận (tùy ràng buộc thời gian).

### Ví dụ minh họa (n=4)

* Mặt nạ tăng dần: `{0,1}`, `{0,2}`, `{0,3}`, …, `{0,1,2,3}`.
* Sau khi điền hết `dp`, lấy `min(dp[1111][i]+cost(i,0))` để ra chi phí tối ưu (80 cho ví dụ 4×4 ở trên).

---

## Khi nào dùng phương pháp nào?

* **Backtracking**: cần **đúng tuyệt đối** và n **nhỏ**; có thể kết hợp nhiều cận dưới/cắt tỉa.
* **Greedy**: cần **rất nhanh**; dựng nghiệm ban đầu cho metaheuristic (2-Opt/3-Opt, Tabu, SA, GA…).
* **DP (Held–Karp)**: cần **đúng tuyệt đối** với n **vừa** (≈ 12–15); cân bằng giữa tối ưu và thời gian.

---

## Kết quả từ dữ liệu của bạn (tóm tắt chi phí)

| Test |  n | Backtracking | Greedy | DP (Held–Karp) |
| ---: | -: | -----------: | -----: | -------------: |
|    1 |  6 |          102 |    115 |            102 |
|    2 |  8 |          144 |    144 |            144 |
|    3 | 10 |          186 |    186 |            186 |
|    4 | 11 |          142 |    168 |            142 |
|    5 | 12 |          149 |    166 |            149 |

> Nhận xét nhanh: Greedy **trùng tối ưu** ở test 2–3 nhưng **lệch** ở test 1,4,5. DP khớp Backtracking (đúng tối ưu) với thời gian tốt hơn trên n lớn.

---

## Phụ lục — Mẫu định dạng I/O bạn đã dùng

### input.txt (rút gọn từ yêu cầu của bạn)

```
6
0 15 30 10 25 20
15 0 35 12 18 22
30 35 0 14 28 26
10 12 14 0 16 24
25 18 28 16 0 19
20 22 26 24 19 0
...
```

### output.txt (rút gọn từ kết quả bạn cung cấp)

```
=== TEST CASE 1 (6 dinh) ===

==== BACKTRACKING ====
Chi phi toi thieu: 102
Lo trinh: 0 1 4 5 2 3 0

==== GREEDY ====
Chi phi: 115
Lo trinh: 0 3 1 4 5 2 0

==== DYNAMIC PROGRAMMING ====
Chi phi toi thieu: 102
Lo trinh: 0 3 2 5 4 1 0
...
```

---

## Gợi ý mở rộng

* Cận dưới tốt hơn cho Backtracking: **MST 1-tree**/MST trên tập chưa thăm.
* Hậu xử lý Greedy: **2-Opt/3-Opt** để cải thiện nhanh.
* DP: giảm bộ nhớ bằng **rolling array** theo kích thước mask; chia nhỏ theo **subset size**.
