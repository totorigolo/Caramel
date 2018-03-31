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
#include "../ast/statements/declaration/FunctionDeclaration.h"
#include "../ast/statements/definition/FunctionDefinition.h"


using namespace caramel::ast;
using namespace caramel::utils;
using namespace caramel::colors;
using namespace caramel::visitors;

antlrcpp::Any
AbstractSyntaxTreeVisitor::visitFunctionDeclaration(CaramelParser::FunctionDeclarationContext *ctx) {
    logger.trace() << "Visiting function declaration: " << grey << ctx->getText();

    auto innerCtx = ctx->functionDeclarationInner();

    PrimaryType::Ptr returnType = visitTypeParameter(innerCtx->typeParameter()).as<TypeSymbol::Ptr>()->getType();
    std::string name = visitValidIdentifier(innerCtx->validIdentifier());
    std::vector<Symbol::Ptr> params = visitFunctionArguments(innerCtx->functionArguments());
    FunctionDeclaration::Ptr functionDeclaration = std::make_shared<FunctionDeclaration>(innerCtx->start);
    FunctionSymbol::Ptr functionSymbol = currentContext()->getSymbolTable()->addFunctionDeclaration(
            innerCtx, returnType, name, params, functionDeclaration);
    functionDeclaration->setFunctionSymbol(functionSymbol);

    auto traceLogger = logger.trace();
    traceLogger << "New function declared " << name << " with return type " << returnType->getIdentifier();
    for (Symbol::Ptr const &param : params) {
        traceLogger << "\n\t- param: " << param->getName() << " as " << param->getType()->getIdentifier();
    }
    traceLogger.show();

    return castTo<Statement::Ptr>(functionDeclaration);
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitFunctionDefinition(CaramelParser::FunctionDefinitionContext *ctx) {
    logger.trace() << "Visiting function definition: " << grey << ctx->getText();

    Context::Ptr parentContext = currentContext();
    ContextPusher contextPusher(*this);

    Context::Ptr functionContext = currentContext();

    auto innerCtx = ctx->functionDeclarationInner();

    PrimaryType::Ptr returnType = visitTypeParameter(innerCtx->typeParameter()).as<TypeSymbol::Ptr>()->getType();
    std::string name = visitValidIdentifier(innerCtx->validIdentifier());
    std::vector<Symbol::Ptr> params = visitFunctionArguments(innerCtx->functionArguments());

    FunctionDefinition::Ptr functionDefinition = std::make_shared<FunctionDefinition>(functionContext, innerCtx->start);
    FunctionSymbol::Ptr functionSymbol = parentContext->getSymbolTable()->addFunctionDefinition(
            innerCtx, functionContext, returnType, name, params, functionDefinition
    );
    functionDefinition->setSymbol(functionSymbol);

    for (auto &param : params) {
        param->addDefinition(functionDefinition);
    }

    functionContext->addStatements(visitBlock(ctx->block()));

    return castTo<Statement::Ptr>(functionDefinition);
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitFunctionArguments(CaramelParser::FunctionArgumentsContext *ctx) {
    logger.trace() << "Visiting named arguments: " << grey << ctx->getText();

    std::vector<Symbol::Ptr> params;
    for (auto argument : ctx->functionArgument()) {
        Symbol::Ptr symbol = visitFunctionArgument(argument);
        params.push_back(symbol);
    }
    return params;
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitFunctionArgument(CaramelParser::FunctionArgumentContext *ctx) {
    logger.trace() << "Visiting function argument: " << grey << ctx->getText();

    // Get the optional name, or generate a unique one
    std::string name;
    if (ctx->validIdentifier()) {
        name = visitValidIdentifier(ctx->validIdentifier()).as<std::string>();
    } else {
        std::stringstream nameSS;
        nameSS << "__unnamed_argument_" << ctx->start->getLine() << "_" << ctx->start->getCharPositionInLine() << "__";
        name = nameSS.str();
    }

    // Get the type
    TypeSymbol::Ptr type;
    type = visitTypeParameter(ctx->typeParameter());

    // The argument is an array
    if (ctx->functionArgumentArraySuffix()) {
        logger.warning() << "Array function parameters aren't handled yet.";
        return currentContext()->getSymbolTable()->addFunctionParameter(
                ctx, name, type->getType(), SymbolType::ArraySymbol);
    } else {
        return currentContext()->getSymbolTable()->addFunctionParameter(
                ctx, name, type->getType(), SymbolType::VariableSymbol);
    }
}
