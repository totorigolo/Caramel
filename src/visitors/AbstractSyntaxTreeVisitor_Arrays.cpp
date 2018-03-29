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
#include "../util/Common.h"
#include "../datastructure/symboltable/ArraySymbol.h"
#include "../datastructure/statements/declaration/ArrayDeclaration.h"
#include "../datastructure/statements/definition/ArrayDefinition.h"
#include "../datastructure/statements/expressions/atomicexpression/Constant.h"
#include "../exceptions/ArrayBlockSizeExceedsDeclarationException.h"


using namespace caramel::ast;
using namespace caramel::util;
using namespace caramel::colors;
using namespace caramel::visitors;
using namespace caramel::exceptions;

antlrcpp::Any AbstractSyntaxTreeVisitor::visitArrayDeclaration(CaramelParser::ArrayDeclarationContext *ctx) {
    logger.trace() << "visiting array declaration: " << grey <<ctx->getText();

    bool sized = ctx->arraySizedInner() != nullptr;
    size_t size = {};

    std::string name;
    TypeSymbol::Ptr typeSymbol;

    if (sized) {
        name = visitValidIdentifier(ctx->arraySizedInner()->validIdentifier()).as<std::string>();
        typeSymbol = visitTypeParameter(ctx->arraySizedInner()->typeParameter());
        size = castAnyTo<AtomicExpression::Ptr, Constant::Ptr>(
                visitPositiveConstant(ctx->arraySizedInner()->positiveConstant()))->getValue();
    } else {
        name = visitValidIdentifier(ctx->arrayUnsizedInner()->validIdentifier()).as<std::string>();
        typeSymbol = visitTypeParameter(ctx->arrayUnsizedInner()->typeParameter());
    }

    ArrayDeclaration::Ptr arrayDeclaration = std::make_shared<ArrayDeclaration>(ctx->getStart());
    ArraySymbol::Ptr arraySymbol = currentContext()->getSymbolTable()->addArrayDeclaration(
            ctx, typeSymbol->getType(), name, sized, size, arrayDeclaration);
    arrayDeclaration->setSymbol(arraySymbol);

    return castTo<Statement::Ptr>(arrayDeclaration);
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitArrayDefinition(CaramelParser::ArrayDefinitionContext *ctx) {
    logger.trace() << "visiting array definition: " << grey <<ctx->getText();

    bool sized = ctx->arraySizedInner() != nullptr;
    size_t size = {};

    std::string name;
    TypeSymbol::Ptr typeSymbol;

    if (sized) {
        name = visitValidIdentifier(ctx->arraySizedInner()->validIdentifier()).as<std::string>();
        typeSymbol = visitTypeParameter(ctx->arraySizedInner()->typeParameter());
        size = castAnyTo<AtomicExpression::Ptr, Constant::Ptr>(
                visitPositiveConstant(ctx->arraySizedInner()->positiveConstant()))->getValue();
    } else {
        name = visitValidIdentifier(ctx->arrayUnsizedInner()->validIdentifier()).as<std::string>();
        typeSymbol = visitTypeParameter(ctx->arrayUnsizedInner()->typeParameter());
    }

    bool hasBlock = ctx->arrayBlock() != nullptr;
    std::vector<Expression::Ptr> content;
    if (hasBlock) {
        content = std::move(visitArrayBlock(ctx->arrayBlock()).as<std::vector<Expression::Ptr>>());

        if (sized && size < content.size()) {
                throw ArrayBlockSizeExceedsDeclarationException(
                        "Array " + name + " block size of " + std::to_string(content.size())
                        + " exceeds the declared size of " + std::to_string(size)
                );
        } else if (!sized) {
            size = content.size();
            sized = true;
        }
    }

    // Fill the block with default values to make it the same size as the declared size
    while (content.size() < size) {
        content.push_back(Constant::defaultConstant(ctx->getStart()));
    }

    ArrayDefinition::Ptr arrayDefinition = std::make_shared<ArrayDefinition>(ctx->getStart());
    ArraySymbol::Ptr arraySymbol = currentContext()->getSymbolTable()->addArrayDefinition(
            ctx, typeSymbol->getType(), name, std::move(content), arrayDefinition);
    arrayDefinition->setSymbol(arraySymbol);

    return castTo<Statement::Ptr>(arrayDefinition);
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitArrayBlock(CaramelParser::ArrayBlockContext *ctx) {
    logger.trace() << "visiting array block: " << grey <<ctx->getText();

    std::vector<Expression::Ptr> expressions;
    for (auto expression : ctx->expression()) {
        Expression::Ptr exp = visitExpression(expression);
        expressions.push_back(exp);
    }
    return expressions;
}
