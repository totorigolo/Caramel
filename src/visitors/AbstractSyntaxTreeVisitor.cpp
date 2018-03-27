/*
 * MIT License
 *
 * Copyright (c) 2018 insa.4if.hexanome_kalate
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include "AbstractSyntaxTreeVisitor.h"
#include "../Logger.h"
#include "../datastructure/statements/expressions/atomicexpression/Constant.h"
#include "../datastructure/symboltable/VariableSymbol.h"
#include "../datastructure/symboltable/FunctionSymbol.h"
#include "../datastructure/statements/declaration/VariableDeclaration.h"
#include "../datastructure/statements/declaration/FunctionDeclaration.h"
#include "../datastructure/statements/definition/VariableDefinition.h"
#include "../datastructure/statements/definition/FunctionDefinition.h"
#include "../datastructure/symboltable/ArraySymbol.h"
#include "../datastructure/statements/declaration/ArrayDeclaration.h"
#include "../datastructure/statements/expressions/binaryexpression/BinaryExpression.h"
#include "../exceptions/ArraySizeNonConstantException.h"
#include "../datastructure/statements/jumps/Jump.h"
#include "../datastructure/statements/controlblocks/ControlBlock.h"


using namespace caramel::visitors;

AbstractSyntaxTreeVisitor::AbstractSyntaxTreeVisitor(std::string const &sourceFileName)
        : mSourceFileUtil{sourceFileName} {
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitR(CaramelParser::RContext *ctx) {
    pushNewContext();
    return visitStatements(ctx->statements());
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitStatements(CaramelParser::StatementsContext *ctx) {
    logger.trace() << "visit statements: " << ctx->getText();
    using namespace caramel::colors;
    using caramel::ast::Statement;

    std::vector<Statement::Ptr> statements;
    for (auto statement : ctx->statement()) {
        logger.debug() << "Visiting: " << mSourceFileUtil.getLine(statement->start->getLine());

        antlrcpp::Any r = visitStatement(statement);
        if (r.is<Statement::Ptr>()) {
            statements.push_back(r.as<Statement::Ptr>());
            logger.info() << green << "Yay statement:\n" << statement->getText();
        } else if (r.is<std::vector<Statement::Ptr>>()) {
            auto statementVector = r.as<std::vector<Statement::Ptr>>();
            std::copy(statementVector.begin(), statementVector.end(), std::back_inserter(statements));
            logger.info() << green << "Yay statement:\n" << statement->getText();
        } else {
            logger.warning() << "Skipping unhandled statement:\n" << statement->getText();
        }
    }
    currentContext()->addStatements(std::move(statements));
    return currentContext();
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitBlock(CaramelParser::BlockContext *ctx) {
    logger.trace() << "visit block: " << ctx->getText();

    using caramel::ast::Statement;

    if (ctx->declarations()) {
        std::vector<Statement::Ptr> declarations = visitDeclarations(ctx->declarations());
        currentContext()->addStatements(std::move(declarations));
    }
    if (ctx->instructions()) {
        std::vector<Statement::Ptr> instructions = visitInstructions(ctx->instructions());
        currentContext()->addStatements(std::move(instructions));
    }

    return currentContext();
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitDeclarations(CaramelParser::DeclarationsContext *ctx) {
    logger.trace() << "visit declarations: " << ctx->getText();

    using caramel::ast::Statement;

    std::vector<Statement::Ptr> declarations;
    for (auto declaration : ctx->declaration()) {
        antlrcpp::Any r = visitDeclaration(declaration);
        if (r.is<Statement::Ptr>()) {
            declarations.push_back(r.as<Statement::Ptr>());
        } else if (r.is<std::vector<Statement::Ptr>>()) {
            auto declarationVector = r.as<std::vector<Statement::Ptr>>();
            std::copy(declarationVector.begin(), declarationVector.end(), std::back_inserter(declarations));
        } else {
            logger.warning() << "Skipping unhandled declaration:\n" << declaration->getText();
        }
    }
    return declarations;
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitInstructions(CaramelParser::InstructionsContext *ctx) {
    logger.trace() << "visit instructions: " << ctx->getText();

    using caramel::ast::Statement;

    std::vector<Statement::Ptr> instructions;
    // TODO: Instructions
    logger.warning() << "Skipping unimplemented instructions at line " << __LINE__ << ".";
//    for (auto instruction : ctx->instruction()) {
//        instructions.push_back(visitInstruction(instruction).as<Statement::Ptr>());
//    }
    return instructions;
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitValidIdentifier(CaramelParser::ValidIdentifierContext *ctx) {
    return ctx->getText();
}

antlrcpp::Any
AbstractSyntaxTreeVisitor::visitTypeParameter(CaramelParser::TypeParameterContext *ctx) {
    logger.trace() << "type parameter: " << ctx->getText();

    using namespace caramel::ast;

    Symbol::Ptr symbol = currentContext()->getSymbolTable()->getSymbol(ctx->getText());
    TypeSymbol::Ptr typeSymbol = std::dynamic_pointer_cast<TypeSymbol>(symbol);
    return typeSymbol;
}

antlrcpp::Any
AbstractSyntaxTreeVisitor::visitVariableDeclaration(CaramelParser::VariableDeclarationContext *ctx) {

    using namespace caramel::ast;

    logger.trace() << "Visiting variable declaration: " << ctx->getText();
    TypeSymbol::Ptr typeSymbol = visitTypeParameter(ctx->typeParameter()).as<TypeSymbol::Ptr>();
    std::vector<Statement::Ptr> variables;
    for (auto validIdentifierCtx : ctx->validIdentifier()) {
        std::string name = visitValidIdentifier(validIdentifierCtx);
        VariableSymbol::Ptr variableSymbol = std::make_shared<VariableSymbol>(name, typeSymbol);

        VariableDeclaration::Ptr variableDeclaration = std::make_shared<VariableDeclaration>(variableSymbol,
                                                                                             validIdentifierCtx->getStart());
        variables.push_back(variableDeclaration);

        currentContext()->getSymbolTable()->addVariableDeclaration(ctx, typeSymbol->getType(), name,
                                                                   variableDeclaration);

        logger.trace() << "New variable declared " << name << " of type " << typeSymbol->getType()->getIdentifier();
    }

    return variables;
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitVariableDefinition(CaramelParser::VariableDefinitionContext *ctx) {

    using namespace caramel::ast;

    logger.trace() << "Visiting variable definition: " << ctx->getText();

    TypeSymbol::Ptr typeSymbol = visitTypeParameter(ctx->typeParameter()).as<TypeSymbol::Ptr>();
    std::vector<Statement::Ptr> variables;
    for (auto child : ctx->children) {
        if (nullptr != dynamic_cast<CaramelParser::ValidIdentifierContext *>(child)) {
            auto *identifierContext = dynamic_cast<CaramelParser::ValidIdentifierContext *>(child);
            std::string name = visitValidIdentifier(identifierContext);
            logger.trace() << "New variable declared: '" << name << "' with default value";
            VariableSymbol::Ptr variableSymbol = std::make_shared<VariableSymbol>(name, typeSymbol);
            VariableDefinition::Ptr variableDef = std::make_shared<VariableDefinition>(variableSymbol,
                                                                                       identifierContext->getStart());
            variables.push_back(variableDef);

            currentContext()->getSymbolTable()->addVariableDefinition(ctx, typeSymbol->getType(), name, variableDef);

        } else if (nullptr != dynamic_cast<CaramelParser::VariableDefinitionAssignmentContext *>(child)) {
            auto *vdAssignmentContext = dynamic_cast<CaramelParser::VariableDefinitionAssignmentContext *>(child);
            std::string name = visitValidIdentifier(vdAssignmentContext->validIdentifier());
            // Fixme : replace nullptr by (visitExpression(vdAssignmentContext->expression()).as<Statement::Ptr>());
            Expression::Ptr expression = Constant::defaultConstant(ctx->getStart());
            logger.trace() << "New variable declared: '" << name << "' with value";
            VariableSymbol::Ptr variableSymbol = std::make_shared<VariableSymbol>(name, typeSymbol);
            VariableDefinition::Ptr variableDef = std::make_shared<VariableDefinition>(variableSymbol, expression,
                                                                                       vdAssignmentContext->getStart());
            variables.push_back(variableDef);

            currentContext()->getSymbolTable()->addVariableDefinition(ctx, typeSymbol->getType(), name, variableDef);
        }
    }

    return variables;
}

antlrcpp::Any
AbstractSyntaxTreeVisitor::visitFunctionDeclaration(CaramelParser::FunctionDeclarationContext *ctx) {

    using namespace caramel::ast;

    logger.trace() << "Visiting function declaration: " << ctx->getText() << ' ' << ctx->getStart();

    auto innerCtx = ctx->functionDeclarationInner();

    PrimaryType::Ptr returnType = visitTypeParameter(innerCtx->typeParameter()).as<TypeSymbol::Ptr>()->getType();
    std::string name = visitValidIdentifier(innerCtx->validIdentifier());
    std::vector<Symbol::Ptr> params = visitFunctionArguments(innerCtx->functionArguments());
    FunctionDeclaration::Ptr functionDeclaration = std::make_shared<FunctionDeclaration>(
            std::make_shared<FunctionSymbol>(name, returnType), ctx->start);
    currentContext()->getSymbolTable()->addFunctionDeclaration(ctx, returnType, name, params,
                                                               functionDeclaration);

    auto traceLogger = logger.trace();
    traceLogger << "New function declared " << name << " with return type " << returnType->getIdentifier();
    for (Symbol::Ptr const &param : params) {
        traceLogger << "\n\t- param: " << param->getName() << " as " << param->getType()->getIdentifier();
    }
    traceLogger.show();

    return std::dynamic_pointer_cast<Statement>(functionDeclaration);
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitFunctionDefinition(CaramelParser::FunctionDefinitionContext *ctx) {
    logger.trace() << "Visiting function definition: " << ctx->getText() << ' ' << ctx->getStart();

    using namespace caramel::ast;

    Context::Ptr parentContext = currentContext();
    pushNewContext();
    Context::Ptr functionContext = currentContext();

    CaramelParser::FunctionDeclarationInnerContext *innerCtx = ctx->functionDeclarationInner();

    PrimaryType::Ptr returnType = visitTypeParameter(innerCtx->typeParameter()).as<TypeSymbol::Ptr>()->getType();
    std::string name = visitValidIdentifier(innerCtx->validIdentifier());
    std::vector<Symbol::Ptr> params = visitFunctionArguments(innerCtx->functionArguments());

    FunctionDefinition::Ptr functionDefinition = std::make_shared<FunctionDefinition>(functionContext, ctx->start);
    FunctionSymbol::Ptr functionSymbol = functionContext->getSymbolTable()->addFunctionDefinition(
            ctx, returnType, name, params, functionDefinition
    );

    parentContext->getSymbolTable()->addFunctionDefinition(
            ctx, returnType, name, params, functionDefinition
    );

    functionDefinition->setSymbol(functionSymbol);

    // Visit the function's block, which adds inner statements into context
    visitBlock(ctx->block());

    popContext();
    return std::dynamic_pointer_cast<Statement>(functionDefinition);
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitFunctionArguments(CaramelParser::FunctionArgumentsContext *ctx) {

    using namespace caramel::ast;

    logger.debug() << "Visiting named arguments: " << ctx->getText();

    std::vector<Symbol::Ptr> params;
    for (auto argument : ctx->functionArgument()) {
        antlrcpp::Any symbol = visitFunctionArgument(argument);
        if(symbol.is<ArraySymbol::Ptr>()) {
            params.push_back(symbol.as<ArraySymbol::Ptr>());
        } else if (symbol.is<VariableSymbol::Ptr>()) {
            params.push_back(symbol.as<VariableSymbol::Ptr>());
        }

    }
    return params;
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitFunctionArgument(CaramelParser::FunctionArgumentContext *ctx) {

    using namespace caramel::ast;

    logger.trace() << "Visiting function argument: " << ctx->getText();

    // Get the optional name, or generate a unique one
    std::string name;
    if (nullptr != ctx->validIdentifier()) {
        name = visitValidIdentifier(ctx->validIdentifier()).as<std::string>();
    } else {
        std::stringstream nameSS;
        nameSS << "__unnamed_argument_" << ctx->start->getLine() << "_" << ctx->start->getCharPositionInLine() << "__";
        name = nameSS.str();
    }

    // Get the type
    TypeSymbol::Ptr type;
    type = visitTypeParameter(ctx->typeParameter());

    // The argument is an array
    if (nullptr != ctx->functionArgumentArraySuffix()) {
        return std::make_shared<ArraySymbol>(name, type); // FIXME: Return a new ArrayVariableSymbol
    } else {
        return std::make_shared<VariableSymbol>(name, type);
    }
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitIfBlock(CaramelParser::IfBlockContext *ctx) {
    return CaramelBaseVisitor::visitIfBlock(ctx);
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitAtomicExpression(CaramelParser::AtomicExpressionContext *ctx) {

    using namespace caramel::ast;

    if (nullptr != ctx->validIdentifier()) {
        std::string varName = visitValidIdentifier(ctx->validIdentifier());
        // Fixme : replace with true atomicExpression constructor
        AtomicExpression::Ptr atomicExpression = Constant::defaultConstant(ctx->getStart());
        currentContext()->getSymbolTable()->addVariableUsage(ctx, varName, atomicExpression);
    }
    return std::dynamic_pointer_cast<Expression>(visitChildren(ctx).as<AtomicExpression::Ptr>());
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitNumberConstant(CaramelParser::NumberConstantContext *ctx) {

    using namespace caramel::ast;

    long long value = std::stoll(ctx->getText());
    return std::dynamic_pointer_cast<AtomicExpression>(std::make_shared<Constant>(value, ctx->start));
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitCharConstant(CaramelParser::CharConstantContext *ctx) {

    using namespace caramel::ast;

    char value = ctx->getText().at(0);
    return std::dynamic_pointer_cast<AtomicExpression>(std::make_shared<Constant>(value, ctx->start));
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitAdditiveExpression(CaramelParser::AdditiveExpressionContext *ctx) {
    using namespace caramel::ast;

    if (ctx->children.size() == 1) {
        // One children = No BinaryExpression at this step.
        return visitChildren(ctx).as<Expression::Ptr>();
    } else {
        return std::dynamic_pointer_cast<Expression>(std::make_shared<BinaryExpression>(
                visitAdditiveExpression(ctx->additiveExpression(0)),
                visitAdditiveOperator(ctx->additiveOperator()),
                visitAdditiveExpression(ctx->additiveExpression(1)),
                ctx->getStart()
        ));
    }

}

antlrcpp::Any
AbstractSyntaxTreeVisitor::visitAdditiveOperator(caramel_unused CaramelParser::AdditiveOperatorContext *ctx) {
    using caramel::ast::BinaryOperator;
    return std::dynamic_pointer_cast<BinaryOperator>(mPlusOperator);
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitInstruction(CaramelParser::InstructionContext *ctx) {
    using namespace caramel::ast;
    if (ctx->jump()) { // pour kévin : != nullptr
        return std::dynamic_pointer_cast<Statement>(visitJump(ctx->jump()).as<Jump::Ptr>());
    } else if (ctx->controlBlock()) {
        return std::dynamic_pointer_cast<Statement>(visitControlBlock(ctx->controlBlock()).as<ControlBlock::Ptr>());
    }

    return std::dynamic_pointer_cast<Statement>(visitExpression(ctx->expression()).as<Expression::Ptr>());
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitArrayDefinition(CaramelParser::ArrayDefinitionContext *ctx) {

    using namespace caramel::ast;

    ArraySymbol::Ptr arraySymbol;
    ArrayDeclaration::Ptr arrayDeclaration;

    if (nullptr != ctx->arrayDeclarationVoidInner()) {
        arraySymbol = visitArrayDeclarationVoidInner(ctx->arrayDeclarationVoidInner()).as<ArraySymbol::Ptr>();

        std::vector<Expression::Ptr> expressions = visitArrayBlock(ctx->arrayBlock());
        long arraySize = expressions.size();
        arraySymbol->setSize(arraySize);

        arrayDeclaration = std::make_shared<ArrayDeclaration>(arraySymbol,
                                                    ctx->arrayDeclarationVoidInner()->validIdentifier()->getStart());

    } else if (nullptr != ctx->arrayDeclarationInner()) {
        arraySymbol = visitArrayDeclarationInner(ctx->arrayDeclarationInner()).as<ArraySymbol::Ptr>();

        arrayDeclaration = std::make_shared<ArrayDeclaration>(arraySymbol,
                                                    ctx->arrayDeclarationInner()->validIdentifier()->getStart());

    }

    currentContext()->getSymbolTable()->addVariableDeclaration(ctx, arraySymbol->getType(), arraySymbol->getName(),
                                                               arrayDeclaration);


    logger.trace() << "New array declared : '" << arraySymbol->getName() << "' with return type " << arraySymbol->getType()->getIdentifier()
                   << ", size " << arraySymbol->getSize();

    //TODO : Cas d'erreur
    return std::dynamic_pointer_cast<Statement>(arrayDeclaration);
}

antlrcpp::Any
AbstractSyntaxTreeVisitor::visitArrayDeclarationVoidInner(CaramelParser::ArrayDeclarationVoidInnerContext *ctx) {

    using namespace caramel::ast;

    logger.trace() << "Visiting array declaration void: " << ctx->getText();

    auto typeSymbol = visitTypeParameter(ctx->typeParameter()).as<TypeSymbol::Ptr>();
    std::string name = visitValidIdentifier(ctx->validIdentifier());
    auto arraySymbol{std::make_shared<ArraySymbol>(name, typeSymbol, 0)};

    return arraySymbol;
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitArrayBlock(CaramelParser::ArrayBlockContext *ctx) {
    using namespace caramel::ast;
    std::vector<Expression::Ptr> expressions;
    for (auto expression : ctx->expression()) {
        Expression::Ptr exp = visitExpression(expression);
        expressions.push_back(exp);
    }
    return expressions;
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitArrayDeclarationInner(CaramelParser::ArrayDeclarationInnerContext *ctx) {
    logger.trace() << "Visiting array declaration : " << ctx->getText();

    using namespace caramel::ast;
    using namespace caramel::exceptions;

    auto typeSymbol = visitTypeParameter(ctx->typeParameter()).as<TypeSymbol::Ptr>();
    std::string name = visitValidIdentifier(ctx->validIdentifier());
    antlrcpp::Any arraySizeAny = visitArraySizeDeclaration(ctx->arraySizeDeclaration());
    Constant::Ptr arraySize = std::dynamic_pointer_cast<Constant>(arraySizeAny.as<AtomicExpression::Ptr>());
//    if (!arraySizeAny.is<Constant::Ptr>()) {
//        throw ArraySizeNonConstantException("Non constant expression not handled for array sizes.");
//    }
    ArraySymbol::Ptr arraySymbol = std::make_shared<ArraySymbol>(name, typeSymbol, arraySize->getValue().as<long long>());

    return arraySymbol;
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitArraySizeDeclaration(CaramelParser::ArraySizeDeclarationContext *ctx) {
    return visitPositiveConstant(ctx->positiveConstant());
}


antlrcpp::Any AbstractSyntaxTreeVisitor::visitPositiveConstant(CaramelParser::PositiveConstantContext *ctx) {
    using namespace caramel::ast;

    long long value = std::stoll(ctx->getText());
    return std::dynamic_pointer_cast<AtomicExpression>(std::make_shared<Constant>(value, ctx->getStart()));
}

void AbstractSyntaxTreeVisitor::pushNewContext() {

    using namespace caramel::ast;

    logger.debug() << "Pushed a new context.";

    SymbolTable::Ptr parentTable;
    if (!mContextStack.empty()) {
        Context::Ptr parent = mContextStack.top();
        mContextStack.push(std::make_shared<Context>(parent));
    } else {
        mContextStack.push(std::make_shared<Context>());
    }

    SymbolTable::Ptr symbolTable = currentContext()->getSymbolTable();

    PrimaryType::Ptr void_t = Void_t::Create();
    PrimaryType::Ptr char_t = Char::Create();
    PrimaryType::Ptr int8_t = Int8_t::Create();
    PrimaryType::Ptr int16_t = Int16_t::Create();
    PrimaryType::Ptr int32_t = Int32_t::Create();
    PrimaryType::Ptr int64_t = Int64_t::Create();

    symbolTable->addPrimaryType(void_t, void_t->getIdentifier());
    symbolTable->addPrimaryType(char_t, char_t->getIdentifier());
    symbolTable->addPrimaryType(int8_t, int8_t->getIdentifier());
    symbolTable->addPrimaryType(int16_t, int16_t->getIdentifier());
    symbolTable->addPrimaryType(int32_t, int32_t->getIdentifier());
    symbolTable->addPrimaryType(int64_t, int64_t->getIdentifier());
}

std::shared_ptr<Context> AbstractSyntaxTreeVisitor::currentContext() {
    return mContextStack.top();
}

void AbstractSyntaxTreeVisitor::popContext() {
    logger.debug() << "Pop context.";
    mContextStack.pop();
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitChildren(antlr4::tree::ParseTree *node) {
    size_t n = node->children.size();
    size_t i = 0;
    while (i < n && nullptr == node->children[i]) { i++; }
    antlrcpp::Any childResult;
    if (i < n) {
        childResult = node->children[i]->accept(this);
    } else {
        childResult = defaultResult();
    }
    return childResult;
}
