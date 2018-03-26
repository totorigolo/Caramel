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

#include "../Console.h"
#include "../datastructure/context/Context.h"
#include "../util/SourceFileUtil.h"
#include "../datastructure/operators/binaryoperators/PlusOperator.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wattributes"

#include <CaramelBaseVisitor.h>

#pragma GCC diagnostic pop

#include <stdexcept>
#include <iostream>

using namespace caramel::dataStructure;

namespace caramel::visitors {

class ParseTree;

class AbstractSyntaxTreeVisitor : public CaramelBaseVisitor {

public:
    explicit AbstractSyntaxTreeVisitor(std::string const &sourceFileName);

public:
    antlrcpp::Any visitChildren(antlr4::tree::ParseTree *node) override;

public:
    antlrcpp::Any visitR(CaramelParser::RContext *ctx) override;

    // Return vector<Statement::Ptr>
    antlrcpp::Any visitStatements(CaramelParser::StatementsContext *ctx) override;

    // Return vector<Statement::Ptr>
    antlrcpp::Any visitBlock(CaramelParser::BlockContext *ctx) override;

    // Return vector<Statement::Ptr>
    antlrcpp::Any visitDeclarations(CaramelParser::DeclarationsContext *ctx) override;

    // Return vector<Statement::Ptr>
    antlrcpp::Any visitInstructions(CaramelParser::InstructionsContext *ctx) override;

    // Return std::string
    antlrcpp::Any visitValidIdentifier(CaramelParser::ValidIdentifierContext *ctx) override;

    // Return SymbolType::Ptr (in a Symbol::Ptr)
    antlrcpp::Any visitTypeParameter(CaramelParser::TypeParameterContext *ctx) override;

    // Return vector<Statement::Ptr>
    antlrcpp::Any visitVariableDeclaration(CaramelParser::VariableDeclarationContext *ctx) override;

    // Return vector<Statement::Ptr>
    antlrcpp::Any visitVariableDefinition(CaramelParser::VariableDefinitionContext *ctx) override;

    antlrcpp::Any visitPositiveConstant(CaramelParser::PositiveConstantContext *ctx) override;

    antlrcpp::Any visitFunctionDeclaration(CaramelParser::FunctionDeclarationContext *ctx) override;

    // Return FunctionDefinition::Ptr
    antlrcpp::Any visitFunctionDefinition(CaramelParser::FunctionDefinitionContext *ctx) override;

    antlrcpp::Any visitFunctionArguments(CaramelParser::FunctionArgumentsContext *ctx) override;

    antlrcpp::Any visitFunctionArgument(CaramelParser::FunctionArgumentContext *ctx) override;

    antlrcpp::Any visitIfBlock(CaramelParser::IfBlockContext *ctx) override;

    // Return Expression::Ptr
    antlrcpp::Any visitAtomicExpression(CaramelParser::AtomicExpressionContext *ctx) override;

    // Return Expression::Ptr
    antlrcpp::Any visitExpression(CaramelParser::ExpressionContext *ctx) override;

    // Return Constant::Ptr
    antlrcpp::Any visitNumberConstant(CaramelParser::NumberConstantContext *ctx) override;

    // Return Constant::Ptr
    antlrcpp::Any visitCharConstant(CaramelParser::CharConstantContext *ctx) override;

    antlrcpp::Any visitArrayDefinition(CaramelParser::ArrayDefinitionContext *ctx) override;

    antlrcpp::Any visitArrayDeclarationVoidInner(CaramelParser::ArrayDeclarationVoidInnerContext *ctx) override;

    antlrcpp::Any visitArrayDeclarationInner(CaramelParser::ArrayDeclarationInnerContext *ctx) override;

    antlrcpp::Any visitArrayBlock(CaramelParser::ArrayBlockContext *ctx) override;

    antlrcpp::Any visitArraySizeDeclaration(CaramelParser::ArraySizeDeclarationContext *ctx) override;

    antlrcpp::Any visitAdditiveExpression(CaramelParser::AdditiveExpressionContext *ctx) override;

    antlrcpp::Any visitAdditiveOperator(CaramelParser::AdditiveOperatorContext *ctx) override;


private:
    void pushNewContext();

    void popContext();

    std::shared_ptr<caramel::dataStructure::context::Context> currentContext();

private:
    std::stack<std::shared_ptr<caramel::dataStructure::context::Context>> mContextStack;
    SourceFileUtil mSourceFileUtil;

    const std::shared_ptr<operators::binaryOperators::PlusOperator> mPlusOperator = std::make_shared<operators::binaryOperators::PlusOperator>();
};

} // namespace Caramel::Visitors
