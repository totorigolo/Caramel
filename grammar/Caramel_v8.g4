grammar Caramel;
r
  : statements EOF
  ;

// Program statements
statements
  : WS_* statement (WS_* statement WS_* )* WS_*
  ;
statement
  : instruction
  | declaration
  | InstructionSeparator
  ;

instructions
  : WS_* instruction (WS_* instruction)* WS_*
  ;
instruction
  : jump (WS_* InstructionSeparator)+
  | controlBlock
  | expression (WS_* InstructionSeparator)+
  ;

declarations
  : WS_* declaration (WS_* declaration)* WS_*
  ;

declaration
  : typeDefinition (WS_* InstructionSeparator)+
  | functionDefinition
  | functionDeclaration (WS_* InstructionSeparator)+
  | arrayDeclaration (WS_* InstructionSeparator)+
  | arrayDefinition (WS_* InstructionSeparator)+
  | variableDeclaration (WS_* InstructionSeparator)+
  | variableDefinition (WS_* InstructionSeparator)+
  ;

// Declarations
typeDefinition
  : 'typedef' InlineWS_+ typeParameter InlineWS_+ typeParameter
  ;

functionDeclarationInner
  : typeParameter InlineWS_+ validIdentifier functionArguments
  ;
functionDeclaration
  : functionDeclarationInner
  ;
functionDefinition
  : functionDeclarationInner WS_* block ;

variableDeclaration
  : 'extern' InlineWS_+ typeParameter InlineWS_+ validIdentifier (WS_* Comma_ WS_* validIdentifier)*
  ;
variableDefinition
  : typeParameter InlineWS_* (variableDefinitionAssignment|validIdentifier) (WS_* Comma_ WS_* (variableDefinitionAssignment|validIdentifier))*
  ;
variableDefinitionAssignment
  : validIdentifier WS_* Assignment_ InlineWS_* expression ;

arrayUnsizedInner
  : typeParameter InlineWS_+ validIdentifier InlineWS_* L_Bracket_ R_Bracket_
  ;
arraySizedInner
  : typeParameter InlineWS_+ validIdentifier InlineWS_* L_Bracket_ InlineWS_* positiveConstant InlineWS_* R_Bracket_
  ;
arraySize
  : L_Bracket_ InlineWS_* positiveConstant InlineWS_* R_Bracket_
  ;
arrayDeclaration
  : 'extern' InlineWS_* (arrayUnsizedInner | arraySizedInner)
  ;
arrayDefinition
  : arrayUnsizedInner WS_* assignmentOperator WS_* arrayBlock
  | arraySizedInner (WS_* assignmentOperator WS_* arrayBlock)?
  ;

// Jump instructions
jump
  : breakJump
  | returnJump
  ;
returnJump
  : ReturnKeyword_ (InlineWS_* expression)?
  ;
breakJump : BreakKeyword_ ;

// Control block
controlBlock
  : ifBlock
  | whileBlock
  | forBlock
  ;

ifBlock
  : IfKeyword_ WS_* L_Par_ WS_* expression WS_* R_Par_ WS_* block (WS_* ElseKeyword_ WS_* (ifBlock|block))?
  ;
whileBlock
  : WhileKeyWord_ WS_* L_Par_ WS_* expression WS_* R_Par_ WS_* block?
  ;
forBlock
  : ForKeyword_ WS_* L_Par_
      WS_* expression WS_* Semilicon_
      WS_* expression WS_* Semilicon_
      WS_* expression WS_* R_Par_
      WS_ * block
  ;

// Blocks
block
  : L_CBracket_ WS_* declarations? WS_* instructions? WS_* R_CBracket_
  ;

arrayBlock
  : L_CBracket_ WS_* R_CBracket_
  | L_CBracket_ WS_* expression (WS_* Comma_ WS_* expression)* Comma_? WS_* R_CBracket_
  ;

// Function definition helpers
functionArguments
  : L_Par_ InlineWS_* R_Par_
  | L_Par_ WS_* functionArgument (WS_* Comma_ WS_*  functionArgument)* Comma_? WS_* R_Par_
  ;
functionArgument
  : typeParameter WS_* validIdentifier? WS_* (functionArgumentArraySuffix)?
  ;
functionArgumentArraySuffix
  : '[' WS_* (expression)? WS_* ']'
  ;

// Type & identifiers
typeParameter
  : Identifier ;
validIdentifier
  : Identifier ;

// Left & Right values
lvalue
  : validIdentifier arrayAccess?
  ;
atomicExpression // As right value
  : L_Par_ WS_* expression WS_* R_Par_ // '(' e ')'
  | validIdentifier
  | numberConstant
  | charConstant
  ;

assignment
  : lvalue InlineWS_* assignmentOperator InlineWS_* expression
  ;

expression : assignment | disjunction ;

disjunction
  : conjunction
  | disjunction WS_* OrOp_ WS_* disjunction
  ;
conjunction
  : orBitwiseExpression
  | conjunction WS_* AndOp_ WS_* conjunction
  ;

orBitwiseExpression
  : xorBitwiseExpression
  | orBitwiseExpression WS_* BitwiseOr_ WS_* orBitwiseExpression
  ;

xorBitwiseExpression
  : andBitwiseExpression
  | xorBitwiseExpression WS_* BitwiseXor_ WS_* xorBitwiseExpression
  ;

andBitwiseExpression
  : equalityComparison
  | andBitwiseExpression WS_* BitwiseAnd_ WS_* andBitwiseExpression
  ;

equalityComparison
  : comparison
  | equalityComparison WS_* equalityOperator WS_* equalityComparison
  ;

comparison
  : bitwiseShiftExpression
  | comparison WS_* comparativeOperator WS_* comparison
  ;

bitwiseShiftExpression
  : additiveExpression
  | bitwiseShiftExpression WS_* bitwiseShiftOperator WS_* bitwiseShiftExpression
  ;

additiveExpression
  : multiplicativeExpression
  | additiveExpression WS_* additiveOperator WS_* additiveExpression
  ;
multiplicativeExpression
  : prefixUnaryExpression
  | multiplicativeExpression WS_* multiplicativeOperator WS_* multiplicativeExpression
  ;

prefixUnaryExpression :     prefixUnaryOperator* postfixUnaryExpression;
postfixUnaryExpression :    atomicExpression postfixUnaryOperation* ;

// Operators
multiplicativeOperator : ( Times_ | Div_ | Mod_ ) ;
additiveOperator : ( Plus_ | Minus_ ) ;
comparativeOperator : ( LowerThan_ | LowerEqThan_ | GreaterThan_ | GreaterEqThan_ ) ;
equalityOperator : ( EqualityOp_ | DiffOp_ ) ;
postfixUnaryOperator : ( IncOp_ | DecOp_ ) ;
prefixUnaryOperator : ( Minus_ | IncOp_ | DecOp_ | InvOp_ ' '* | BitwiseNot_ ' '* | cast InlineWS_*) ;
bitwiseShiftOperator : ( RightShiftOp_ | LeftShiftOp_ ) ;
assignmentOperator : ( Assignment_ | PlusAssign_ | MinusAssign_ | TimesAssign_ | DivAssign_ | ModAssign_ | BitwiseAndAssign_ | BitwiseOrAssign_ | BitwiseXorAssign_ | BitwiseNotAssign_ ) ;

postfixUnaryOperation
  : callSufix
  | arrayAccess
  | postfixUnaryOperator
  ;

cast
  : L_Par_ InlineWS_* typeParameter InlineWS_* R_Par_
  ;


callSufix
  : L_Par_ InlineWS_* R_Par_
  | L_Par_ InlineWS_* expression (InlineWS_* Comma_ InlineWS_* expression)* Comma_? InlineWS_* R_Par_
  ;

arrayAccess
  : L_Bracket_ InlineWS_* expression InlineWS_* R_Bracket_
  ;

// Constants
numberConstant
  : PositiveNumber
  | Minus_ PositiveNumber
  ;
positiveConstant : PositiveNumber;
charConstant
  : '\''.'\''
  | EscapedNL_
  | EscapedCR_
  | EscapedTB_
  | EscapedSimpeQuote_
  | EscapedDoubleQuote_
  | EscapedAntibackslash_
  ;

InstructionSeparator : Semilicon_ ;
PositiveNumber : FragmentNumber_;
Identifier : FragmentIdentifier_ ;

// Ignored blocks
BlockComment
    : '/*' .*? '*/' -> skip ;
LineComment
    : '//' ~[\r\n]* -> skip ;
Macro
    : '#' ~[\r\n]* -> skip ;
WS
    : WS_ -> skip ;


// Do not delete the following comment line
// InjectedTokens
NewLine_ : '\\n' ;
FragmentNumber_ : Digit_+;
Digit_ : ('0'|'1'|'2'|'3'|'4'|'5'|'6'|'7'|'8'|'9') ;
LowerCaseLetter_ : ('a'|'b'|'c'|'d'|'e'|'f'|'g'|'h'|'i'|'j'|'k'|'l'|'m'|'n'|'o'|'p'|'q'|'r'|'s'|'t'|'u'|'v'|'w'|'x'|'y'|'z') ;
UpperCaseLetter_ : ('A'|'B'|'C'|'D'|'E'|'F'|'G'|'H'|'I'|'J'|'K'|'L'|'M'|'N'|'O'|'P'|'Q'|'R'|'S'|'T'|'U'|'V'|'W'|'X'|'Y'|'Z') ;
Underscore_ : '_' ;
DigitSeparator_ : '.' ;
CarryReturn_ : '\\r' ;
Tab_ : '\\t' ;
Space_ : ' ' ;
InlineWS_ : ( Space_ | Tab_ | CarryReturn_ );
WS_ : ( InlineWS_ | NewLine_ );
PlusAssign_ : '+=' ;
MinusAssign_ : '-=' ;
TimesAssign_ : '*=' ;
DivAssign_ : '/=' ;
ModAssign_ : '%=' ;
BitwiseOrAssign_ : '|=' ;
BitwiseXorAssign_ : '^=' ;
BitwiseAndAssign_ : '&=' ;
BitwiseNotAssign_ : '~=' ;
Plus_ : '+' ;
Minus_ : '-' ;
Times_ : '*' ;
Div_ : '/' ;
Mod_ : '%' ;
IncOp_ : '++' ;
DecOp_ : '--' ;
L_Par_ : '(' ;
R_Par_ : ')' ;
L_CBracket_ : '{' ;
R_CBracket_ : '}' ;
L_Bracket_ : '[' ;
R_Bracket_ : ']' ;
LowerThan_ : '<' ;
GreaterThan_ : '>' ;
LowerEqThan_ : '<=' ;
GreaterEqThan_ : '>=' ;
EqualityOp_ : '==' ;
RefEqualityOp_ : '===' ;
DiffOp_ : '!=' ;
Letter_ : ( LowerCaseLetter_ | UpperCaseLetter_ ) ;
FragmentIdentifier_ : ( Underscore_? Letter_ AnyCharacter_* ) ;
AnyCharacter_ : ( Underscore_ | Letter_ | Digit_ ) ;
Assignment_ : '=' ;
Comma_ : ',' ;
EscapedNL_ : '\'\\\\n\'';
EscapedCR_ : '\'\\\\r\'';
EscapedTB_ : '\'\\\\t\'';
EscapedSimpeQuote_ : '\'\\\\\'\'';
EscapedDoubleQuote_ : '\'\\\\"\'';
EscapedAntibackslash_ : '\'\\\\\\\\\'';
Semilicon_ : ';' ;
InvOp_ : '!' ;
AndOp_ : '&&' ;
OrOp_ : '||' ;
IfKeyword_ : 'if' ;
ForKeyword_ : 'for' ;
WhileKeyWord_ : 'while' ;
ElseKeyword_ : 'else' ;
ReturnKeyword_ : 'return' ;
BreakKeyword_ : 'break' ;
RightShiftOp_ : '>>' ;
LeftShiftOp_ : '<<' ;
BitwiseAnd_ : '&' ;
BitwiseOr_ : '|';
BitwiseXor_ : '^' ;
BitwiseNot_ : '~' ;
