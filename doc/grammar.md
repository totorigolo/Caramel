# Grammar 
The grammar does not accept the `,` operator. We do not authorize a declaration
of an array without a defined size and no assignment :
```C
int32_t a[];
// Should be int32_t a[5];
// or        int32_t a = {1, 2, 3, 4, 5};
```
