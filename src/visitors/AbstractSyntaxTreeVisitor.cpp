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
#include "../datastructure/statements/expressions/atomicexpression/Constant.h"
#include "../datastructure/symboltable/PrimaryType.h"
#include "../datastructure/symboltable/VariableSymbol.h"
#include "../datastructure/symboltable/FunctionSymbol.h"
#include "../datastructure/statements/declaration/VariableDeclaration.h"
#include "../datastructure/statements/declaration/FunctionDeclaration.h"


using namespace Caramel::Visitors;

AbstractSyntaxTreeVisitor::AbstractSyntaxTreeVisitor(std::string const &sourceFileName)
        : mSourceFileUtil{sourceFileName} {
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitR(CaramelParser::RContext *ctx) {
    pushNewContext();
    return visitChildren(ctx).as<Context::Ptr>();
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitStatements(CaramelParser::StatementsContext *ctx) {
    std::vector<Statement::Ptr> statements;
    for (auto statement : ctx->statement()) {
        logger.debug() << "Visiting: " << mSourceFileUtil.getLine(statement->start->getLine());

        antlrcpp::Any r = visitStatement(statement);
        if (r.is<Statement::Ptr>()) {
            statements.push_back(r.as<Statement::Ptr>());
            logger.debug() << "Statement ptr";
        } else if (r.is<std::vector<Statement::Ptr>>()) {
            logger.debug() << "Vector<Statement>";
            auto statementVector = r.as<std::vector<Statement::Ptr>>();
            std::copy(statementVector.begin(), statementVector.end(), std::back_inserter(statements));
        } else {
            logger.fatal() << "Skipping unhandled statement: " << ctx->getText();
        }
    }
    return statements;
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitNumberConstant(CaramelParser::NumberConstantContext *ctx) {
    long long value = std::stoll(ctx->getText());
    return Constant::Create(value, ctx->start);
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitCharConstant(CaramelParser::CharConstantContext *ctx) {
    char value = ctx->getText().at(0);
    return Constant::Create(value, ctx->start);
}

antlrcpp::Any
AbstractSyntaxTreeVisitor::visitValidIdentifier(CaramelParser::ValidIdentifierContext *ctx) {
    return ctx->getText();
}

antlrcpp::Any
AbstractSyntaxTreeVisitor::visitTypeParameter(CaramelParser::TypeParameterContext *ctx) {
    logger.trace() << "type parameter: " << ctx->getText();
    return currentContext()->getSymbolTable()->getSymbol(ctx->getText());
}

antlrcpp::Any
AbstractSyntaxTreeVisitor::visitVariableDeclaration(CaramelParser::VariableDeclarationContext *ctx) {
    logger.trace() << "Visiting variable declaration: " << ctx->getText();

    auto typeSymbol = visitTypeParameter(ctx->typeParameter()).as<TypeSymbol::Ptr>();
    std::vector<Statement::Ptr> variables;
    for (auto validIdentifierCtx : ctx->validIdentifier()) {
        std::string name = visitValidIdentifier(validIdentifierCtx);
        auto variableSymbol{VariableSymbol::Create(name, typeSymbol)};

        auto variableDeclaration{VariableDeclaration::Create(variableSymbol, validIdentifierCtx->getStart())};
        variables.push_back(variableDeclaration);

        currentContext()->getSymbolTable()->addVariableDeclaration(ctx, typeSymbol->getType(), name,
                                                                   variableDeclaration);

        logger.trace() << "New variable declared " << name << " of type " << typeSymbol;
    }
    return variables;
}

antlrcpp::Any
AbstractSyntaxTreeVisitor::visitFunctionDeclaration(CaramelParser::FunctionDeclarationContext *ctx) {
    logger.trace() << "Visiting function declaration: " << ctx->getText() << ' ' << ctx->getStart();

    PrimaryType::Ptr returnType = visitTypeParameter(ctx->typeParameter()).as<Symbol::Ptr>()->getType();
    std::string name = visitValidIdentifier(ctx->validIdentifier());
    std::vector<Symbol::Ptr> params = visitFunctionArguments(ctx->functionArguments());

    auto functionDeclaration = FunctionDeclaration::Create(FunctionSymbol::Create(name, returnType), ctx->start);
    currentContext()->getSymbolTable()->addFunctionDeclaration(ctx, returnType, name, params,
                                                               functionDeclaration);

    auto traceLogger = logger.trace();
    traceLogger << "New function declared " << name << " with return type " << returnType;
    for (Symbol::Ptr const &param : params) {
        traceLogger << "\n\t- param: " << param->getName() << " as " << param->getType();
    }
    traceLogger.show();

    return functionDeclaration;
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitFunctionArguments(CaramelParser::FunctionArgumentsContext *ctx) {
    logger.debug() << "Visiting named arguments: " << ctx->getText();

    std::vector<Symbol::Ptr> params;
    for (auto argument : ctx->functionArgument()) {
        params.push_back(visitFunctionArgument(argument).as<Symbol::Ptr>());
    }
    return params;
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitFunctionArgument(CaramelParser::FunctionArgumentContext *ctx) {
    logger.trace() << "Visiting function argument: " << ctx->getText();

    // Get the optional name, or generate a unique one
    std::string name;
    if (nullptr != ctx->validIdentifier()) {
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
    if (nullptr != ctx->functionArgumentArraySuffix()) {
        return VariableSymbol::Create(name, type); // FIXME: Return a new ArrayVariableSymbol
    } else {
        return VariableSymbol::Create(name, type);
    }
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitIfBlock(CaramelParser::IfBlockContext *ctx) {
    return CaramelBaseVisitor::visitIfBlock(ctx);
}

void AbstractSyntaxTreeVisitor::pushNewContext() {
    logger.debug() << "Pushed a new context.";

    SymbolTable::Ptr parentTable;
    if(!mContextStack.empty()) {
        Context::Ptr parent = mContextStack.top();
        mContextStack.push(Context::Create(parent));
    } else {
        mContextStack.push(Context::Create());
    }


    SymbolTable::Ptr symbolTable = currentContext()->getSymbolTable();

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
}

Context::Ptr AbstractSyntaxTreeVisitor::currentContext() {
    return mContextStack.top();
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitArrayDeclarationVoid(CaramelParser::ArrayDeclarationVoidContext *ctx) {
    PrimaryType::Ptr typeName = visitTypeParameter(ctx->typeParameter());
    std::string name = visitValidIdentifier(ctx->validIdentifier());
    //currentContext()->getSymbolTable() // FIXME: Occurrence

    return {};
}
