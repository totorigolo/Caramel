grammar Caramel;
r
  : statements EOF
  ;

// Program statements
statements
  : MultilineWhiteSpace_* statement (MultilineWhiteSpace_* statement)* MultilineWhiteSpace_*
  ;
statement
  : instruction
  | declaration
  ;

instructions
  : MultilineWhiteSpace_* instruction (MultilineWhiteSpace_* instruction)* MultilineWhiteSpace_*
  ;
instruction
  : jump MultilineWhiteSpace_* InstructionSeparator
  | controlBlock
  | expression MultilineWhiteSpace_* InstructionSeparator
  | InstructionSeparator
  ;

declarations
  : MultilineWhiteSpace_* declaration (MultilineWhiteSpace_* declaration)* MultilineWhiteSpace_*
  ;

declaration
  : functionDefinition
  | functionDeclaration MultilineWhiteSpace_* InstructionSeparator
  | arrayDeclaration MultilineWhiteSpace_* InstructionSeparator
  | arrayDefinition MultilineWhiteSpace_* InstructionSeparator
  | variableDeclaration MultilineWhiteSpace_* InstructionSeparator
  | variableDefinition MultilineWhiteSpace_* InstructionSeparator
  | InstructionSeparator
  ;

// Declarations
functionDeclarationInner
  : typeParameter InlineWhiteSpace_+ validIdentifier functionArguments
  ;
functionDeclaration
  : functionDeclarationInner
  ;
functionDefinition
  : functionDeclarationInner MultilineWhiteSpace_* block ;

variableDeclaration
  : 'extern' InlineWhiteSpace_+ typeParameter InlineWhiteSpace_+ validIdentifier (InlineWhiteSpace_* Comma_ InlineWhiteSpace_* validIdentifier)*
  ;
variableDefinition
  : typeParameter InlineWhiteSpace_* (variableDefinitionAssignment|validIdentifier) MultilineWhiteSpace_* (MultilineWhiteSpace_* Comma_ InlineWhiteSpace_* (variableDefinitionAssignment|validIdentifier))*
  ;
variableDefinitionAssignment
  : validIdentifier InlineWhiteSpace_* Assignment_ InlineWhiteSpace_ expression ;

arrayDeclarationVoidInner
  : typeParameter InlineWhiteSpace_+ validIdentifier InlineWhiteSpace_* arraySizeDeclarationVoid
  ;
arrayDeclarationInner
  : typeParameter InlineWhiteSpace_+ validIdentifier InlineWhiteSpace_* arraySizeDeclaration
  ;

arrayDeclaration
  : 'extern' InlineWhiteSpace_* arrayDeclarationInner
  | 'extern' InlineWhiteSpace_* arrayDeclarationVoidInner
  ;

arrayDefinition
  : arrayDeclarationVoidInner MultilineWhiteSpace_* assignmentOperator MultilineWhiteSpace_* arrayBlock
  | arrayDeclarationInner (MultilineWhiteSpace_* assignmentOperator MultilineWhiteSpace_* arrayBlock)*
  ;
arraySizeDeclaration
  : L_Bracket_ InlineWhiteSpace_* positiveConstant InlineWhiteSpace_* R_Bracket_
  ;
arraySizeDeclarationVoid
  : L_Bracket_ InlineWhiteSpace_* R_Bracket_
  ;

// Jump instructions
jump
  : breakJump
  | returnJump
  ;
returnJump
  : ReturnKeyword_ InlineWhiteSpace_* expression
  | ReturnKeyword_
  ;
breakJump : BreakKeyword_ ;

// Control block
controlBlock
  : ifBlock
  | whileBlock
  | forBlock
  ;
ifBlock
  : IfKeyword_ MultilineWhiteSpace_* L_Par_ MultilineWhiteSpace_* expression InlineWhiteSpace_* R_Par_ MultilineWhiteSpace_* block? (MultilineWhiteSpace_* ElseKeyword_ MultilineWhiteSpace_* (ifBlock|block))?
  ;
whileBlock
  : WhileKeyWord_ MultilineWhiteSpace_* L_Par_ InlineWhiteSpace_* expression InlineWhiteSpace_* R_Par_ MultilineWhiteSpace_* block?
  ;
forBlock
  : ForKeyword_ MultilineWhiteSpace_* L_Par_
      InlineWhiteSpace_* expression? InlineWhiteSpace_* Semilicon_
      InlineWhiteSpace_* expression? InlineWhiteSpace_* Semilicon_
      InlineWhiteSpace_* expression? InlineWhiteSpace_* R_Par_
      MultilineWhiteSpace_ * block?
  ;

// Blocks
block
  : L_CBracket_ MultilineWhiteSpace_* declarations? MultilineWhiteSpace_* instructions? MultilineWhiteSpace_* R_CBracket_
  ;

arrayBlock
  : L_CBracket_ MultilineWhiteSpace_* R_CBracket_
  | L_CBracket_ MultilineWhiteSpace_* expression (MultilineWhiteSpace_* Comma_ MultilineWhiteSpace_* expression)* MultilineWhiteSpace_* R_CBracket_
  ;

// Function definition helpers
functionArguments
  : L_Par_ InlineWhiteSpace_* R_Par_
  | L_Par_ MultilineWhiteSpace_* functionArgument (MultilineWhiteSpace_* Comma_ MultilineWhiteSpace_*  functionArgument)* MultilineWhiteSpace_* R_Par_
  ;
functionArgument
  : typeParameter MultilineWhiteSpace_* validIdentifier? MultilineWhiteSpace_* (functionArgumentArraySuffix)?
  ;
functionArgumentArraySuffix
  : '[' MultilineWhiteSpace_* (expression)? MultilineWhiteSpace_* ']'
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
  : L_Par_ MultilineWhiteSpace_* expression MultilineWhiteSpace_* R_Par_ // '(' e ')'
  | validIdentifier
  | numberConstant
  | charConstant
  ;

assignment
  : lvalue InlineWhiteSpace_* assignmentOperator InlineWhiteSpace_* expression
  ;

expression : assignment | disjunction ;

disjunction
  : conjunction
  | disjunction MultilineWhiteSpace_* OrOp_ MultilineWhiteSpace_* conjunction
  ;
conjunction
  : orBitwiseExpression
  | conjunction MultilineWhiteSpace_* AndOp_ MultilineWhiteSpace_* conjunction
  ;

orBitwiseExpression
  : xorBitwiseExpression
  | orBitwiseExpression MultilineWhiteSpace_* BitwiseOr_ MultilineWhiteSpace_* orBitwiseExpression
  ;

xorBitwiseExpression
  : andBitwiseExpression
  | xorBitwiseExpression MultilineWhiteSpace_* BitwiseXor_ MultilineWhiteSpace_* xorBitwiseExpression
  ;

andBitwiseExpression
  : equalityComparison
  | andBitwiseExpression MultilineWhiteSpace_* BitwiseAnd_ MultilineWhiteSpace_* andBitwiseExpression
  ;

equalityComparison
  : comparison
  | equalityComparison MultilineWhiteSpace_* equalityOperator MultilineWhiteSpace_* equalityComparison
  ;

comparison
  : bitwiseShiftExpression
  | comparison MultilineWhiteSpace_* comparativeOperator MultilineWhiteSpace_* comparison
  ;

bitwiseShiftExpression
  : additiveExpression
  | bitwiseShiftExpression MultilineWhiteSpace_* bitwiseShiftOperator MultilineWhiteSpace_* bitwiseShiftExpression
  ;

additiveExpression
  : multiplicativeExpression
  | additiveExpression MultilineWhiteSpace_* additiveOperator MultilineWhiteSpace_* additiveExpression
  ;
multiplicativeExpression
  : prefixUnaryExpression
  | multiplicativeExpression MultilineWhiteSpace_* multiplicativeOperator MultilineWhiteSpace_* multiplicativeExpression
  ;

prefixUnaryExpression :     prefixUnaryOperator* postfixUnaryExpression;
postfixUnaryExpression :    atomicExpression postfixUnaryOperation* ;

// Operators
multiplicativeOperator : ( Times_ | Div_ | Mod_ ) ;
additiveOperator : ( Plus_ | Minus_ ) ;
comparativeOperator : ( LowerThan_ | LowerEqThan_ | GreaterThan_ | GreaterEqThan_ ) ;
equalityOperator : ( EqualityOp_ | DiffOp_ ) ;
postfixUnaryOperator : ( IncOp_ | DecOp_ ) ;
prefixUnaryOperator : ( Minus_ | IncOp_ | DecOp_ | InvOp_ ' '* | BitwiseNot_ ' '* | cast InlineWhiteSpace_*) ;
bitwiseShiftOperator : ( RightShiftOp_ | LeftShiftOp_ ) ;
assignmentOperator : ( Assignment_ | PlusAssign_ | MinusAssign_ | TimesAssign_ | DivAssign_ | ModAssign_ | BitwiseAndAssign_ | BitwiseOrAssign_ | BitwiseXorAssign_ | BitwiseNotAssign_ ) ;

postfixUnaryOperation
  : callSufix
  | arrayAccess
  | postfixUnaryOperator
  ;

cast
  : L_Par_ InlineWhiteSpace_* typeParameter InlineWhiteSpace_* R_Par_
  ;


callSufix
  : L_Par_ InlineWhiteSpace_* R_Par_
  | L_Par_ InlineWhiteSpace_* expression (InlineWhiteSpace_* Comma_ InlineWhiteSpace_* expression )* InlineWhiteSpace_* R_Par_
  ;

arrayAccess
  : L_Bracket_ InlineWhiteSpace_* expression InlineWhiteSpace_* R_Bracket_
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
InlineWhiteSpace_ : ( WS_ | Tab_ | CarryReturn_ );
MultilineWhiteSpace_ : ( InlineWhiteSpace_ | NewLine_ );
WS_ : ' ' ;
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
