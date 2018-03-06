grammar Caramel;
r
  : statements EOF
  ;

BlockComment
  : '/*' .*? '*/' -> skip
  ;

LineComment
  : '//' .*? '\n' -> skip
  ;


statements
  : declarations? instructions?
  ;

instructions
  : InstructionSeparator* instruction (InstructionSeparator+ instructions)* InstructionSeparator*
  ;

instruction
  : expression
  | ifCondition
  | loop
  ;

declarations
  : InstructionSeparator* declaration (InstructionSeparator+ declaration)* InstructionSeparator*
  ;

declaration
  : variableDefinition
  | functionDeclaration
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
  : TypeParameter ValidIdentifier
  ;

TypeParameter
  : 'int'
  | 'int64_t'
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

BinaryAssignmentOperator
  : '+='
  | '-='
  | '*='
  | '/='
  | '%='
  ;

expression
  : disjunction
  | lvalue (BinaryAssignmentOperator disjunction)*
  | lvalue '=' disjunction
  ;

lvalue
  : ValidIdentifier
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
  : additiveExpression (ComparisonOperation additiveExpression)*
  ;

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
  | functionCall
  ;

atomicExpression
  : '(' expression ')'
  | literalConstant
  | ValidIdentifier
  ;

functionCall
  : ValidIdentifier '()'
  | ValidIdentifier callSuffix
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


literalConstant
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

EqualityOperation
  : '!='
  | '=='
  ;

postfixUnaryOperation
  : '--'
  | '++'
  | arrayAccess
  ;

PrefixUnaryOperation
  : '--'
  | '++'
  ;

callSuffix
  : valuedArguments
  ;

arrayAccess
  : '[' expression ']'
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
//  | '\n'
  ;

WhiteSpace
  : [ \t\r\n] -> skip
  ;