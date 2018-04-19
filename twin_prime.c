#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 素数配列のサイズ
#define N 10000000
// 素数配列の更新幅
#define D 1000

typedef unsigned long long MyNumType;
MyNumType prime[N+1] = {2, 3, 5};   // 生成した素数配列
MyNumType num_prime = 2;            // 生成した素数配列の末尾の添字(0始まり)
MyNumType sq_last_prime = 5*5;      // 末尾の素数の2乗

// ---------------- prototype ----------------
// max以下の素数の配列作成
void generate_prime_list(MyNumType max);
// x^(1/2)まで素数配列を拡張
void extend_prime_list(MyNumType x);

// 素数判定
// sq_last_prime >= x であること
int is_prime(MyNumType x);

// [start, end]区間にある最小の双子素数の組の小さい方の素数を返す
MyNumType get_min_twin_prime(MyNumType start, MyNumType end);
// [start, end]区間にある双子素数を昇順で列挙する
void show_twin_primes(MyNumType start, MyNumType end);

// オプションに-h,-helpを指定した時のメッセージ
void usage(const char* path_str);

// ---------------- implementation ----------------
int main(int argc, char const *argv[]) {
    // init
    MyNumType input_start = argc > 1 ? strtoull(argv[1], NULL, 10) : 200000;
    MyNumType input_end   = argc > 2 ? strtoull(argv[2], NULL, 10) : input_start + 200000;
    int list_mode = (argc > 3 && strcmp("-l", argv[3]) == 0);

    if (argc > 1 && (strcmp("-h", argv[1]) == 0 || strcmp("-help", argv[1]) == 0)) {
        usage(argv[0]);
        exit(0);
    }

    printf("start: %llu%s\n", input_start, argc > 1 ? "" : " (default)");
    printf("end  : %llu%s\n", input_end,   argc > 2 ? "" : " (default)");

    printf("\n*** Searching twin prime in [%llu, %llu]. ***\n\n", input_start, input_end);
    if (list_mode) {
        show_twin_primes(input_start, input_end);
    } else {
        MyNumType result = get_min_twin_prime(input_start, input_end);
        if (result) {
            printf("solution: %llu, %llu\n", result, result+2);
        } else {
            printf("There is no solution in the range, [%llu, %llu].\n", input_start, input_end);
        }
    }

    return 0;
}

void generate_prime_list(MyNumType max) {
    MyNumType i;
    int d = (num_prime % 2) ? 4 : 2;

    for (i = prime[num_prime] + d; i <= max; i += (d = d == 4 ? 2 : 4)) {
        if (is_prime(i)) {
            prime[++num_prime] = i;
        }
        if (num_prime == N) {
            printf("error:配列サイズNが足りないYO\n");
            exit(1);
        }

    }
    sq_last_prime = prime[num_prime]*prime[num_prime];
}

void extend_prime_list(MyNumType x) {
    while (x > sq_last_prime) {
        generate_prime_list(prime[num_prime] + D);
    }
}

int is_prime(MyNumType x) {
    MyNumType r;
    int num;

    for (r = prime[num = 0]; r*r <= x; r = prime[++num]) {
        if (x % r == 0) {
            return 0;
        }
    }
    return 1;
}

MyNumType get_min_twin_prime(MyNumType start, MyNumType end) {
    if (start+2 > end) {
        return 0;
    }
    if (start <= 3 && 5 <= end) {
        return 3;
    }

    for (start += (5 - start % 6); start < end; start += 6) {
        extend_prime_list(start+2);
        if (is_prime(start) && is_prime(start+2) && (start+2 <= end)) {
            return start;
        }
    }
    return 0;
}

void show_twin_primes(MyNumType start, MyNumType end) {
    int count = 0;
    MyNumType result;
    if (start <= 3 && 5 <= end) {
        printf("%llu, %llu\n", 3ll, 5ll);
        start = 5;
        count++;
    }
    for (; start <= end; start += 6) {
        result = get_min_twin_prime(start, end);
        if (result) {
            printf("%llu, %llu\n", result, result+2);
            start = result;
            count++;
        }
    }
    printf("\nFound %d twin primes.\n", count);
}

void usage(const char* path_str) {
    printf("\nusage:"
            "\n    \e[34m%s\e[m"
            "\n        200000から400000の範囲で最小の双子素数の組を出力"
            "\n"
            "\n    \e[34m%s [start] [end] (-l)\e[m"
            "\n        start から end の範囲にある最小の双子素数の組を出力"
            "\n        (-l 全ての組を出力)"
            "\n\n", path_str, path_str);
}
