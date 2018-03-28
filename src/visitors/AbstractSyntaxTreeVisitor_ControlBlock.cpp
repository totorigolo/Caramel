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
#include "../datastructure/statements/controlblocks/IfBlock.h"
#include "../datastructure/statements/controlblocks/WhileBlock.h"
#include "../Logger.h"
#include "../datastructure/statements/controlblocks/ForBlock.h"


using namespace caramel::ast;
using namespace caramel::visitors;


antlrcpp::Any AbstractSyntaxTreeVisitor::visitControlBlock(CaramelParser::ControlBlockContext *ctx) {
    using namespace caramel::ast;
    if (nullptr != ctx-> ifBlock()) {
        return std::dynamic_pointer_cast<ControlBlock>(visitIfBlock(ctx->ifBlock()).as<IfBlock::Ptr>());
    } else if (nullptr != ctx->whileBlock()){
        return std::dynamic_pointer_cast<ControlBlock>(visitWhileBlock(ctx->whileBlock()).as<WhileBlock::Ptr>());
    } else if (nullptr != ctx->forBlock()) {
        return std::dynamic_pointer_cast<ControlBlock>(visitForBlock(ctx->forBlock()).as<ForBlock::Ptr>());
    }

    return CaramelBaseVisitor::visitControlBlock(ctx);
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitIfBlock(CaramelParser::IfBlockContext *ctx) {
    using namespace caramel::ast;

    logger.trace() << "visit if block: ";

    IfBlock::Ptr ifBlock;
    Expression::Ptr expression = visitExpression(ctx->expression());
    logger.trace() << "if condition :";
    std::vector<Statement::Ptr> thenBlock = visitBlock(ctx->block(0));

    if (ctx->block().size()>1) {
        logger.trace() << "visit else :";
        std::vector<Statement::Ptr> elseBlock = visitBlock(ctx->block(1));
        ifBlock = std::make_shared<IfBlock>(expression,thenBlock,elseBlock,ctx->start);
    } else if (nullptr != ctx->ifBlock()) {
        logger.trace() << "visit else if :";
        Statement::Ptr elseIfBlock = std::dynamic_pointer_cast<Statement>(visitIfBlock(ctx->ifBlock()).as<IfBlock::Ptr>());
        std::vector<Statement::Ptr> elseIfBlockVector;
        elseIfBlockVector.push_back(elseIfBlock);
        ifBlock = std::make_shared<IfBlock>(expression,thenBlock,elseIfBlockVector,ctx->start);
    }

    return  ifBlock;
}


antlrcpp::Any AbstractSyntaxTreeVisitor::visitWhileBlock(CaramelParser::WhileBlockContext *ctx) {
    using namespace caramel::ast;

    logger.trace() << "visit while block: ";

    Expression::Ptr expression = visitExpression(ctx->expression());
    logger.trace() << "while condition :";
    std::vector<Statement::Ptr> block = visitBlock(ctx->block());

    WhileBlock::Ptr whileBlock = std::make_shared<WhileBlock>(expression,block,ctx->start);
    return whileBlock;
}


antlrcpp::Any AbstractSyntaxTreeVisitor::visitForBlock(CaramelParser::ForBlockContext *ctx) {
    using namespace caramel::ast;

    logger.trace() << "visit for block: ";

    Expression::Ptr begin = visitExpression(ctx->expression(0));
    Expression::Ptr end = visitExpression(ctx->expression(1));
    Expression::Ptr step = visitExpression(ctx->expression(2));
    logger.trace() << "for condition :";
    std::vector<Statement::Ptr> block = visitBlock(ctx->block());

    ForBlock::Ptr forblock = std::make_shared<ForBlock>(begin,end,step,block,ctx->start);
    return forblock;
}
