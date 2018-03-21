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

#include "AbstractSyntaxTreeVisitor.h"
#include "../Logger.h"
#include "../datastructure/Constant.h"
#include "../datastructure/PrimaryType.h"
#include "../datastructure/VariableSymbol.h"
#include "../datastructure/FunctionSymbol.h"

using namespace Caramel::Visitors;

antlrcpp::Any AbstractSyntaxTreeVisitor::visitR(CaramelParser::RContext *ctx) {
    pushNewContext();
    return visitChildren(ctx).as<Context::Ptr>();
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitStatements(CaramelParser::StatementsContext *ctx) {
    std::vector<Statement::Ptr> statements;
    for (auto statement : ctx->statement()) {
        antlrcpp::Any r = visitStatement(statement);
        if (r.is<Statement::Ptr>()) {
            statements.push_back(r.as<Statement::Ptr>());
        } else {
            // A vector ¯\_(ツ)_/¯
            auto statementVector = r.as<std::vector<Statement::Ptr>>();
            std::copy(statementVector.begin(), statementVector.end(), std::back_inserter(statements));
        }
    }
    return statements;
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitNumberConstant(CaramelParser::NumberConstantContext *ctx) {
    long long value = std::stoll(ctx->getText());

    return Constant::Create(value);
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitCharConstant(CaramelParser::CharConstantContext *ctx) {
    return CaramelBaseVisitor::visitCharConstant(ctx);
}

antlrcpp::Any Caramel::Visitors::AbstractSyntaxTreeVisitor::visitValidIdentifier(CaramelParser::ValidIdentifierContext *ctx) {
    return ctx->getText();
}

antlrcpp::Any Caramel::Visitors::AbstractSyntaxTreeVisitor::visitTypeParameter(CaramelParser::TypeParameterContext *ctx) {
    std::string type = ctx->getText();
    // TODO: Checker in the SymbolTable

    if (type == "int8_t") {
        return Int8_t::Create();
    } else if (type == "int16_t") {
        return Int16_t::Create();
    } else if (type == "int32_t") {
        return Int32_t::Create();
    } else if (type == "int64_t") {
        return Int64_t::Create();
    } else {
        // TODO: Throw an exception
        return nullptr;
    }
}

antlrcpp::Any Caramel::Visitors::AbstractSyntaxTreeVisitor::visitVariableDeclaration(CaramelParser::VariableDeclarationContext *ctx) {
    PrimaryType::Ptr typeName = visitTypeParameter(ctx->typeParameter());
    std::vector<VariableSymbol::Ptr> variables;
    for (auto validIdentifierCtx : ctx->validIdentifier()) {
        std::string name = visitValidIdentifier(validIdentifierCtx);
        currentContext()->getSymbolTable()->addVariableDeclaration(typeName, name, nullptr); // FIXME: Occurrence
        variables.push_back(VariableSymbol::Create(name, typeName));
        logger.trace() << "New variable declared " << name << " of type " << typeName;
    }
    return variables ;
}


antlrcpp::Any
AbstractSyntaxTreeVisitor::visitFunctionDeclaration(CaramelParser::FunctionDeclarationContext *ctx) {
    PrimaryType::Ptr returnType = visitTypeParameter(ctx->typeParameter());
    std::string name = visitValidIdentifier(ctx->validIdentifier());
    std::vector<Symbol::Ptr> params = visitNamedArguments(ctx->namedArguments());
    currentContext()->getSymbolTable()->addFunctionDeclaration(returnType, name, params, nullptr); // FIXME: Occurrence
    logger.trace() <<  "New function declared " << name << " with return type " << returnType;
    for (Symbol::Ptr param: params) {
        logger.trace() << "\nparam : int" << param->getType()->getMemoryLength();
    }
    return (FunctionSymbol::Create(name,returnType));
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitNamedArguments(CaramelParser::NamedArgumentsContext *ctx) {
    std::vector<Symbol::Ptr> params;
    for (auto argument : ctx->namedArgument()) {
        params.push_back(visitNamedArgument(argument).as<Symbol::Ptr>());
    }
    return params;
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitNamedArgument(CaramelParser::NamedArgumentContext *ctx) {
    if (nullptr != ctx->variableDeclaration()) {
        std::vector<antlrcpp::Any> variables = visitVariableDeclaration(ctx->variableDeclaration());
        return variables.at(0).as<Symbol::Ptr>();
    } else if (nullptr != ctx->arrayDeclarationVoid()) {
        return visitArrayDeclarationVoid(ctx->arrayDeclarationVoid()).as<Symbol::Ptr>();
    } else if (nullptr != ctx->typeParameter()) {
        return visitTypeParameter(ctx->typeParameter()).as<Symbol::Ptr>();
    }

    // TODO: Throw an error
    return nullptr;
}

void AbstractSyntaxTreeVisitor::pushNewContext() {
    mContextStack.push(Context::Create());
}

Context::Ptr AbstractSyntaxTreeVisitor::currentContext() {
    return mContextStack.top();
}


