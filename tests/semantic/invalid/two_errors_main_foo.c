/*
 * One error in main(), an other in foo()
 */




int32_t main() {
    int32_t i = 1;
    int32_t a = b;
    return 0;
}

int32_t foo(int32_t a, int32_t b, int32_t c);

int32_t foo(int32_t a, int32_t b)
{
    a+b;
    return 0;
}
