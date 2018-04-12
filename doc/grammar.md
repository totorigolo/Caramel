# Grammar 

* We do not authorize a declaration of an array without a defined size and
no assignment:
```C
int32_t a[];
// Should be int32_t a[5];
// or        int32_t a[] = {1, 2, 3, 4, 5};
```

* We are strict with whitespaces (spaces, line breaks,...). For example,
we do not authorize this line, since it's badly formated:
```C
int32_t i =
0;
```
