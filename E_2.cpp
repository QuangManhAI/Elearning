#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
using namespace std;
using namespace chrono;

void quayLuiDeQuy(
    int i,
    int soDinh,
    int maTran[15][15],
    int danhDauTham[15],
    int loTrinhTam[15],
    int loTrinhTotNhat[15],
    int &chiPhiTotThieu,
    int chiPhiHienTai
) {
    if (i == soDinh) {
        int tong = chiPhiHienTai + maTran[loTrinhTam[soDinh - 1]][0];
        if (tong < chiPhiTotThieu) {
            chiPhiTotThieu = tong;
            for (int k = 0; k < soDinh; k++)
                loTrinhTotNhat[k] = loTrinhTam[k];
        }
        return;
    }

    for (int j = 1; j < soDinh; j++) {
        if (!danhDauTham[j]) {
            danhDauTham[j] = 1;
            loTrinhTam[i] = j;
            quayLuiDeQuy(i + 1, soDinh, maTran, danhDauTham, loTrinhTam, loTrinhTotNhat, chiPhiTotThieu,
                         chiPhiHienTai + maTran[loTrinhTam[i - 1]][j]);
            danhDauTham[j] = 0;
        }
    }
}

void chayQuayLui(int soDinh, int maTran[15][15], ofstream &fout) {
    int danhDauTham[15] = {0};
    int loTrinhTam[15];
    int loTrinhTotNhat[15];
    int chiPhiTotThieu = 999999;

    loTrinhTam[0] = 0;
    danhDauTham[0] = 1;

    auto batDau = high_resolution_clock::now();

    quayLuiDeQuy(1, soDinh, maTran, danhDauTham, loTrinhTam, loTrinhTotNhat, chiPhiTotThieu, 0);

    auto ketThuc = high_resolution_clock::now();
    double thoiGian = duration<double, milli>(ketThuc - batDau).count();

    fout << "==== BACKTRACKING ====\n";
    fout << "Chi phi toi thieu: " << chiPhiTotThieu << endl;
    fout << "Lo trinh: ";
    for (int i = 0; i < soDinh; i++)
        fout << loTrinhTotNhat[i] << " ";
    fout << "0\n";
    fout << fixed << setprecision(6);
    fout << "Thoi gian: " << thoiGian << " ms\n";
}


void thamLam(int soDinh, int maTran[15][15], ofstream& fout) {
    auto batDau = high_resolution_clock::now();

    int danhDauTham[15] = {};
    int loTrinhTam[15];
    int batDauDinh = 0, chiPhi = 0, truocDo = batDauDinh, k = 0;

    danhDauTham[batDauDinh] = 1;
    loTrinhTam[k++] = batDauDinh;

    for (int step = 1; step < soDinh; step++) {
        int khoangCachMin = 999999, ganNhat = -1;
        for (int j = 0; j < soDinh; j++) {
            if (!danhDauTham[j] && maTran[truocDo][j] > 0 && maTran[truocDo][j] < khoangCachMin) {
                khoangCachMin = maTran[truocDo][j];
                ganNhat = j;
            }
        }
        if (ganNhat == -1) break;
        danhDauTham[ganNhat] = 1;
        loTrinhTam[k++] = ganNhat;
        chiPhi += khoangCachMin;
        truocDo = ganNhat;
    }

    chiPhi += maTran[truocDo][batDauDinh];
    loTrinhTam[k++] = batDauDinh;

    auto ketThuc = high_resolution_clock::now();
    double thoiGian = duration<double, milli>(ketThuc - batDau).count();

    fout << "==== GREEDY ====\n";
    fout << "Chi phi: " << chiPhi << endl;
    fout << "Lo trinh: ";
    for (int i = 0; i < k; i++) fout << loTrinhTam[i] << " ";
    fout << "\n";
    fout << fixed << setprecision(6);
    fout << "Thoi gian: " << thoiGian << " ms\n";
}

void quyHoachDong(int soDinh, int maTran[15][15], ofstream& fout) {
    auto batDau = high_resolution_clock::now();

    const int voCung = 999999;
    int bangDP[1 << 15][15], bangTruyVet[1 << 15][15];

    int matNaMax = 1 << soDinh;
    for (int mask = 0; mask < matNaMax; mask++)
        for (int i = 0; i < soDinh; i++)
            bangDP[mask][i] = voCung, bangTruyVet[mask][i] = -1;

    bangDP[1][0] = 0;

    for (int mask = 1; mask < matNaMax; mask++) {
        for (int u = 0; u < soDinh; u++) {
            if (!(mask & (1 << u))) continue;
            if (bangDP[mask][u] == voCung) continue;
            for (int v = 0; v < soDinh; v++) {
                if (mask & (1 << v)) continue;
                int nextMask = mask | (1 << v);
                int chiPhiMoi = bangDP[mask][u] + maTran[u][v];
                if (chiPhiMoi < bangDP[nextMask][v]) {
                    bangDP[nextMask][v] = chiPhiMoi;
                    bangTruyVet[nextMask][v] = u;
                }
            }
        }
    }

    int fullMask = matNaMax - 1, chiPhiTot = voCung, lastCity = -1;
    for (int i = 1; i < soDinh; i++) {
        int tong = bangDP[fullMask][i] + maTran[i][0];
        if (tong < chiPhiTot) {
            chiPhiTot = tong;
            lastCity = i;
        }
    }

    int mask = fullMask, city = lastCity, loTrinhQHD[15], pos = soDinh - 1;
    while (pos >= 0) {
        loTrinhQHD[pos--] = city;
        int prev = bangTruyVet[mask][city];
        if (prev == -1) break;
        mask ^= (1 << city);
        city = prev;
    }

    auto ketThuc = high_resolution_clock::now();
    double thoiGian = duration<double, milli>(ketThuc - batDau).count();

    fout << "==== DYNAMIC PROGRAMMING ====\n";
    fout << "Chi phi toi thieu: " << chiPhiTot << endl;
    fout << "Lo trinh: ";
    for (int i = 0; i < soDinh; i++) fout << loTrinhQHD[i] << " ";
    fout << "0\n";
    fout << fixed << setprecision(6);
    fout << "Thoi gian: " << thoiGian << " ms\n";
}


bool docMotMaTran(ifstream& fin, int& soDinh, int maTran[15][15]) {
    if (!(fin >> soDinh)) return false; 
    for (int i = 0; i < soDinh; i++)
        for (int j = 0; j < soDinh; j++)
            fin >> maTran[i][j];
    return true;
}


int main() {
    ifstream fin("input.txt");
    ofstream fout("output.txt");

    int soDinh;
    int maTran[15][15];
    int testCase = 1;

    while (docMotMaTran(fin, soDinh, maTran)) {
        fout << "=== TEST CASE " << testCase++ << " (" << soDinh << " dinh) ===\n\n";

        chayQuayLui(soDinh, maTran, fout);
        fout << endl;

        thamLam(soDinh, maTran, fout);
        fout << endl;

        quyHoachDong(soDinh, maTran, fout);
        fout << endl;
    }

    fin.close();
    fout.close();
    cout << "Da xu ly xong tat ca test case.\n";
    return 0;
}
