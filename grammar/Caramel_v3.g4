grammar Caramel;
r
  : statements wsOrNls EOF
  ;

statements
  : InstructionSeparator* wsOrNls statement (InstructionSeparator? wsOrNls statement wsOrNls)* wsOrNls InstructionSeparator* wsOrNls
  ;

definition
  : variableDefinition
  | functionDefinition
  ;

statement
  : controlBlock
  | definition
  | instruction
  ;

variableDefinition
  : declaration wsOrNls (AssignmentOp wss expression)? wsOrNls InstructionSeparator? ;

functionDefinition
  : declaration namedArguments wsOrNls fonctionBody? ;

namedArguments
  : L_Parenthesis wss R_Parenthesis
  | L_Parenthesis wss declaration wss (Comma wss declaration wss)* wss R_Parenthesis
  ;

fonctionBody
  : L_CurlyBracket R_CurlyBracket
  | L_CurlyBracket statements R_CurlyBracket
  ;

declaration
  : typeParameter ws+ identifier ;

typeParameter
  : identifier ;

instruction
  : jump
  | assignment
  | rvalue
  ;

controlBlock
  : ifBlock
  | whileBlock
  ;

ifBlock
  : ifOperator wss L_Parenthesis wss expression wss R_Parenthesis wsOrNls controleStructureBody? (wsOrNls elseOperator wsOrNls controleStructureBody)? wsOrNls;

ifOperator : 'if' ;
elseOperator : 'else';

whileBlock
  : whileOperator wss L_Parenthesis wss expression wss R_Parenthesis wsOrNls controleStructureBody? wsOrNls
  ;

whileOperator : 'while' ;

controleStructureBody
  : L_CurlyBracket R_CurlyBracket
  | L_CurlyBracket statements R_CurlyBracket
  ;

assignment
  : lvalue wss assignmentOperators wss rvalue InstructionSeparator?
  ;

lvalue
  : identifier ;

rvalue : expression ;

expression
  : disjunction ;

disjunction
  : conjunction (wss OrOp wss conjunction)* wss
  ;

conjunction
  : equalityComparison (wss AndOp wss equalityComparison)* wss
  ;

equalityComparison
  : comparison (wss equalityOperators wss comparison)* wss
  ;

equalityOperators
  : EqualsOp
  | NotEqualsOp
  ;

comparison
  : additiveExpression (wss comparisonOperators wss additiveExpression)* wss
  ;

comparisonOperators
  : LowerOp
  | GreaterOp
  | LowerEqOp
  | GreaterEqOp
  ;

additiveExpression
  : multiplicativeExpression (wss additiveOperators wss multiplicativeExpression)* wss
  ;

additiveOperators
  : PlusOp
  | MinusOp
  ;

multiplicativeExpression
  : prefixUnaryExpression (wss multiplicativeOperators wss prefixUnaryExpression)* wss ;

multiplicativeOperators
  : TimesOp
  | DivOp
  | ModOp
  ;

prefixUnaryExpression
  : prefixUnaryOperators* postfixUnaryExpression
  ;

prefixUnaryOperators
  : PlusOp
  | MinusOp
  | IncrementOp
  | DecrementOp
  ;

postfixUnaryExpression
  : atomicExpression postfixUnaryOperation* ;

postfixUnaryOperation
  : postfixOperators
  | callSuffix
  | arrayAccess
  ;

callSuffix
  : valueArguments ;

valueArguments
  : wss L_Parenthesis wss R_Parenthesis wss
  | wss L_Parenthesis expression (wss Comma wss expression)* R_Parenthesis wss
  ;

arrayAccess
  : L_Bracket wss expression wss R_Bracket wss
  ;

jump
  : breakOp
  | returnJump
  ;

breakOp : Break;

returnJump
  : returnOp wss expression? ;

returnOp : Return;

postfixOperators
  : IncrementOp
  | DecrementOp
  ;

atomicExpression
  : L_Parenthesis wsOrNl * expression wsOrNls R_Parenthesis wsOrNls
  | literalConstant
  | identifier
  ;

literalConstant
  : Number
  | Character
  ;

identifier
  : Identifier ;

assignmentOperators
  : AssignmentOp
  | PlusAssignOp
  | MinusAssignOp
  | TimesAssignOp
  | DivAssignOp
  | ModAssignOp
  ;

wss : ws* ;

ws
 : WhiteSpace
 ;


wsOrNls
 : wsOrNl*
 ;

wsOrNl
 : ws
 | NewLine
 | CarryReturn
 | Tab
 ;

Number
  : '-'? Digit + ;

Character : '\'' . '\'' ;

Identifier
  : ('_')? Letter LetterOrDigit* ;

fragment LetterOrDigit : [0-9a-zA-Z_] ;
fragment Digit : [0-9] ;
fragment Letter : [a-zA-Z] ;

// Operators

AndOp : '&&' ;
OrOp : '||' ;

PlusOp : '+' ;
MinusOp : '-' ;
TimesOp : '*' ;
DivOp : '/' ;
ModOp : '%' ;

AssignmentOp : '=' ;
PlusAssignOp : '+=' ;
MinusAssignOp : '-=' ;
TimesAssignOp : '*=' ;
DivAssignOp : '/=' ;
ModAssignOp : '%=' ;

LowerOp : '<' ;
GreaterOp : '>' ;
LowerEqOp : '<=' ;
GreaterEqOp : '>=';

EqualsOp : '==';
NotEqualsOp : '!=';

L_Parenthesis : '(' ;
R_Parenthesis : ')';

L_Bracket : '[' ;
R_Bracket : ']';

L_CurlyBracket : '{' ;
R_CurlyBracket : '}';

IncrementOp : '++';
DecrementOp : '--';

Comma : ',' ;

// Jumps
Break : 'break' ;
Return : 'return' ;


InstructionSeparator : ';' ;
NewLine : '\n';
WhiteSpace : ' ';
Tab : '\t';
CarryReturn : '\r';
