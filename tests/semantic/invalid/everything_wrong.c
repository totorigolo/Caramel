/*
 * This is an invalid test displaying every kind of errors we can manage
 * Also shows how it display all the errors
 */

// variable undefined
int32_t x = y;

// error in block
void main(){
    // variable already declared
    extern int32_t a;
    extern int32_t a;

}

// variable already declared with different type
extern int32_t b;
extern int64_t b;

// variable already defined
int32_t c = 0;
int64_t c;

// variable declaration mismatch
int64_t d[2];
d = 2;

// function declared with different number of parameters
void foo(int8_t e, char f);
void foo(int8_t f)
{
    return;
}

// function name already declared
int32_t foo2;
void foo2(char g);

// function name already declared as array
int32_t bar2[2];
void bar2(char g);

// function parameter has a different name : warning
void foo3(int8_t h, char i);
void foo3(int8_t h, char j)
{
    return;
}

// function parameter has a different type
void bar3(int8_t k, char l);
void bar3(int8_t k, int8_t l)
{
    return;
}
