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
#include "../ast/statements/declaration/FunctionDeclaration.h"
#include "../ast/statements/definition/FunctionDefinition.h"


using namespace caramel::ast;
using namespace caramel::utils;
using namespace caramel::colors;
using namespace caramel::visitors;

antlrcpp::Any
ASTVisitor::visitFunctionDeclaration(CaramelParser::FunctionDeclarationContext *ctx) {
    logger.trace() << "Visiting function declaration: " << grey << ctx->getText();

    auto innerCtx = ctx->functionDeclarationInner();

    PrimaryType::Ptr returnType = visitTypeParameter(innerCtx->typeParameter()).as<TypeSymbol::Ptr>()->getType();
    std::string name = visitValidIdentifier(innerCtx->validIdentifier());
    std::vector<std::tuple<std::string, PrimaryType::Ptr, SymbolType>> params =
            visitFunctionArguments(innerCtx->functionArguments());
    FunctionDeclaration::Ptr functionDeclaration = std::make_shared<FunctionDeclaration>(innerCtx->start);
    FunctionSymbol::Ptr functionSymbol = currentContext()->getSymbolTable()->addFunctionDeclaration(
            innerCtx, returnType, name, params, functionDeclaration);
    functionDeclaration->setFunctionSymbol(functionSymbol);

    auto traceLogger = logger.trace();
    traceLogger << "New function declared " << name << " with return type " << returnType->getIdentifier();
    for (auto const &[paramName, paramType, paramSymbolType] : params) {
        traceLogger << "\n\t- param: " << paramName << " as " << paramType->getIdentifier();
    }
    traceLogger.show();

    return castTo<Statement::Ptr>(functionDeclaration);
}

antlrcpp::Any ASTVisitor::visitFunctionDefinition(CaramelParser::FunctionDefinitionContext *ctx) {
    logger.trace() << "Visiting function definition: " << grey << ctx->getText();

    Context::Ptr parentContext = currentContext();
    ContextPusher contextPusher(*this);

    Context::Ptr functionContext = currentContext();

    auto innerCtx = ctx->functionDeclarationInner();

    PrimaryType::Ptr returnType = visitTypeParameter(innerCtx->typeParameter()).as<TypeSymbol::Ptr>()->getType();
    std::string name = visitValidIdentifier(innerCtx->validIdentifier());
    std::vector<std::tuple<std::string, PrimaryType::Ptr, SymbolType>> params =
            visitFunctionArguments(innerCtx->functionArguments());

    std::vector<Symbol::Ptr> paramsSymbols;
    for (auto const &[paramName, paramType, paramSymbolType] : params) {
        paramsSymbols.push_back(currentContext()->getSymbolTable()->addFunctionParameter(
                ctx, paramName, paramType, paramSymbolType));
    }

    FunctionDefinition::Ptr functionDefinition = std::make_shared<FunctionDefinition>(functionContext, innerCtx->start);
    FunctionSymbol::Ptr functionSymbol = parentContext->getSymbolTable()->addFunctionDefinition(
            innerCtx, functionContext, returnType, name, paramsSymbols, functionDefinition
    );
    functionDefinition->setSymbol(functionSymbol);

    for (auto &param : paramsSymbols) {
        param->addDefinition(functionDefinition);
    }

    functionContext->addStatements(visitBlock(ctx->block()));

    return castTo<Statement::Ptr>(functionDefinition);
}

antlrcpp::Any ASTVisitor::visitFunctionArguments(CaramelParser::FunctionArgumentsContext *ctx) {
    logger.trace() << "Visiting named arguments: " << grey << ctx->getText();

    std::vector<std::tuple<std::string, PrimaryType::Ptr, SymbolType>> params;
    for (auto argument : ctx->functionArgument()) {
        std::tuple<std::string, PrimaryType::Ptr, SymbolType> symbol = visitFunctionArgument(argument);
        params.push_back(symbol);
    }
    return params;
}

antlrcpp::Any ASTVisitor::visitFunctionArgument(CaramelParser::FunctionArgumentContext *ctx) {
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
        return std::make_tuple(name, type->getType(), SymbolType::ArraySymbol);
    } else {
        return std::make_tuple(name, type->getType(), SymbolType::VariableSymbol);
    }
}
