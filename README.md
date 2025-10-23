# Traveling Salesman Problem (TSP)

## Mô tả bài toán

Bài toán TSP (Travelling Salesman Problem) yêu cầu tìm hành trình ngắn nhất qua tất cả các đỉnh và quay trở lại điểm xuất phát, sao cho mỗi đỉnh được đi qua đúng một lần.

Chương trình này triển khai **3 chiến lược khác nhau**:

1. **Backtracking (Quay lui)** – duyệt toàn bộ các hoán vị, đảm bảo nghiệm tối ưu.
2. **Greedy (Tham lam)** – chọn thành phố gần nhất kế tiếp, chạy nhanh nhưng không đảm bảo tối ưu.
3. **Dynamic Programming (Quy hoạch động - Held-Karp)** – sử dụng bitmask để tối ưu, đảm bảo nghiệm tối ưu với độ phức tạp thấp hơn quay lui.

---

## Cấu trúc dự án

```
main.cpp          # Mã nguồn chính
input.txt         # Dữ liệu đầu vào (ma trận trọng số)
output.txt        # Kết quả các thuật toán
```

---

## Chiến lược và chú thích code

### 1. Backtracking (Quay lui)

```cpp
// Ý tưởng: Duyệt toàn bộ các hoán vị có thể của các thành phố.
// Với mỗi hoán vị, tính tổng chi phí di chuyển, lưu lại giá trị nhỏ nhất.
// Độ phức tạp: O(n!) – đảm bảo tìm được lời giải tối ưu.
void chayQuayLui(int soDinh, int maTran[15][15], ofstream& fout) {
    int danhDauTham[15] = {}; // mảng đánh dấu các đỉnh đã đi qua
    int loTrinhTam[15], loTrinhTotNhat[15];
    int chiPhiTotThieu = 999999;

    auto quayLui = [&](auto& self, int i, int chiPhiHienTai) -> void {
        if (i == soDinh) {
            int tong = chiPhiHienTai + maTran[loTrinhTam[soDinh - 1]][0];
            if (tong < chiPhiTotThieu) {
                chiPhiTotThieu = tong;
                for (int k = 0; k < soDinh; k++) loTrinhTotNhat[k] = loTrinhTam[k];
            }
            return;
        }
        for (int j = 1; j < soDinh; j++) {
            if (!danhDauTham[j]) {
                danhDauTham[j] = 1;
                loTrinhTam[i] = j;
                self(self, i + 1, chiPhiHienTai + maTran[loTrinhTam[i - 1]][j]);
                danhDauTham[j] = 0;
            }
        }
    };

    loTrinhTam[0] = 0;
    danhDauTham[0] = 1;
    quayLui(quayLui, 1, 0);

    fout << "==== BACKTRACKING ====\n";
    fout << "Chi phi toi thieu: " << chiPhiTotThieu << endl;
    fout << "Lo trinh: ";
    for (int i = 0; i < soDinh; i++) fout << loTrinhTotNhat[i] << " ";
    fout << "0\n";
}
```

### 2. Greedy (Tham lam)

```cpp
// Ý tưởng: Xuất phát từ đỉnh 0, ở mỗi bước chọn thành phố gần nhất chưa thăm.
// Nhanh, đơn giản, nhưng không đảm bảo tối ưu toàn cục.
// Độ phức tạp: O(n^2)
void thamLam(int soDinh, int maTran[15][15], ofstream& fout) {
    int danhDauTham[15] = {};
    int loTrinhTam[15];
    int batDau = 0, chiPhi = 0, hienTai = 0;

    danhDauTham[0] = 1;
    loTrinhTam[0] = 0;

    for (int i = 1; i < soDinh; i++) {
        int ganNhat = -1, kcMin = 999999;
        for (int j = 0; j < soDinh; j++) {
            if (!danhDauTham[j] && maTran[hienTai][j] < kcMin) {
                kcMin = maTran[hienTai][j];
                ganNhat = j;
            }
        }
        danhDauTham[ganNhat] = 1;
        loTrinhTam[i] = ganNhat;
        chiPhi += kcMin;
        hienTai = ganNhat;
    }
    chiPhi += maTran[hienTai][0];

    fout << "==== GREEDY ====\n";
    fout << "Chi phi: " << chiPhi << endl;
    fout << "Lo trinh: ";
    for (int i = 0; i <= soDinh; i++) fout << loTrinhTam[i % soDinh] << " ";
    fout << "\n";
}
```

### 3. Dynamic Programming (Held-Karp)

```cpp
// Ý tưởng: Dùng bitmask để lưu trạng thái tập các đỉnh đã đi qua.
// dp[mask][i] = chi phí nhỏ nhất khi đi đến i qua tập đỉnh mask.
// Độ phức tạp: O(n^2 * 2^n) – tối ưu hơn quay lui, cho kết quả chính xác.
void quyHoachDong(int soDinh, int maTran[15][15], ofstream& fout) {
    const int voCung = 999999;
    int dp[1 << 15][15];
    int truoc[1 << 15][15];

    int fullMask = 1 << soDinh;
    for (int mask = 0; mask < fullMask; mask++)
        for (int i = 0; i < soDinh; i++)
            dp[mask][i] = voCung, truoc[mask][i] = -1;

    dp[1][0] = 0; // bắt đầu từ đỉnh 0

    for (int mask = 1; mask < fullMask; mask++) {
        for (int u = 0; u < soDinh; u++) {
            if (!(mask & (1 << u))) continue;
            for (int v = 0; v < soDinh; v++) {
                if (mask & (1 << v)) continue;
                int newMask = mask | (1 << v);
                int newCost = dp[mask][u] + maTran[u][v];
                if (newCost < dp[newMask][v]) {
                    dp[newMask][v] = newCost;
                    truoc[newMask][v] = u;
                }
            }
        }
    }

    int best = voCung, last = -1;
    for (int i = 1; i < soDinh; i++) {
        int cost = dp[fullMask - 1][i] + maTran[i][0];
        if (cost < best) best = cost, last = i;
    }

    fout << "==== DYNAMIC PROGRAMMING ====\n";
    fout << "Chi phi toi thieu: " << best << endl;
}
```

---

## File input (input.txt)

```
6
0 15 30 10 25 20
15 0 35 12 18 22
30 35 0 14 28 26
10 12 14 0 16 24
25 18 28 16 0 19
20 22 26 24 19 0
8
0 12 23 34 45 21 32 18
12 0 17 28 39 25 30 22
23 17 0 19 31 27 24 20
34 28 19 0 16 33 29 26
45 39 31 16 0 35 40 38
21 25 27 33 35 0 14 19
32 30 24 29 40 14 0 13
18 22 20 26 38 19 13 0
10
0 11 22 33 44 55 21 32 43 25
11 0 13 24 35 46 27 38 49 30
22 13 0 15 26 37 28 39 40 31
33 24 15 0 17 28 29 30 41 32
44 35 26 17 0 19 20 21 42 33
55 46 37 28 19 0 11 12 43 34
21 27 28 29 20 11 0 13 24 35
32 38 39 30 21 12 13 0 25 36
43 49 40 41 42 43 24 25 0 37
25 30 31 32 33 34 35 36 37 0
11
0 14 23 31 18 25 33 27 19 24 30
14 0 13 24 15 22 30 24 16 21 27
23 13 0 11 18 19 27 21 13 18 24
31 24 11 0 21 20 18 14 16 22 19
18 15 18 21 0 17 25 19 11 16 22
25 22 19 20 17 0 13 10 12 18 14
33 30 27 18 25 13 0 11 13 19 15
27 24 21 14 19 10 11 0 10 16 12
19 16 13 16 11 12 13 10 0 13 9
24 21 18 22 16 18 19 16 13 0 11
30 27 24 19 22 14 15 12 9 11 0
12
0 13 22 31 18 25 33 27 19 24 30 21
13 0 11 24 15 22 30 24 16 21 27 18
22 11 0 13 18 19 27 21 13 18 24 15
31 24 13 0 21 20 18 14 16 22 19 12
18 15 18 21 0 17 25 19 11 16 22 13
25 22 19 20 17 0 13 10 12 18 14 15
33 30 27 18 25 13 0 11 13 19 15 16
27 24 21 14 19 10 11 0 10 16 12 13
19 16 13 16 11 12 13 10 0 13 9 10
24 21 18 22 16 18 19 16 13 0 11 12
30 27 24 19 22 14 15 12 9 11 0 10
21 18 15 12 13 15 16 13 10 12 10 0
```

---

## File output (output.txt)

```
=== TEST CASE 1 (6 dinh) ===

==== BACKTRACKING ====
Chi phi toi thieu: 102
Lo trinh: 0 1 4 5 2 3 0
Thoi gian: 0 ms

==== GREEDY ====
Chi phi: 115
Lo trinh: 0 3 1 4 5 2 0 
Thoi gian: 0 ms

==== DYNAMIC PROGRAMMING ====
Chi phi toi thieu: 102
Lo trinh:0 3 2 5 4 1 0
Thoi gian: 0 ms

=== TEST CASE 2 (8 dinh) ===

==== BACKTRACKING ====
Chi phi toi thieu: 144
Lo trinh: 0 1 2 3 4 5 6 7 0
Thoi gian: 0 ms

==== GREEDY ====
Chi phi: 144
Lo trinh: 0 1 2 3 4 5 6 7 0 
Thoi gian: 0 ms

==== DYNAMIC PROGRAMMING ====
Chi phi toi thieu: 144
Lo trinh:0 7 6 5 4 3 2 1 0
Thoi gian: 0 ms

=== TEST CASE 3 (10 dinh) ===

==== BACKTRACKING ====
Chi phi toi thieu: 186
Lo trinh: 0 1 2 3 4 5 6 7 8 9 0
Thoi gian: 22 ms

==== GREEDY ====
Chi phi: 186
Lo trinh: 0 1 2 3 4 5 6 7 8 9 0 
Thoi gian: 0 ms

==== DYNAMIC PROGRAMMING ====
Chi phi toi thieu: 186
Lo trinh:0 9 8 7 6 5 4 3 2 1 0
Thoi gian: 0 ms

=== TEST CASE 4 (11 dinh) ===

==== BACKTRACKING ====
Chi phi toi thieu: 142
Lo trinh: 0 1 2 3 7 5 6 8 10 9 4 0
Thoi gian: 150 ms

==== GREEDY ====
Chi phi: 168
Lo trinh: 0 1 2 3 7 5 8 10 9 4 6 0 
Thoi gian: 0 ms

==== DYNAMIC PROGRAMMING ====
Chi phi toi thieu: 142
Lo trinh:0 4 9 10 8 6 5 7 3 2 1 0
Thoi gian: 0 ms

=== TEST CASE 5 (12 dinh) ===

==== BACKTRACKING ====
Chi phi toi thieu: 149
Lo trinh: 0 1 2 3 11 9 10 5 6 7 8 4 0
Thoi gian: 1757 ms

==== GREEDY ====
Chi phi: 166
Lo trinh: 0 1 2 3 11 8 10 9 4 5 7 6 0 
Thoi gian: 0 ms

==== DYNAMIC PROGRAMMING ====
Chi phi toi thieu: 149
Lo trinh:0 4 8 7 6 5 10 9 11 3 2 1 0
Thoi gian: 0 ms
```

---

## Nhận xét

* Backtracking cho nghiệm chính xác nhưng thời gian tăng nhanh theo n.
* Greedy chạy nhanh nhất nhưng có thể sai lệch.
* DP là sự cân bằng: chính xác như backtracking, nhanh hơn đáng kể.
