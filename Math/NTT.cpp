#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1 << 20;
const int pr[3] = {1004535809, 1007681537, 1012924417}; //2 ^ 20 * {958, 961, 966} + 1
const int pw[3] = {3, 3, 5}; //primitive roots

int add(int a, int b, int p = (int) 1e9 + 7) {return a + b - (a + b >= p) * p;}
int sub(int a, int b, int p = (int) 1e9 + 7) {return a - b + (a - b < 0) * p;}
int mul(int a, int b, int p = (int) 1e9 + 7) {return ((long long) a * b) % p;}
int fpow(int a, int k, int p = (int) 1e9 + 7) {int r = 1; while (k) {if (k & 1) r = mul(r, a, p); k >>= 1; a = mul(a, a, p);} return r;}
int inv(int a, int p = (int) 1e9 + 7) {return fpow(a, p - 2, p);}

void fft(int a[], int p, int r, int n) {
    for (int m = n, h; h = m / 2, m >= 2; r = (long long) r * r % p, m = h) {
        for (int i = 0, w = 1; i < h; i++, w = (long long) w * r % p) {
            for (int j = i; j < n; j += m) {
                int k = j + h;
                int x = a[j] - a[k];
                if (x < 0) x += p;
                a[j] += a[k];
                if (a[j] >= p) a[j] -= p;
                a[k] = (long long) w * x % p;
            }
        }
    }
    for (int i = 0, j = 1; j < n - 1; j++) {
        for (int k = n / 2; k > (i ^= k); k /= 2);
        if (j < i) swap(a[i], a[j]);
    }
}
void multiply(int a[], int b[], int c[], int na, int nb, int p, int prt) {
    static int fa[MAXN], fb[MAXN];
    int len = na + nb - 1;
    int k = 0; while ((1 << k) < na + nb - 1) k++;
    for (int i = 0; i < (1 << k); i++) fa[i] = fb[i] = 0;
    for (int i = 0; i < na; i++) fa[i] = a[i];
    for (int i = 0; i < nb; i++) fb[i] = b[i];
    int w = fpow(prt, (p - 1) / (1 << k), p);
    fft(fa, p, w, 1 << k);
    fft(fb, p, w, 1 << k);
    for (int i = 0; i < (1 << k); i++) c[i] = mul(fa[i], fb[i], p);
    fft(c, p, inv(w, p), 1 << k);
    int rem = inv(1 << k, p);
    for (int i = 0; i < (1 << k); i++) c[i] = mul(c[i], rem, p);
}
void multiply(int a[], int b[], int c[], int na, int nb, int mod) {
    static int fc[3][MAXN];
    for (int r = 0; r < 3; r++) multiply(a, b, fc[r], na, nb, pr[r], pw[r]);
    for (int i = 0; i < na + nb - 1; i++) {
        long long cur1 = (long long) mul(sub(fc[1][i], fc[0][i], pr[1]), inv(pr[0], pr[1]), pr[1]) * pr[0] + fc[0][i];
        long long cur2 = mul(sub(fc[2][i], cur1 % pr[2], pr[2]), inv(mul(pr[0], pr[1], pr[2]), pr[2]), pr[2]);
        cur2 = add(mul(cur2 % mod, mul(pr[0], pr[1], mod), mod), cur1 % mod, mod);
        c[i] = cur2;
    }
}

const int maxn = 1 << 20;
int a[maxn];
int b[maxn];
int c[maxn << 1];
int d[maxn << 1];

int main() {
    srand(time(NULL));
    int n = rand() % 10000, p = (int) 1e9 + 7;
    for (int i = 0; i < n; i++) a[i] = rand();
    for (int i = 0; i < n; i++) b[i] = rand();
    multiply(a, b, c, n, n, p);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            d[i + j] = (d[i + j] + (long long) a[i] * b[j]) % p;
        }
    }
    for (int i = 0; i < (n << 1) - 1; i++) {
        assert(c[i] == d[i]);
    }
    cout << "Correct!\n";
    return 0;
}
