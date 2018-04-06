/*
 * Changing the name of a parameter gives an error
 * More rigorous than c, won't compile with this error
 */

void foo(int8_t a, char c);

void foo(int8_t a, char d)
{
    return;
}
