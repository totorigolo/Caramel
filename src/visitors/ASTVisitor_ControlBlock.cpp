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
#include "../ast/statements/controlblocks/IfBlock.h"
#include "../ast/statements/controlblocks/ForBlock.h"
#include "../ast/statements/controlblocks/WhileBlock.h"
#include "../ast/statements/expressions/binaryexpression/BinaryExpression.h"
#include "../ast/statements/controlblocks/Do_WhileBlock.h"


using namespace caramel::ast;
using namespace caramel::utils;
using namespace caramel::colors;
using namespace caramel::visitors;


antlrcpp::Any ASTVisitor::visitControlBlock(CaramelParser::ControlBlockContext *ctx) {
    logger.trace() << "visiting array block: " << grey <<ctx->getText();

    if (ctx->ifBlock()) {
        return castAnyTo<IfBlock::Ptr, ControlBlock::Ptr>(visitIfBlock(ctx->ifBlock()));
    } else if (ctx->whileBlock()) {
        return castAnyTo<WhileBlock::Ptr, ControlBlock::Ptr>(visitWhileBlock(ctx->whileBlock()));
    } else if (ctx->doWhileBlock()) {
        return castAnyTo<Do_WhileBlock::Ptr, ControlBlock::Ptr>(visitDoWhileBlock(ctx->doWhileBlock()));
    } else { // if (ctx->forBlock()) {
        return castAnyTo<ForBlock::Ptr, ControlBlock::Ptr>(visitForBlock(ctx->forBlock()));
    }
}

antlrcpp::Any ASTVisitor::visitIfBlock(CaramelParser::IfBlockContext *ctx) {
    logger.trace() << "visiting if block: " << grey <<ctx->getText();

    IfBlock::Ptr ifBlock;
    Expression::Ptr expression = visitExpression(ctx->expression());
    logger.trace() << "if condition :";
    std::vector<Statement::Ptr> thenBlock = visitBlock(ctx->block(0));

    if (ctx->block().size() > 1) {
        logger.trace() << "visit else :";
        std::vector<Statement::Ptr> elseBlock = visitBlock(ctx->block(1));
        ifBlock = std::make_shared<IfBlock>(expression, thenBlock, elseBlock, ctx->start);
    } else if (nullptr != ctx->ifBlock()) {
        logger.trace() << "visit else if :";
        Statement::Ptr elseIfBlock = std::dynamic_pointer_cast<Statement>(
                visitIfBlock(ctx->ifBlock()).as<IfBlock::Ptr>());
        std::vector<Statement::Ptr> elseIfBlockVector;
        elseIfBlockVector.push_back(elseIfBlock);
        ifBlock = std::make_shared<IfBlock>(expression, thenBlock, elseIfBlockVector, ctx->start);
    } else {
        ifBlock = std::make_shared<IfBlock>(expression, thenBlock, std::vector<Statement::Ptr>(), ctx->start);
    }

    return ifBlock;
}

antlrcpp::Any ASTVisitor::visitWhileBlock(CaramelParser::WhileBlockContext *ctx) {
    logger.trace() << "visiting while block: " << grey <<ctx->getText();

    Expression::Ptr expression = visitExpression(ctx->expression());
    logger.trace() << "while condition :";
    std::vector<Statement::Ptr> block = visitBlock(ctx->block());

    WhileBlock::Ptr whileBlock = std::make_shared<WhileBlock>(expression, block, ctx->start);
    return whileBlock;
}

antlrcpp::Any ASTVisitor::visitDoWhileBlock(CaramelParser::DoWhileBlockContext *ctx) {
    logger.trace() << "visiting do while block: " << grey <<ctx->getText();

    Expression::Ptr expression = visitExpression(ctx->expression());
    logger.trace() << "do while condition :";
    std::vector<Statement::Ptr> block = visitBlock(ctx->block());

    Do_WhileBlock::Ptr doWhileBlock = std::make_shared<Do_WhileBlock>(expression, block, ctx->start);
    return doWhileBlock;
}

antlrcpp::Any ASTVisitor::visitForBlock(CaramelParser::ForBlockContext *ctx) {
    logger.trace() << "visiting for block: " << grey <<ctx->getText();

    Expression::Ptr begin = visitExpression(ctx->expression(0));
    Expression::Ptr end = visitExpression(ctx->expression(1));
    Expression::Ptr step = visitExpression(ctx->expression(2));
    logger.trace() << "for condition :";
    std::vector<Statement::Ptr> block = visitBlock(ctx->block());

    ForBlock::Ptr forBlock = std::make_shared<ForBlock>(begin, end, step, block, ctx->start);
    return forBlock;
}
