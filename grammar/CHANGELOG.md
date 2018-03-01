# Grammar versions

## Features V1

### Syntax

#### Valid Identifier
```antlrv4
validIdentifier
  : '_'? Letter anyCharacter*
  ;

anyCharacter
  : Letter
  | Digit
  | '_'
  ;
```

#### Digit and numberLiteral
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

#### CharacterLiteral
Any characters between simple quote `'`.
```antlrv4
characterLiteral
  : EscpaceChar
  | '\''.'\''
  ;

EscpaceChar
  : '\'\\\\\''
  | '\'\\a\''
  | '\'\\b\''
  | '\'\\f\''
  | '\'\\r\''
  | '\'\\t\''
  | '\'\\n\''
  | '\'\\v\''
  | '\'\\\'\''
  | '\'\\"\''
  ;
```

Example snippets:
```C
'a'
'\n'
```

#### Comments
Single line comment :
```antlrv4
LineComment
  : '//' .*? NewLine* -> skip
  ;
```

BlockComment
```antlrv4
BlockComment
  : '/*' .*? '*/' NewLine* -> skip
  ;
```

