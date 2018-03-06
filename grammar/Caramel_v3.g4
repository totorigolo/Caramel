grammar Caramel;
r
  : statements
  ;

statements
  : declarations
  | instructions
  ;

declarations
  : InstructionSeparator* declaration (InstructionSeparator+ declaration)* InstructionSeparator*
  ;

declaration
  : functionDeclaration
  | variableDefinition
  ;

functionDeclaration
  : variableDeclaration namedArguments functionBody?
  ;

instructions
  : InstructionSeparator* instruction (InstructionSeparator+ instructions)* InstructionSeparator*
  ;

instruction
  : expression
  ;

namedArguments
  : '()'
  | '(' variableDeclaration (',' variableDeclaration)? ')'
  ;

functionBody
  : '{}'
  ;

variableDeclaration
  : typeParameter ValidIdentifier
  ;

typeParameter
  : ValidIdentifier
  ;

variableDefinition
  : variableDeclaration ('=' expression)? InstructionSeparator?
  ;

ValidIdentifier
  : '_'? Letter AnyCharacter*
  ;

AnyCharacter
  : Letter
  | Digit
  | '_'
  ;

expression
  : disjunction (AssignmentOperator disjunction)*
  ;

disjunction
  : conjunction ('||' conjunction)*
  ;

conjunction
  : equalityComparison ('&&' equalityComparison)*
  ;

equalityComparison
  : comparison (EqualityOperation comparison)*
  ;

comparison
  : atomicExpression (ComparisonOperation atomicExpression)*
  ;
/*
additiveExpression
  : multiplicativeExpression (AdditiveOperation multiplicativeExpression)*
  ;

multiplicativeExpression
  : prefixUnaryExpression (MultiplicativeOperation prefixUnaryExpression)*
  ;

prefixUnaryExpression
  : PrefixUnaryOperation* postfixUnaryExpression
  ;

postfixUnaryExpression
  : atomicExpression postfixUnaryOperation*
  ;
*/
atomicExpression
  : '(' expression ')'
  | LiteralConstant
  | ValidIdentifier
  | ifCondition
  | loop
  ;

ifCondition
  : 'if' '(' expression ')' controlStructureBody? InstructionSeparator? ('else' controlStructureBody)?
  ;

controlStructureBody
  : instructions
  ;

loop
  : whileLoop
  ;

whileLoop
  : 'while' '(' expression ')' controlStructureBody
  ;


LiteralConstant
  : NumberLiteral
  | CharacterLiteral
  ;

MultiplicativeOperation
  : '*'
  | '/'
  | '%'
  ;

AdditiveOperation
  : '+'
  | '-'
  ;

ComparisonOperation
  : '<'
  | '>'
  | '>='
  | '<='
  ;

AssignmentOperator
  : '+='
  | '-='
  | '*='
  | '/='
  | '%='
  ;

EqualityOperation
  : '!='
  | '=='
  ;

postfixUnaryOperation
  : '--'
  | '++'
  | callSuffix
//  | arrayAccess
  ;

PrefixUnaryOperation
  : '--'
  | '++'
  | '-'
  | '+'
  ;

callSuffix
  : valuedArguments
  ;

valuedArguments
  : '(' expression (',' expression)* ')'
  ;

NumberLiteral
  : '-'?Digit+
  ;

Letter
  : [a-zA-Z]
  ;

CharacterLiteral
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
  : ';'
  | '\n'
  ;

BlockComment
  : '/*' .*? '*/' -> skip
  ;

LineComment
  : '//' .*? '\n' -> skip
  ;


WhiteSpace
  : [ \t\r\n] -> skip
  ;