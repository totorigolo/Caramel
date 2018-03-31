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

#include "ASTVisitor.h"
#include "../Logger.h"
#include "../utils/Common.h"
#include "../ast/statements/jumps/Jump.h"
#include "../ast/statements/jumps/ReturnStatement.h"
#include "../ast/statements/controlblocks/ControlBlock.h"
#include "../ast/statements/expressions/atomicexpression/Constant.h"


using namespace caramel::ast;
using namespace caramel::utils;
using namespace caramel::colors;
using namespace caramel::visitors;

ASTVisitor::ASTVisitor(std::string const &sourceFileName)
        : mSourceFileUtil{sourceFileName},
          mBitwiseShiftOperator{std::make_shared<BitwiseShiftOperator>()},
          mMultOperator{std::make_shared<MultOperator>()},
          mPlusOperator{std::make_shared<PlusOperator>()} {
}

antlrcpp::Any ASTVisitor::visitR(CaramelParser::RContext *ctx) {
    logger.trace() << "Visiting R: " << grey << ctx->getText();

    ContextPusher contextPusher(*this);
    Context::Ptr context = contextPusher.getContext();

    SymbolTable::Ptr symbolTable = context->getSymbolTable();

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

    context->addStatements(visitStatements(ctx->statements()));
    return context;
}

antlrcpp::Any ASTVisitor::visitStatements(CaramelParser::StatementsContext *ctx) {
    logger.trace() << "visiting statements: " << grey <<ctx->getText();

    std::vector<Statement::Ptr> statements;
    for (auto statement : ctx->statement()) {
        logger.trace() << "[statements] visiting statement: " << grey << statement->getText();

        antlrcpp::Any r = visitStatement(statement);
        if (r.is<Statement::Ptr>()) {
            statements.push_back(r.as<Statement::Ptr>());
            logger.debug() << green << "Yay statement:\n" << statement->getText();
        } else if (r.is<std::vector<Statement::Ptr>>()) {
            auto statementVector = r.as<std::vector<Statement::Ptr>>();
            std::copy(statementVector.begin(), statementVector.end(), std::back_inserter(statements));
            logger.debug() << green << "Yay statement:\n" << statement->getText();
        } else {
            logger.warning() << "Skipping unhandled statement:\n" << statement->getText();
        }
    }
    return statements;
}

antlrcpp::Any ASTVisitor::visitBlock(CaramelParser::BlockContext *ctx) {
    logger.trace() << "visiting block: " << grey <<ctx->getText();

    std::vector<Statement::Ptr> returnStatements;

    if (ctx->declarations()) {
        std::vector<Statement::Ptr> declarations = visitDeclarations(ctx->declarations());
        //currentContext()->addStatements(std::move(declarations));
        std::move(declarations.begin(),declarations.end(),std::back_inserter(returnStatements));
    }
    if (ctx->instructions()) {
        std::vector<Statement::Ptr> instructions = visitInstructions(ctx->instructions());
        //currentContext()->addStatements(std::move(instructions));
        std::move(instructions.begin(),instructions.end(),std::back_inserter(returnStatements));
    }

    return returnStatements;
}

antlrcpp::Any ASTVisitor::visitDeclarations(CaramelParser::DeclarationsContext *ctx) {
    logger.trace() << "visiting declarations: " << grey <<ctx->getText();

    std::vector<Statement::Ptr> declarations;
    for (auto declaration : ctx->declaration()) {
        antlrcpp::Any r = visitDeclaration(declaration);
        if (r.is<Statement::Ptr>()) {
            declarations.push_back(r.as<Statement::Ptr>());
        } else if (r.is<std::vector<Statement::Ptr>>()) {
            auto declarationVector = r.as<std::vector<Statement::Ptr>>();
            std::move(declarationVector.begin(), declarationVector.end(), std::back_inserter(declarations));
        } else {
            logger.warning() << "Skipping unhandled declaration:\n" << declaration->getText();
        }
    }
    return declarations;
}

antlrcpp::Any ASTVisitor::visitInstructions(CaramelParser::InstructionsContext *ctx) {
    logger.trace() << "visiting instructions: " << grey <<ctx->getText();

    std::vector<Statement::Ptr> instructions;
    for (auto instructionCtx : ctx->instruction()) {
        Statement::Ptr instructionAsStatement = visitInstruction(instructionCtx);
        instructions.push_back(instructionAsStatement);
    }
    return instructions;
}

antlrcpp::Any ASTVisitor::visitValidIdentifier(CaramelParser::ValidIdentifierContext *ctx) {
    logger.trace() << "visiting identifier: " << grey << ctx->getText();
    return ctx->getText();
}

antlrcpp::Any
ASTVisitor::visitTypeParameter(CaramelParser::TypeParameterContext *ctx) {
    logger.trace() << "visiting type parameter: " << grey << ctx->getText();

    std::string symbolName = ctx->getText();
    if (currentContext()->getSymbolTable()->hasSymbol(symbolName)) {
        Symbol::Ptr symbol = currentContext()->getSymbolTable()->getSymbol(ctx, symbolName);
        return castTo<TypeSymbol::Ptr>(symbol);
    } else {
        logger.warning() << "Default symbol " << symbolName << " is created with void_t as return type";
        return std::make_shared<TypeSymbol>( symbolName, Void_t::Create() );
    }
}

antlrcpp::Any ASTVisitor::visitInstruction(CaramelParser::InstructionContext *ctx) {
    logger.trace() << "visiting instruction: " << grey <<ctx->getText();

    if (ctx->jump()) {
        return castAnyTo<Jump::Ptr, Statement::Ptr>(visitJump(ctx->jump()));
    } else if (ctx->controlBlock()) {
        return castAnyTo<ControlBlock::Ptr, Statement::Ptr>(visitControlBlock(ctx->controlBlock()));
    }

    return castAnyTo<Expression::Ptr, Statement::Ptr>(visitExpression(ctx->expression()));
}

antlrcpp::Any ASTVisitor::visitTypeDefinition(CaramelParser::TypeDefinitionContext *ctx) {
    logger.trace() << "visiting type definition: " << grey <<ctx->getText();

    TypeSymbol::Ptr primaryTypeSymbol = visitTypeParameter(ctx->typeParameter()[0]);
    TypeSymbol::Ptr typeAliasDefault = visitTypeParameter(ctx->typeParameter()[1]);

    TypeDefinition::Ptr typeDefinition = std::make_shared<TypeDefinition>(
            ctx->getStart(), typeAliasDefault->getName(), primaryTypeSymbol);
    currentContext()->getSymbolTable()->addType(ctx, typeDefinition);

    return castTo<Statement::Ptr>(typeDefinition);
}

antlrcpp::Any ASTVisitor::visitReturnJump(CaramelParser::ReturnJumpContext *ctx) {
    logger.trace() << "visiting return jump: " << grey <<ctx->getText();

    // Fixme : return true value

    ReturnStatement::Ptr returnStatement;
    Expression::Ptr returnedExpression;
    if (ctx->expression()) {
        returnedExpression = visitExpression(ctx->expression());
    } else {
        returnedExpression = castTo<Expression::Ptr>(Constant::defaultConstant(ctx->getStart()));
    }

    return castTo<Jump::Ptr>(std::make_shared<ReturnStatement>(returnedExpression, ctx->getStart()));
}

std::shared_ptr<Context> ASTVisitor::currentContext() {
    return mContextStack.top();
}

antlrcpp::Any ASTVisitor::visitChildren(antlr4::tree::ParseTree *node) {
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

ContextPusher::ContextPusher(ASTVisitor &ASTVisitor)
        : mASTVisitor(ASTVisitor) {
    logger.trace() << "ContextPusher: Trying to push a new context.";

    auto &contextStack = mASTVisitor.mContextStack;
    SymbolTable::Ptr parentTable;
    if (not contextStack.empty()) {
        Context::Ptr parent = contextStack.top();
        contextStack.push(std::make_shared<Context>(parent));
    } else {
        contextStack.push(std::make_shared<Context>());
    }

    logger.debug() << "Pushed a new context.";
}

ContextPusher::~ContextPusher() {
    logger.debug() << "Pop context: " << *mASTVisitor.mContextStack.top();
    mASTVisitor.mContextStack.pop();
}

Context::Ptr ContextPusher::getContext() {
    return mASTVisitor.currentContext();
}
