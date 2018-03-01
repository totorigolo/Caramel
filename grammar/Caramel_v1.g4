grammar Caramel;
r
  : declaration+
  ;

declaration
  : functionDeclaration
  | variableDefinition
  ;

functionDeclaration
  : variableDeclaration namedArguments functionBody?
  ;

namedArguments
  : '()'
  | '(' variableDeclaration (',' variableDeclaration)? ')'
  ;

functionBody
  : '{}'
  ;

variableDeclaration
  : typeParameter validIdentifier
  ;

typeParameter
  : validIdentifier
  ;

variableDefinition
  : variableDeclaration ('=' expression)? InstructionSeparator?
  ;

validIdentifier
  : '_'? Letter anyCharacter*
  ;

anyCharacter
  : Letter
  | Digit
  | '_'
  ;

expression
  : numberLiteral
  | characterLiteral
  ;

numberLiteral
  : '-'?Digit+
  ;

Letter
  : [a-zA-Z]
  ;

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

Digit
  : [0-9]
  ;

InstructionSeparator
  : NewLine
  | ';'
  ;

BlockComment
  : '/*' .*? '*/' NewLine* -> skip
  ;

LineComment
  : '//' .*? NewLine* -> skip
  ;

NewLine
  : '\n'
  ;

WhiteSpace
  : [ \t\r] -> skip
  ;