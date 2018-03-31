/*
 * MIT License
 *
 * Copyright (c) 2018 Kalate Hexanome, 4IF, INSA Lyon
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
#include "../utils/Common.h"
#include "../ast/statements/expressions/atomicexpression/Constant.h"
#include "../ast/statements/expressions/atomicexpression/Identifier.h"
#include "../ast/statements/expressions/atomicexpression/ArrayAccess.h"
#include "../ast/statements/expressions/atomicexpression/FunctionCall.h"
#include "../ast/statements/expressions/binaryexpression/BinaryExpression.h"
#include "../ast/statements/expressions/atomicexpression/AtomicExpression.h"


using namespace caramel::ast;
using namespace caramel::utils;
using namespace caramel::colors;
using namespace caramel::visitors;

antlrcpp::Any AbstractSyntaxTreeVisitor::visitExpression(CaramelParser::ExpressionContext *ctx) {
    logger.trace() << "visiting expression: " << grey <<ctx->getText();
    return visitChildren(ctx);
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitAdditiveExpression(CaramelParser::AdditiveExpressionContext *ctx) {
    logger.fatal() << "visiting additive expression: " << grey <<ctx->getText();

    if (ctx->children.size() == 1) {
        // One children = No BinaryExpression at this step.
        return visitChildren(ctx).as<Expression::Ptr>();
    } else {
        return castTo<Expression::Ptr>(std::make_shared<BinaryExpression>(
                visitAdditiveExpression(ctx->additiveExpression(0)),
                visitAdditiveOperator(ctx->additiveOperator()),
                visitAdditiveExpression(ctx->additiveExpression(1)),
                ctx->getStart()
        ));
    }
}

antlrcpp::Any
AbstractSyntaxTreeVisitor::visitAdditiveOperator(caramel_unused CaramelParser::AdditiveOperatorContext *ctx) {
    logger.trace() << "visiting additive operator: " << grey <<ctx->getText();
    return std::dynamic_pointer_cast<BinaryOperator>(mPlusOperator);
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitAtomicExpression(CaramelParser::AtomicExpressionContext *ctx) {
    logger.trace() << "visiting atomic expression: " << grey <<ctx->getText();

    if (ctx->validIdentifier()) {
        std::string varName = visitValidIdentifier(ctx->validIdentifier());
        if (ctx->callSufix()) {
            std::vector<Expression::Ptr> arguments;
            for (auto expression : ctx->callSufix()->expression()) {
                Expression::Ptr exp = visitExpression(expression);
                arguments.push_back(exp);
            }

            FunctionCall::Ptr functionCall = std::make_shared<FunctionCall>(std::move(arguments), ctx->getStart());
            FunctionSymbol::Ptr variableSymbol = currentContext()->getSymbolTable()->addFunctionCall(
                    ctx, varName, functionCall);
            functionCall->setSymbol(variableSymbol);
            return castTo<Expression::Ptr>(functionCall);
        } else if (ctx->arrayAccess()) {
            Expression::Ptr index = visitExpression(ctx->arrayAccess()->expression());
            ArrayAccess::Ptr arrayAccess = std::make_shared<ArrayAccess>(index, ctx->getStart());
            ArraySymbol::Ptr variableSymbol = currentContext()->getSymbolTable()->addArrayAccess(
                    ctx, varName, arrayAccess);
            arrayAccess->setSymbol(variableSymbol);
            return castTo<Expression::Ptr>(arrayAccess);
        } else { // if (ctx->identifier()) {
            Identifier::Ptr identifier = std::make_shared<Identifier>(ctx->getStart());
            VariableSymbol::Ptr variableSymbol = currentContext()->getSymbolTable()->addVariableUsage(
                    ctx, varName, identifier);
            identifier->setSymbol(variableSymbol);
            return castTo<Expression::Ptr>(identifier);
        }
    } else if (ctx->charConstant()) {
        return castAnyTo<AtomicExpression::Ptr, Expression::Ptr>(visitCharConstant(ctx->charConstant()));
    } else if (ctx->numberConstant()) {
        return castAnyTo<AtomicExpression::Ptr, Expression::Ptr>(visitNumberConstant(ctx->numberConstant()));
    } else if (ctx->expression()) {
        return visitExpression(ctx->expression());
    } else { // if (ctx->stringConstant()) {
        logger.fatal() << "String constants aren't handled yet!";
        exit(1);
    }
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitNumberConstant(CaramelParser::NumberConstantContext *ctx) {
    logger.trace() << "visiting number constant: " << grey <<ctx->getText();

    long long value = std::stoll(ctx->getText());
    return castTo<AtomicExpression::Ptr>(std::make_shared<Constant>(value, ctx->start));
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitCharConstant(CaramelParser::CharConstantContext *ctx) {
    logger.trace() << "visiting char constant: " << grey <<ctx->getText();

    char value = ctx->getText().at(0);
    return castTo<AtomicExpression::Ptr>(std::make_shared<Constant>(value, ctx->start));
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitPositiveConstant(CaramelParser::PositiveConstantContext *ctx) {
    logger.trace() << "visiting positive constant: " << grey <<ctx->getText();

    long long value = std::stoll(ctx->getText());
    return castTo<AtomicExpression::Ptr>(std::make_shared<Constant>(value, ctx->getStart()));
}

antlrcpp::Any
AbstractSyntaxTreeVisitor::visitPostfixUnaryExpression(CaramelParser::PostfixUnaryExpressionContext *ctx) {
    logger.fatal() << "visiting postfix unary expression: " << grey <<ctx->getText();

    antlrcpp::Any atomicExpression = visitAtomicExpression(ctx->atomicExpression());
    if (ctx->postfixUnaryOperation()) {
        if (!atomicExpression.is<VariableSymbol::Ptr>()) {
            throw std::runtime_error("Cannot use postfix Unary Operator on a non variable symbol.");
        }
    }

    if (atomicExpression.is<Symbol::Ptr>()) {

    } else {
        return atomicExpression;
    }
}
