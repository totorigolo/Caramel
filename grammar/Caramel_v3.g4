grammar Caramel;
r
  : statements wsOrNl* EOF
  ;

statements
  : InstructionSeparator* wsOrNl* statement (InstructionSeparator? wsOrNl* statement wsOrNl*)* wsOrNl* InstructionSeparator* wsOrNl*
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
  : declaration wsOrNl* (AssignmentOp ws* expression)? wsOrNl* InstructionSeparator? ;

functionDefinition
  : declaration namedArguments wsOrNl* fonctionBody? ;

namedArguments
  : L_Parenthesis ws* R_Parenthesis
  | L_Parenthesis ws* declaration ws* (Comma ws* declaration ws*)* ws* R_Parenthesis
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
  : assignment
  | rvalue
  | jump
  ;

controlBlock
  : ifBlock
  | whileBlock
  ;

ifBlock
  : ifOperator ws* L_Parenthesis ws* expression ws* R_Parenthesis wsOrNl* controleStructureBody? (wsOrNl* elseOperator wsOrNl* controleStructureBody)? wsOrNl*;

ifOperator : 'if' ;
elseOperator : 'else';

whileBlock
  : whileOperator ws* L_Parenthesis ws* expression ws* R_Parenthesis wsOrNl* controleStructureBody? wsOrNl*
  ;

whileOperator : 'while' ;

controleStructureBody
  : L_CurlyBracket R_CurlyBracket
  | L_CurlyBracket statements R_CurlyBracket
  ;

assignment
  : lvalue ws* assignmentOperators ws* rvalue InstructionSeparator?
  ;

lvalue
  : identifier ;

rvalue : expression ;

expression
  : disjunction ;

disjunction
  : conjunction (ws* OrOp ws* conjunction)* ws*
  ;

conjunction
  : equalityComparison (ws* AndOp ws* equalityComparison)* ws*
  ;

equalityComparison
  : comparison (ws* equalityOperators ws* comparison)* ws*
  ;

equalityOperators
  : EqualsOp
  | NotEqualsOp
  ;

comparison
  : additiveExpression (ws* comparisonOperators ws* additiveExpression)* ws*
  ;

comparisonOperators
  : LowerOp
  | GreaterOp
  | LowerEqOp
  | GreaterEqOp
  ;

additiveExpression
  : multiplicativeExpression (ws* additiveOperators ws* multiplicativeExpression)* ws*
  ;

additiveOperators
  : PlusOp
  | MinusOp
  ;

multiplicativeExpression
  : prefixUnaryExpression (ws* multiplicativeOperators ws* prefixUnaryExpression)* ws* ;

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
  : ws* L_Parenthesis ws* R_Parenthesis ws*
  | ws* L_Parenthesis expression (ws* Comma ws* expression)* R_Parenthesis ws*
  ;

arrayAccess
  : L_Bracket ws* expression ws* R_Bracket ws*
  ;

jump
  : breakOp
  | returnJump
  ;

breakOp : Break;

returnJump
  : returnOp ws* expression? ;

returnOp : Return;

postfixOperators
  : IncrementOp
  | DecrementOp
  ;

atomicExpression
  : L_Parenthesis wsOrNl * expression wsOrNl* R_Parenthesis wsOrNl*
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

ws
 : WhiteSpace
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