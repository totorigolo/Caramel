# Grammar versions

## Features V1

### Syntax
Digit and numberLiteral
```antlrv4
numberLiteral
  : '-'? Digit+
  ;

Digit
  : [0-9]
  ;
```

Example snippets:
```C
0
10
-1868
```

CharacterLiteral
Any characters between simple quote `'`.
```antlrv4
CharacterLiteral
  : '\'\\\\\''
  | '\'\\n\''
  | '\''.'\''
  ;
```

Example snippets:
```C
'a'
'\n'
```