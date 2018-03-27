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
#include "../datastructure/statements/expressions/binaryexpression/BinaryExpression.h"

using namespace caramel::ast;
using namespace caramel::visitors;

antlrcpp::Any AbstractSyntaxTreeVisitor::visitAdditiveExpression(CaramelParser::AdditiveExpressionContext *ctx) {
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
    return std::dynamic_pointer_cast<BinaryOperator>(mPlusOperator);
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitExpression(CaramelParser::ExpressionContext *ctx) {
    return CaramelBaseVisitor::visitExpression(ctx);
}
