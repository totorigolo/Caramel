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

#include "ASTVisitor.h"
#include "../Logger.h"
#include "../utils/Common.h"
#include "../ast/statements/expressions/CommaExpression.h"
#include "../ast/statements/expressions/atomicexpression/Constant.h"
#include "../ast/statements/expressions/atomicexpression/Identifier.h"
#include "../ast/statements/expressions/atomicexpression/ArrayAccess.h"
#include "../ast/statements/expressions/atomicexpression/FunctionCall.h"
#include "../ast/statements/expressions/binaryexpression/BinaryExpression.h"
#include "../ast/operators/BinaryOperator.h"
#include "../ast/operators/binaryoperators/AssignmentOperator.h"
#include "../ast/operators/binaryoperators/BitwiseAndOperator.h"
#include "../ast/operators/binaryoperators/BitwiseOrOperator.h"
#include "../ast/operators/binaryoperators/BitwiseXorOperator.h"
#include "../ast/operators/binaryoperators/DisjunctionOperator.h"
#include "../ast/operators/binaryoperators/ConjunctionOperator.h"
#include "../ast/statements/expressions/unaryexpression/UnaryExpression.h"
#include "../ast/statements/expressions/atomicexpression/Identifier.h"


#define FIND_BINARY_OP(ctx) mBinaryOperatorIndex.getOpForToken((ctx)->getText())
#define FIND_PREFIX_OP(ctx) mPrefixOperatorIndex.getOpForToken((ctx)->getText())
#define FIND_POSTFIX_OP(ctx) mPostfixOperatorIndex.getOpForToken((ctx)->getText())

using namespace caramel::ast;
using namespace caramel::utils;
using namespace caramel::colors;
using namespace caramel::visitors;

//--------------------------------------------------------------------------------------------------------
// Binary Expressions

antlrcpp::Any ASTVisitor::visitExpression(CaramelParser::ExpressionContext *ctx) {
    logger.trace() << "visiting expression: " << grey << ctx->getText();

    if (ctx->children.size() == 1) {
        // One children = No comma expression.
        return visitChildren(ctx).as<Expression::Ptr>();
    } else {
        std::vector<Expression::Ptr> expressions;
        for (auto expression : ctx->expressionNoComma()) {
            Expression::Ptr expr = visitExpressionNoComma(expression);
            expressions.push_back(expr);
        }
        return castTo<Expression::Ptr>(std::make_shared<CommaExpression>(
                ctx->getStart(),
                expressions
        ));
    }
}

antlrcpp::Any ASTVisitor::visitAdditiveExpression(CaramelParser::AdditiveExpressionContext *ctx) {
    logger.trace() << "visiting additive expression: " << grey << ctx->getText();
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
ASTVisitor::visitBitwiseShiftExpression(CaramelParser::BitwiseShiftExpressionContext *ctx) {
    logger.trace() << "visiting bitwise shift expression: " << grey << ctx->getText();
    if (ctx->children.size() == 1) {
        // One children = No BinaryExpression at this step.
        return visitChildren(ctx).as<Expression::Ptr>();
    } else {
        return castTo<Expression::Ptr>(std::make_shared<BinaryExpression>(
                visitBitwiseShiftExpression(ctx->bitwiseShiftExpression(0)),
                visitBitwiseShiftOperator(ctx->bitwiseShiftOperator()),
                visitBitwiseShiftExpression(ctx->bitwiseShiftExpression(1)),
                ctx->getStart()
        ));
    }
}

antlrcpp::Any
ASTVisitor::visitComparison(CaramelParser::ComparisonContext *ctx) {
    logger.trace() << "visiting comparison: " << grey << ctx->getText();
    if (ctx->children.size() == 1) {
        // One children = No BinaryExpression at this step.
        return visitChildren(ctx).as<Expression::Ptr>();
    } else {
        return castTo<Expression::Ptr>(std::make_shared<BinaryExpression>(
                visitComparison(ctx->comparison(0)),
                visitComparativeOperator(ctx->comparativeOperator()),
                visitComparison(ctx->comparison(1)),
                ctx->getStart()
        ));
    }
}

antlrcpp::Any ASTVisitor::visitEqualityComparison(CaramelParser::EqualityComparisonContext *ctx) {
    logger.trace() << "visiting equality comparison: " << grey << ctx->getText();
    if (ctx->children.size() == 1) {
        // One children = No BinaryExpression at this step.
        return visitChildren(ctx).as<Expression::Ptr>();
    } else {
        return castTo<Expression::Ptr>(std::make_shared<BinaryExpression>(
                visitEqualityComparison(ctx->equalityComparison(0)),
                visitEqualityOperator(ctx->equalityOperator()),
                visitEqualityComparison(ctx->equalityComparison(1)),
                ctx->getStart()
        ));
    }
}

antlrcpp::Any
ASTVisitor::visitMultiplicativeExpression(CaramelParser::MultiplicativeExpressionContext *ctx) {
    logger.trace() << "visiting multiplicative expression: " << grey << ctx->getText();
    if (ctx->children.size() == 1) {
        // One children = No BinaryExpression at this step.
        return visitChildren(ctx).as<Expression::Ptr>();
    } else {
        return castTo<Expression::Ptr>(std::make_shared<BinaryExpression>(
                visitMultiplicativeExpression(ctx->multiplicativeExpression(0)),
                visitMultiplicativeOperator(ctx->multiplicativeOperator()),
                visitMultiplicativeExpression(ctx->multiplicativeExpression(1)),
                ctx->getStart()
        ));
    }
}

antlrcpp::Any ASTVisitor::visitAtomicExpression(CaramelParser::AtomicExpressionContext *ctx) {
    logger.trace() << "visiting atomic expression: " << grey << ctx->getText();

    if (ctx->validIdentifier()) {
        std::string varName = visitValidIdentifier(ctx->validIdentifier());
        std::vector<Expression::Ptr> arguments;
        for (auto expression : ctx->callSufix()->expressionNoComma()) {
            Expression::Ptr exp = visitExpressionNoComma(expression);
            arguments.push_back(exp);
        }

        FunctionCall::Ptr functionCall = std::make_shared<FunctionCall>(std::move(arguments), ctx->getStart());
        FunctionSymbol::Ptr variableSymbol = currentContext()->getSymbolTable()->addFunctionCall(
                ctx, varName, functionCall);
        functionCall->setSymbol(variableSymbol);
        return castTo<Expression::Ptr>(functionCall);
    } else if (ctx->lvalue()) {
        return visitLvalue(ctx->lvalue());
    } else if (ctx->charConstant()) {
        return castAnyTo<AtomicExpression::Ptr, Expression::Ptr>(visitCharConstant(ctx->charConstant()));
    } else if (ctx->numberConstant()) {
        return castAnyTo<AtomicExpression::Ptr, Expression::Ptr>(visitNumberConstant(ctx->numberConstant()));
    } else if (ctx->expression()) {
        return visitExpression(ctx->expression());
    } else { // if (ctx->stringConstant()) {
        logger.fatal() << "ConstString constants aren't handled yet!";
        exit(1);
    }
}

antlrcpp::Any ASTVisitor::visitAndBitwiseExpression(CaramelParser::AndBitwiseExpressionContext *ctx) {
    logger.trace() << "visiting bitwise AND expression: " << grey << ctx->getText();
    if (ctx->children.size() == 1) {
        // One children = No BinaryExpression at this step.
        return visitChildren(ctx).as<Expression::Ptr>();
    } else {
        return castTo<Expression::Ptr>(std::make_shared<BinaryExpression>(
                visitAndBitwiseExpression(ctx->andBitwiseExpression(0)),
                mBinaryOperatorIndex.getOpForToken(BitwiseAndOperator::SYMBOL),
                visitAndBitwiseExpression(ctx->andBitwiseExpression(1)),
                ctx->getStart()
        ));
    }
}

antlrcpp::Any ASTVisitor::visitOrBitwiseExpression(CaramelParser::OrBitwiseExpressionContext *ctx) {
    logger.trace() << "visiting bitwise OR expression: " << grey << ctx->getText();
    if (ctx->children.size() == 1) {
        // One children = No BinaryExpression at this step.
        return visitChildren(ctx).as<Expression::Ptr>();
    } else {
        return castTo<Expression::Ptr>(std::make_shared<BinaryExpression>(
                visitOrBitwiseExpression(ctx->orBitwiseExpression(0)),
                mBinaryOperatorIndex.getOpForToken(BitwiseOrOperator::SYMBOL),
                visitOrBitwiseExpression(ctx->orBitwiseExpression(1)),
                ctx->getStart()
        ));
    }
}

antlrcpp::Any ASTVisitor::visitXorBitwiseExpression(CaramelParser::XorBitwiseExpressionContext *ctx) {
    logger.trace() << "visiting bitwise XOR expression: " << grey << ctx->getText();
    if (ctx->children.size() == 1) {
        // One children = No BinaryExpression at this step.
        return visitChildren(ctx).as<Expression::Ptr>();
    } else {
        return castTo<Expression::Ptr>(std::make_shared<BinaryExpression>(
                visitXorBitwiseExpression(ctx->xorBitwiseExpression(0)),
                mBinaryOperatorIndex.getOpForToken(BitwiseXorOperator::SYMBOL),
                visitXorBitwiseExpression(ctx->xorBitwiseExpression(1)),
                ctx->getStart()
        ));
    }
}

antlrcpp::Any ASTVisitor::visitConjunction(CaramelParser::ConjunctionContext *ctx) {
    logger.trace() << "visiting conjuction: " << grey << ctx->getText();
    if (ctx->children.size() == 1) {
        // One children = No BinaryExpression at this step.
        return visitChildren(ctx).as<Expression::Ptr>();
    } else {
        return castTo<Expression::Ptr>(std::make_shared<BinaryExpression>(
                visitConjunction(ctx->conjunction(0)),
                mBinaryOperatorIndex.getOpForToken(ConjunctionOperator::SYMBOL),
                visitConjunction(ctx->conjunction(1)),
                ctx->getStart()
        ));
    }
}

antlrcpp::Any ASTVisitor::visitDisjunction(CaramelParser::DisjunctionContext *ctx) {
    logger.trace() << "visiting disjunction: " << grey << ctx->getText();
    if (ctx->children.size() == 1) {
        // One children = No BinaryExpression at this step.
        return visitChildren(ctx).as<Expression::Ptr>();
    } else {
        return castTo<Expression::Ptr>(std::make_shared<BinaryExpression>(
                visitDisjunction(ctx->disjunction(0)),
                mBinaryOperatorIndex.getOpForToken(DisjunctionOperator::SYMBOL),
                visitDisjunction(ctx->disjunction(1)),
                ctx->getStart()
        ));
    }
}

antlrcpp::Any ASTVisitor::visitAssignment(CaramelParser::AssignmentContext *ctx) {
    logger.trace() << "visiting assignment expression: " << grey << ctx->getText();

    if (ctx->children.size() == 1) {
        // One children = No BinaryExpression at this step.
        return visitChildren(ctx).as<Expression::Ptr>();
    } else {
        return castTo<Expression::Ptr>(std::make_shared<BinaryExpression>(
                visitLvalue(ctx->lvalue()),
                visitAssignmentOperator(ctx->assignmentOperator()),
                visitExpressionNoComma(ctx->expressionNoComma()),
                ctx->getStart()
        ));
    }
}

//--------------------------------------------------------------------------------------------------------
// Unary Expressions

antlrcpp::Any ASTVisitor::visitPrefixUnaryExpression(CaramelParser::PrefixUnaryExpressionContext *ctx) {
    logger.trace() << "visiting prefix unary expression: " << grey << ctx->getText();

    antlrcpp::Any expression = visitPostfixUnaryExpression(ctx->postfixUnaryExpression());

    if (ctx->prefixUnaryOperator()) {
        return castTo<Expression::Ptr>(std::make_shared<UnaryExpression>(
                visitPostfixUnaryExpression(ctx->postfixUnaryExpression()),
                visitPrefixUnaryOperator(ctx->prefixUnaryOperator()),
                ctx->getStart()
        ));
    } else {
        return visitChildren(ctx).as<Expression::Ptr>();
    }
}

antlrcpp::Any
ASTVisitor::visitPostfixUnaryExpression(CaramelParser::PostfixUnaryExpressionContext *ctx) {
    logger.trace() << "visiting postfix unary expression: " << grey << ctx->getText();

    antlrcpp::Any atomicExpression = visitAtomicExpression(ctx->atomicExpression());
    auto *postfixCtx = ctx->postfixUnaryOperation();
    if (postfixCtx) {

        if (postfixCtx->postfixUnaryOperator()) {
            if (castAnyTo<Expression::Ptr, Identifier::Ptr>(atomicExpression)) {

                Expression::Ptr expression = atomicExpression;
                return castTo<Expression::Ptr>(std::make_shared<UnaryExpression>(
                        expression,
                        visitPostfixUnaryOperator(postfixCtx->postfixUnaryOperator()),
                        ctx->getStart()
                ));

            } else if (atomicExpression.is<VariableSymbol::Ptr>()) {
                logger.fatal() << "Cannot execute postfix operation on rvalue";
            }
        }


    }

    if (atomicExpression.is<Symbol::Ptr>()) {
        return castAnyTo<Symbol::Ptr, AtomicExpression::Ptr>(atomicExpression);
    } else {
        return atomicExpression.as<Expression::Ptr>();
    }
}

//--------------------------------------------------------------------------------------------------------
// Binary Operators

antlrcpp::Any
ASTVisitor::visitAdditiveOperator(CaramelParser::AdditiveOperatorContext *ctx) {
    logger.trace() << "visiting additive operator: " << grey << ctx->getText();
    return FIND_BINARY_OP(ctx);
}

antlrcpp::Any
ASTVisitor::visitMultiplicativeOperator(CaramelParser::MultiplicativeOperatorContext *ctx) {
    logger.trace() << "visiting multiplicative operator: " << grey << ctx->getText();
    return FIND_BINARY_OP(ctx);
}

antlrcpp::Any ASTVisitor::visitBitwiseShiftOperator(CaramelParser::BitwiseShiftOperatorContext *ctx) {
    logger.trace() << "visiting bitwise shift operator: " << grey << ctx->getText();
    return FIND_BINARY_OP(ctx);
}

antlrcpp::Any ASTVisitor::visitComparativeOperator(CaramelParser::ComparativeOperatorContext *ctx) {
    logger.trace() << "visiting comparative operator: " << grey << ctx->getText();
    return FIND_BINARY_OP(ctx);
}

antlrcpp::Any ASTVisitor::visitEqualityOperator(CaramelParser::EqualityOperatorContext *ctx) {
    logger.trace() << "visiting equality operator: " << grey << ctx->getText();
    return FIND_BINARY_OP(ctx);
}

antlrcpp::Any ASTVisitor::visitAssignmentOperator(CaramelParser::AssignmentOperatorContext *ctx) {
    logger.trace() << "visiting assignment operator: " << grey << ctx->getText();
    return FIND_BINARY_OP(ctx);
}

//--------------------------------------------------------------------------------------------------------
// Unary Operators

antlrcpp::Any ASTVisitor::visitPrefixUnaryOperator(CaramelParser::PrefixUnaryOperatorContext *ctx) {
    logger.trace() << "visiting unary prefix operator: " << grey << ctx->getText();
    return FIND_PREFIX_OP(ctx);
}

antlrcpp::Any ASTVisitor::visitPostfixUnaryOperator(CaramelParser::PostfixUnaryOperatorContext *ctx) {
    logger.trace() << "visiting unary postfix operator: " << grey << ctx->getText();
    return FIND_POSTFIX_OP(ctx);
}

//--------------------------------------------------------------------------------------------------------
// LValue

antlrcpp::Any ASTVisitor::visitLvalue(CaramelParser::LvalueContext *ctx) {
    logger.trace() << "visiting lvalue: " << grey << ctx->getText();

    std::string varName = visitValidIdentifier(ctx->validIdentifier());
    if (ctx->arrayAccess()) {
        Expression::Ptr index = visitExpression(ctx->arrayAccess()->expression());
        ArrayAccess::Ptr arrayAccess = std::make_shared<ArrayAccess>(index, ctx->getStart());
        ArraySymbol::Ptr variableSymbol = currentContext()->getSymbolTable()->addArrayAccess(
                ctx, varName, arrayAccess);
        arrayAccess->setSymbol(variableSymbol);
        return castTo<Expression::Ptr>(arrayAccess);
    } else {
        Identifier::Ptr identifier = std::make_shared<Identifier>(ctx->getStart());
        Symbol::Ptr variableSymbol = currentContext()->getSymbolTable()->addVariableUsage(ctx, varName, identifier);
        identifier->setSymbol(variableSymbol);
        return castTo<Expression::Ptr>(identifier);
    }
}

//--------------------------------------------------------------------------------------------------------
// RValue

antlrcpp::Any ASTVisitor::visitNumberConstant(CaramelParser::NumberConstantContext *ctx) {
    logger.trace() << "visiting number constant: " << grey << ctx->getText();

    long long value = std::stoll(ctx->getText());
    return castTo<AtomicExpression::Ptr>(std::make_shared<Constant>(value, ctx->start));
}

antlrcpp::Any ASTVisitor::visitCharConstant(CaramelParser::CharConstantContext *ctx) {
    logger.trace() << "visiting char constant: " << grey << ctx->getText();

    char value = ctx->getText().at(1);
    if (value == '\\') {
        if (ctx->getText() == "'\\n'") value = '\n';
        else if (ctx->getText() == "'\\r'") value = '\r';
        else if (ctx->getText() == "'\\t'") value = '\t';
        else if (ctx->getText() == "'\\''") value = '\'';
        else if (ctx->getText() == "'\\\"'") value = '"';
        else if (ctx->getText() == "'\\\\'") value = '\\';
        else if (ctx->getText() == "'\\0'") value = '\0';
    }
    return castTo<AtomicExpression::Ptr>(std::make_shared<Constant>(value, ctx->start));
}

antlrcpp::Any ASTVisitor::visitPositiveConstant(CaramelParser::PositiveConstantContext *ctx) {
    logger.trace() << "visiting positive constant: " << grey << ctx->getText();

    long long value = std::stoll(ctx->getText());
    return castTo<AtomicExpression::Ptr>(std::make_shared<Constant>(value, ctx->getStart()));
}
