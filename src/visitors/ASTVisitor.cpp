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
#include "../ast/statements/jumps/BreakStatement.h"
#include "../ast/statements/jumps/ReturnStatement.h"
#include "../ast/statements/controlblocks/ControlBlock.h"
#include "../ast/statements/definition/FunctionDefinition.h"
#include "../ast/statements/declaration/FunctionDeclaration.h"
#include "../ast/statements/expressions/atomicexpression/Constant.h"
#include "../exceptions/SemanticError.h"


using namespace caramel::ast;
using namespace caramel::utils;
using namespace caramel::colors;
using namespace caramel::visitors;

ASTVisitor::ASTVisitor(std::string const &sourceFileName)
        : mSourceFilename{sourceFileName} {
}

antlrcpp::Any ASTVisitor::visitR(CaramelParser::RContext *ctx) {
    logger.trace() << "Visiting R: " << grey << ctx->getText();

    ContextPusher contextPusher(*this);
    Context::Ptr context = contextPusher.getContext();

    SymbolTable::Ptr symbolTable = context->getSymbolTable();

    // Add types
    logger.debug() << "Adding primary types.";
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

    // Add prelude functions
    // puts
    logger.debug() << "Adding prelude functions.";
    // TODO: Declare puts in prelude when constString_t will be handled
//    auto putsDecl = std::make_shared<FunctionDeclaration>(ctx->getStart());
//    std::vector<FunctionParameterSignature> putsParams = {
//            {"c", char_t, SymbolType::VariableSymbol}
//    };
//    auto putsSymbol = symbolTable->addFunctionDeclaration(ctx, void_t, "puts", putsParams, putsDecl);
//    putsDecl->setFunctionSymbol(putsSymbol);
    // printf
    auto printfDecl = std::make_shared<FunctionDeclaration>(ctx->getStart());
    std::vector<FunctionParameterSignature> printfParams = {};
    auto printfSymbol = symbolTable->addFunctionDeclaration(ctx, void_t, "printf", printfParams, printfDecl, true);
    printfDecl->setFunctionSymbol(printfSymbol);
    // putchar
    auto putcharDecl = std::make_shared<FunctionDeclaration>(ctx->getStart());
    std::vector<FunctionParameterSignature> putcharParams = {
            {"c", int32_t, SymbolType::VariableSymbol}
    };
    auto putcharSymbol = symbolTable->addFunctionDeclaration(ctx, void_t, "putchar", putcharParams, putcharDecl);
    putcharDecl->setFunctionSymbol(putcharSymbol);
    // getchar
    auto getcharDecl = std::make_shared<FunctionDeclaration>(ctx->getStart());
    std::vector<FunctionParameterSignature> getcharParams = {};
    auto getcharSymbol = symbolTable->addFunctionDeclaration(ctx, int32_t, "getchar", getcharParams, getcharDecl);
    getcharDecl->setFunctionSymbol(getcharSymbol);
    // exit
    auto exitDecl = std::make_shared<FunctionDeclaration>(ctx->getStart());
    std::vector<FunctionParameterSignature> exitParams = {
            {"code", int32_t, SymbolType::VariableSymbol}
    };
    auto exitSymbol = symbolTable->addFunctionDeclaration(ctx, void_t, "exit", exitParams, exitDecl);
    exitDecl->setFunctionSymbol(exitSymbol);

    try {context->addStatements(visitStatements(ctx->statements()));}
    catch(caramel::exceptions::SemanticError &semanticError){
        incrementErrorCount();
        semanticError.explain(utils::SourceFileUtil(mSourceFilename));
    }
    return context;
}

antlrcpp::Any ASTVisitor::visitStatements(CaramelParser::StatementsContext *ctx) {
    logger.trace() << "visiting statements: " << grey <<ctx->getText();

    std::vector<Statement::Ptr> statements;
    for (auto statement : ctx->statement()) {
        logger.trace() << "[statements] visiting statement: " << grey << statement->getText();

        antlrcpp::Any r;
        try {r = visitStatement(statement);}
        catch(caramel::exceptions::SemanticError &semanticError){
            incrementErrorCount();
            semanticError.explain(utils::SourceFileUtil(mSourceFilename));
        }
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
        std::vector<Statement::Ptr> declarations;
        try {
            std::vector<Statement::Ptr> r = visitDeclarations( ctx->declarations() );
            declarations = std::move(r); // TODO: Workaround
        }
        catch(caramel::exceptions::SemanticError &semanticError){
            incrementErrorCount();
            semanticError.explain(utils::SourceFileUtil(mSourceFilename));
        }
        //currentContext()->addStatements(std::move(declarations));
        std::move(declarations.begin(),declarations.end(),std::back_inserter(returnStatements));
    }
    if (ctx->instructions()) {
        std::vector<Statement::Ptr> instructions;
        try {
            std::vector<Statement::Ptr> i = visitInstructions(ctx->instructions());
            instructions = std::move(i); // TODO: Workaround
        }
        catch(caramel::exceptions::SemanticError &semanticError){
            incrementErrorCount();
            semanticError.explain(utils::SourceFileUtil(mSourceFilename));
        }
        //currentContext()->addStatements(std::move(instructions));
        std::move(instructions.begin(),instructions.end(),std::back_inserter(returnStatements));
    }

    return returnStatements;
}

antlrcpp::Any ASTVisitor::visitDeclarations(CaramelParser::DeclarationsContext *ctx) {
    logger.trace() << "visiting declarations: " << grey <<ctx->getText();

    std::vector<Statement::Ptr> declarations;
    for (auto declaration : ctx->declaration()) {
        antlrcpp::Any r;
        try {r = visitDeclaration(declaration);}
        catch(caramel::exceptions::SemanticError &semanticError){
            incrementErrorCount();
            semanticError.explain(utils::SourceFileUtil(mSourceFilename));
        }
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
    logger.trace() << "visiting valid identifier: " << grey << ctx->getText();
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
        throw std::runtime_error("Default symbol " + symbolName + " is created with void_t as return type");
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
    if (ctx->expression()) {
        Expression::Ptr returnedExpression = visitExpression(ctx->expression());
        return castTo<Jump::Ptr>(std::make_shared<ReturnStatement>(returnedExpression, ctx->getStart()));
    } else {
        return castTo<Jump::Ptr>(std::make_shared<ReturnStatement>(ctx->getStart()));
    }
}

antlrcpp::Any ASTVisitor::visitBreakJump(CaramelParser::BreakJumpContext *ctx) {
    logger.trace() << "visiting break jump: " << grey <<ctx->getText();

    return castTo<Jump::Ptr>(std::make_shared<BreakStatement>(ctx->getStart()));
}

std::shared_ptr<Context> ASTVisitor::rootContext() {
    Context::Ptr context = currentContext();
    while (context->getParent()) {
        context = context->getParent();
    }
    return context;
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

void ASTVisitor::incrementErrorCount(){
    mErrorCount++;
}

int ASTVisitor::getErrorCount(){
    return mErrorCount;
}

ContextPusher::ContextPusher(ASTVisitor &ASTVisitor)
        : mASTVisitor(ASTVisitor) {
    logger.trace() << "ContextPusher: Trying to push a new context.";

    auto &contextStack = mASTVisitor.mContextStack;
    if (not contextStack.empty()) {
        contextStack.push(std::make_shared<Context>(contextStack.top()));
    } else {
        contextStack.push(std::make_shared<Context>());
    }

    logger.debug() << "Pushed a new context.";
}

ContextPusher::~ContextPusher() {
    logger.trace() << "Popping context: " << *mASTVisitor.mContextStack.top();
    verifUsageStatic(mASTVisitor.mContextStack.top());
    logger.debug() << "Popped context: " << *mASTVisitor.mContextStack.top();
    mASTVisitor.mContextStack.pop();
}

Context::Ptr ContextPusher::getContext() {
    return mASTVisitor.currentContext();
}

void ContextPusher::verifUsageStatic(ast::Context::Ptr context) {
    logger.trace() << "Running static analysis on: " << *mASTVisitor.mContextStack.top();

    auto symbolTable = context->getSymbolTable();
    for (auto symbolMapElem : symbolTable->getSymbols()) {
        auto symbol = symbolMapElem.second;
        if (symbolTable->isDeclared(symbolMapElem.first)) {
            if (!symbolTable->isDefined(symbolMapElem.first)) {
                logger.warning() << "The element '" << symbolMapElem.first << "' was declared but never defined";
            }
        }
        if (symbolTable->isDefined(symbolMapElem.first)) {
            if (symbol->getSymbolType() != ast::SymbolType::TypeSymbol && symbol->getName() != "main") {
                if (symbol->getOccurrences().empty()) {
                    logger.warning() << "The element '" << symbolMapElem.first << "' was declared but never used";
                }
            }

        }
    }
}
