/*
 * Multi-line comment.
 */

int32_t foo(int16_t a, int32_t b, int64_t c);
int32_t foo(int16_t a, int32_t b, int64_t c);
int32_t foo(int16_t a, int32_t b, int64_t c);

int32_t main(int32_t, char argv) {
    int16_t var = 5;
    foo(var, 2, 3);
    foo(2, var, 4);
    return 0;
}

int32_t foo(int16_t a, int32_t b, int64_t c) {
    return b;
}
