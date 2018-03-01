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

instructions
  : InstructionSeparator* instruction (InstructionSeparator+ instructions)* InstructionSeparator*
  ;

instruction
  : expression
  ;

functionDeclaration
  : variableDeclaration namedArguments functionBody?
  ;

namedArguments
  : '()'
  | '(' namedArgument (',' namedArgument)? ')'
  ;

namedArgument
  : typeParameter
  | variableDeclaration
  ;

functionBody
  : '{}'
  | '{' declarations instructions '}'
  | '{' declarations '}'
  | '{' instructions '}'
  ;

variableDeclaration
  : typeParameter validIdentifier
  ;

typeParameter
  : validIdentifier
  ;

variableDefinition
  : variableDeclaration ('=' expression InstructionSeparator?) InstructionSeparator?
  | variableDeclaration
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
  : disjunction (assignmentOprator disjunction)*
  ;

disjunction
  : conjunction ('||' conjunction)*
  ;

conjunction
  : equalityComparison ('&&' equalityComparison)*
  ;

equalityComparison
  : additiveExpression (equalityOperation additiveExpression)*
  ;

additiveExpression
  : multiplicativeExpression (additiveOperation multiplicativeExpression)*
  ;

multiplicativeExpression
  : prefixUnaryExpression (multiplicativeOperation prefixUnaryExpression)*
  ;

prefixUnaryExpression
  : postfixUnaryExpression prefixUnaryExpressionPrime
  ;

prefixUnaryExpressionPrime
  : // Epsilon
  | postfixUnaryExpression prefixUnaryExpressionPrime
  ;

postfixUnaryExpression
  : atomicExpression postfixUnaryExpression*
  ;

atomicExpression
  : '(' expression ')'
  | literalContant
  | validIdentifier
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

literalContant
  : numberLiteral
  | characterLiteral
  ;

// Lexicon
multiplicativeOperation
  : '*'
  | '/'
  | '%'
  ;

additiveOperation
  : '+'
  | '-'
  ;

comparisonOperation
  : '<'
  | '>'
  | '>='
  | '<='
  ;

equalityOperation
  : '!='
  | '=='
  ;

assignmentOprator
  : '='
  | '+='
  | '-='
  | '*='
  | '/='
  | '%='
  ;

prefixUnaryOperation
  : '-'
  | '+'
  | '--'
  | '++'
  ;

postfixUnaryOperation
  : '--'
  | '++'
  | callSuffix
//  | arrayAccess
  ;

callSuffix
  : valuedArguments
  ;

valuedArguments
  : '(' expression (',' expression)* ')'
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
  : '//' .*? NewLine+ -> skip
  ;

NewLine
  : '\n'
  ;

WhiteSpace
  : [ \t\r] -> skip
  ;