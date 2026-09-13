#ifndef TRILANG_MATHUTIL_H
#define TRILANG_MATHUTIL_H

#ifdef __cplusplus
extern "C" {
#endif

/* 迭代法斐波那契：tri_fib(0)=0, tri_fib(1)=1, ... */
int tri_fib(int n);

/* 辗转相除法最大公约数 */
int tri_gcd(int a, int b);

#ifdef __cplusplus
}
#endif

#endif /* TRILANG_MATHUTIL_H */
