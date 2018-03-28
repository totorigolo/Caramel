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

#pragma once

#include "../util/Common.h"
#include "../Console.h"
#include "../datastructure/context/Context.h"
#include "../util/SourceFileUtil.h"
#include "../datastructure/operators/binaryoperators/PlusOperator.h"
#include "../datastructure/operators/binaryoperators/MultOperator.h"
#include "../datastructure/operators/binaryoperators/BitwiseShiftOperator.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wattributes"

#include <CaramelBaseVisitor.h>

#pragma GCC diagnostic pop

#include <stdexcept>
#include <iostream>

using namespace caramel::ast;

namespace caramel::visitors {

class ParseTree;
class ContextPusher;

class AbstractSyntaxTreeVisitor : public CaramelBaseVisitor {

public:
    explicit AbstractSyntaxTreeVisitor(std::string const &sourceFileName);

public:
    antlrcpp::Any visitR(CaramelParser::RContext *ctx) override;

    /// Returns vector<Statement::Ptr>
    antlrcpp::Any visitStatements(CaramelParser::StatementsContext *ctx) override;

    /// Returns vector<Statement::Ptr>
    antlrcpp::Any visitBlock(CaramelParser::BlockContext *ctx) override;

    /// Returns vector<Statement::Ptr>
    antlrcpp::Any visitDeclarations(CaramelParser::DeclarationsContext *ctx) override;

    /// Returns vector<Statement::Ptr>
    antlrcpp::Any visitInstructions(CaramelParser::InstructionsContext *ctx) override;

    /// Returns std::string
    antlrcpp::Any visitValidIdentifier(CaramelParser::ValidIdentifierContext *ctx) override;

    /// Returns SymbolType::Ptr (in a Symbol::Ptr)
    antlrcpp::Any visitTypeParameter(CaramelParser::TypeParameterContext *ctx) override;

    /// Returns vector<Statement::Ptr>
    antlrcpp::Any visitVariableDeclaration(CaramelParser::VariableDeclarationContext *ctx) override;

    /// Returns vector<Statement::Ptr>
    antlrcpp::Any visitVariableDefinition(CaramelParser::VariableDefinitionContext *ctx) override;

    /// Returns AtomicExpression::Ptr
    antlrcpp::Any visitPositiveConstant(CaramelParser::PositiveConstantContext *ctx) override;

    antlrcpp::Any visitFunctionDeclaration(CaramelParser::FunctionDeclarationContext *ctx) override;

    /// Returns FunctionDefinition::Ptr
    antlrcpp::Any visitFunctionDefinition(CaramelParser::FunctionDefinitionContext *ctx) override;

    antlrcpp::Any visitFunctionArguments(CaramelParser::FunctionArgumentsContext *ctx) override;

    antlrcpp::Any visitFunctionArgument(CaramelParser::FunctionArgumentContext *ctx) override;

    antlrcpp::Any visitControlBlock(CaramelParser::ControlBlockContext *ctx) override;

    /// Returns IfBlock::Ptr
    antlrcpp::Any visitIfBlock(CaramelParser::IfBlockContext *ctx) override;

    /// Returns WhileBlock::Ptr
    antlrcpp::Any visitWhileBlock(CaramelParser::WhileBlockContext *ctx) override;

    /// Returns ForBlock::Ptr
    antlrcpp::Any visitForBlock(CaramelParser::ForBlockContext *ctx) override;

    /// Returns Expression::Ptr
    antlrcpp::Any visitAtomicExpression(CaramelParser::AtomicExpressionContext *ctx) override;

    /// Returns Expression::Ptr
    antlrcpp::Any visitExpression(CaramelParser::ExpressionContext *ctx) override;

    /// Returns AtomicExpression::Ptr
    antlrcpp::Any visitNumberConstant(CaramelParser::NumberConstantContext *ctx) override;

    /// Returns AtomicExpression::Ptr
    antlrcpp::Any visitCharConstant(CaramelParser::CharConstantContext *ctx) override;

    /// Returns Statement::Ptr
    antlrcpp::Any visitArrayDefinition(CaramelParser::ArrayDefinitionContext *ctx) override;

    /// Returns Symbol::Ptr
    antlrcpp::Any visitArrayDeclarationVoidInner(CaramelParser::ArrayDeclarationVoidInnerContext *ctx) override;

    /// Returns Symbol::Ptr
    antlrcpp::Any visitArrayDeclarationInner(CaramelParser::ArrayDeclarationInnerContext *ctx) override;

    /// Returns vector<Expression::Ptr>
    antlrcpp::Any visitArrayBlock(CaramelParser::ArrayBlockContext *ctx) override;

    /// Returns AtomicExpression::Ptr
    antlrcpp::Any visitArraySizeDeclaration(CaramelParser::ArraySizeDeclarationContext *ctx) override;

    /// Returns Expression::Ptr
    antlrcpp::Any visitAdditiveExpression(CaramelParser::AdditiveExpressionContext *ctx) override;

    /// Returns BinaryOperator::Ptr
    antlrcpp::Any visitAdditiveOperator(CaramelParser::AdditiveOperatorContext *ctx) override;

    /// Returns Statement::Ptr
    antlrcpp::Any visitInstruction(CaramelParser::InstructionContext *ctx) override;

    antlrcpp::Any visitTypeDefinition(CaramelParser::TypeDefinitionContext *ctx) override;

    antlrcpp::Any visitReturnJump(CaramelParser::ReturnJumpContext *ctx) override;

    antlrcpp::Any visitPostfixUnaryExpression(CaramelParser::PostfixUnaryExpressionContext *ctx) override;

private:
    std::shared_ptr<caramel::ast::Context> currentContext(); //
    friend class ContextPusher;

    antlrcpp::Any visitChildren(antlr4::tree::ParseTree *node) override;

private:
    std::stack<std::shared_ptr<caramel::ast::Context>> mContextStack;
    SourceFileUtil mSourceFileUtil;

    const std::shared_ptr<caramel::ast::BitwiseShiftOperator> mBitwiseShiftOperator;
    const std::shared_ptr<caramel::ast::MultOperator> mMultOperator;
    const std::shared_ptr<caramel::ast::PlusOperator> mPlusOperator = std::make_shared<caramel::ast::PlusOperator>();
};

class ContextPusher {
public:
    explicit ContextPusher(AbstractSyntaxTreeVisitor &abstractSyntaxTreeVisitor);
    virtual ~ContextPusher();

    Context::Ptr getContext();

private:
    AbstractSyntaxTreeVisitor &mAbstractSyntaxTreeVisitor;
};

} // namespace caramel::visitors
