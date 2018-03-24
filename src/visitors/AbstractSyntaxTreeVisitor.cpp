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
#include "../datastructure/statements/Statement.h"
#include "../datastructure/statements/definition/VariableDefinition.h"
#include "../datastructure/symboltable/ArraySymbol.h"
#include "../datastructure/statements/declaration/ArrayDeclaration.h"


using namespace caramel::visitors;

AbstractSyntaxTreeVisitor::AbstractSyntaxTreeVisitor(std::string const &sourceFileName)
        : mSourceFileUtil{sourceFileName} {
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitR(CaramelParser::RContext *ctx) {
    pushNewContext();
    visitStatements(ctx->statements());
    return currentContext();
}

// Return vector<Statement::Ptr>
antlrcpp::Any AbstractSyntaxTreeVisitor::visitStatements(CaramelParser::StatementsContext *ctx) {

    using caramel::dataStructure::statements::Statement;

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
            logger.warning() << "Skipping unhandled statement:\n" << statement->getText();
        }
    }
    currentContext()->addStatements(std::move(statements));
    return {};
}

// Return std::shared_ptr<Statement>
antlrcpp::Any AbstractSyntaxTreeVisitor::visitStatement(CaramelParser::StatementContext *ctx) {
    // TODO: the "Skipping unhandled statement" problem seems to come from here
    return visitChildren(ctx);
}

// Return std::string
antlrcpp::Any
AbstractSyntaxTreeVisitor::visitValidIdentifier(CaramelParser::ValidIdentifierContext *ctx) {
    return ctx->getText();
}

// Return SymbolType::Ptr (in a Symbol::Ptr)
antlrcpp::Any
AbstractSyntaxTreeVisitor::visitTypeParameter(CaramelParser::TypeParameterContext *ctx) {
    logger.trace() << "type parameter: " << ctx->getText();

    using namespace caramel::dataStructure::symbolTable;

    Symbol::Ptr symbol = currentContext()->getSymbolTable()->getSymbol(ctx->getText());
    TypeSymbol::Ptr typeSymbol = std::dynamic_pointer_cast<TypeSymbol>(symbol);
    return typeSymbol;
}

// Return vector<std::shared_ptr<Statement>>
antlrcpp::Any
AbstractSyntaxTreeVisitor::visitVariableDeclaration(CaramelParser::VariableDeclarationContext *ctx) {

    // Statements
    using caramel::dataStructure::statements::Statement;
    using caramel::dataStructure::statements::declaration::VariableDeclaration;
    // Symbols
    using namespace caramel::dataStructure::symbolTable;

    logger.trace() << "Visiting variable declaration: " << ctx->getText();
    TypeSymbol::Ptr typeSymbol = visitTypeParameter(ctx->typeParameter()).as<TypeSymbol::Ptr>();
    std::vector<Statement::Ptr> variables;
    for (auto validIdentifierCtx : ctx->validIdentifier()) {
        std::string name = visitValidIdentifier(validIdentifierCtx);
        VariableSymbol::Ptr variableSymbol = std::make_shared<VariableSymbol>(name, typeSymbol);

        VariableDeclaration::Ptr variableDeclaration = std::make_shared<VariableDeclaration>(variableSymbol, validIdentifierCtx->getStart());
        variables.push_back(variableDeclaration);

        currentContext()->getSymbolTable()->addVariableDeclaration(ctx, typeSymbol->getType(), name,
                                                                   variableDeclaration);

        logger.trace() << "New variable declared " << name << " of type " << typeSymbol->getType()->getIdentifier();
    }
    return variables;
}

// Return vector<Statement::Ptr>
antlrcpp::Any AbstractSyntaxTreeVisitor::visitVariableDefinition(CaramelParser::VariableDefinitionContext *ctx) {

    using namespace caramel::dataStructure::symbolTable;

    using caramel::dataStructure::statements::Statement;
    using caramel::dataStructure::statements::definition::VariableDefinition;
    using caramel::dataStructure::statements::expressions::Expression;

    logger.trace() << "Visiting variable definition: " << ctx->getText();

    TypeSymbol::Ptr typeSymbol = visitTypeParameter(ctx->typeParameter()).as<TypeSymbol::Ptr>();
    std::vector<Statement::Ptr> variables;
    for(auto child : ctx->children) {
        if(nullptr != dynamic_cast<CaramelParser::ValidIdentifierContext*>(child)) {
            auto *identifierContext = dynamic_cast<CaramelParser::ValidIdentifierContext*>(child);
            std::string name = visitValidIdentifier(identifierContext);
            logger.trace() << "New variable declared: '" << name << "' with default value";
            VariableSymbol::Ptr variableSymbol = std::make_shared<VariableSymbol>(name, typeSymbol);
            VariableDefinition::Ptr variableDef = std::make_shared<VariableDefinition>(variableSymbol, identifierContext->getStart());
            variables.push_back(variableDef);

            currentContext()->getSymbolTable()->addVariableDefinition(ctx, typeSymbol->getType(), name, variableDef);

        } else if (nullptr != dynamic_cast<CaramelParser::VariableDefinitionAssignmentContext*>(child)) {
            auto *vdAssignmentContext = dynamic_cast<CaramelParser::VariableDefinitionAssignmentContext*>(child);
            std::string name = visitValidIdentifier(vdAssignmentContext->validIdentifier());
            Expression::Ptr expression = visitExpression(vdAssignmentContext->expression());
            logger.trace() << "New variable declared: '" << name << "' with value";
            VariableSymbol::Ptr variableSymbol = std::make_shared<VariableSymbol>(name, typeSymbol);
            VariableDefinition::Ptr variableDef = std::make_shared<VariableDefinition>(variableSymbol, expression, vdAssignmentContext->getStart());
            variables.push_back(variableDef);

            currentContext()->getSymbolTable()->addVariableDefinition(ctx, typeSymbol->getType(), name, variableDef);
        }
    }

    return variables;

}

// Return vector<Statement::Ptr>
antlrcpp::Any
AbstractSyntaxTreeVisitor::visitFunctionDeclaration(CaramelParser::FunctionDeclarationContext *ctx) {

    using namespace caramel::dataStructure::symbolTable;
    using caramel::dataStructure::statements::declaration::FunctionDeclaration;


    logger.trace() << "Visiting function declaration: " << ctx->getText() << ' ' << ctx->getStart();

    auto innerCtx = ctx->functionDeclarationInner();

    PrimaryType::Ptr returnType = visitTypeParameter(innerCtx->typeParameter()).as<TypeSymbol::Ptr>()->getType();
    std::string name = visitValidIdentifier(innerCtx->validIdentifier());
    std::vector<Symbol::Ptr> params = visitFunctionArguments(innerCtx->functionArguments());
    FunctionDeclaration::Ptr functionDeclaration = std::make_shared<FunctionDeclaration>(std::make_shared<FunctionSymbol>(name, returnType), ctx->start);
    currentContext()->getSymbolTable()->addFunctionDeclaration(ctx, returnType, name, params,
                                                               functionDeclaration);

    auto traceLogger = logger.trace();
    traceLogger << "New function declared " << name << " with return type " << returnType->getIdentifier();
    for (Symbol::Ptr const &param : params) {
        traceLogger << "\n\t- param: " << param->getName() << " as " << param->getType()->getIdentifier();
    }
    traceLogger.show();

    return functionDeclaration;
}

// Return vector<Statement::Ptr>
antlrcpp::Any AbstractSyntaxTreeVisitor::visitFunctionDefinition(CaramelParser::FunctionDefinitionContext *ctx) {
    pushNewContext();
    antlrcpp::Any result = CaramelBaseVisitor::visitFunctionDefinition(ctx);
    popContext();
    return result;
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitFunctionArguments(CaramelParser::FunctionArgumentsContext *ctx) {

    using namespace caramel::dataStructure::symbolTable;

    logger.debug() << "Visiting named arguments: " << ctx->getText();

    std::vector<Symbol::Ptr> params;
    for (auto argument : ctx->functionArgument()) {
        params.push_back(visitFunctionArgument(argument).as<VariableSymbol::Ptr>());
    }
    return params;
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitFunctionArgument(CaramelParser::FunctionArgumentContext *ctx) {

    using namespace caramel::dataStructure::symbolTable;

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
        return std::make_shared<VariableSymbol>(name, type); // FIXME: Return a new ArrayVariableSymbol
    } else {
        return std::make_shared<VariableSymbol>(name, type);
    }
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitIfBlock(CaramelParser::IfBlockContext *ctx) {
    return CaramelBaseVisitor::visitIfBlock(ctx);
}

// Return Expression::Ptr
antlrcpp::Any AbstractSyntaxTreeVisitor::visitAtomicExpression(CaramelParser::AtomicExpressionContext *ctx) {

    using caramel::dataStructure::statements::expressions::atomicExpression::AtomicExpression;

    if(nullptr != ctx->validIdentifier()) {
        std::string varName = visitValidIdentifier(ctx->validIdentifier());
        AtomicExpression::Ptr atomicExpression = std::make_shared<AtomicExpression>(ctx->getStart());
        currentContext()->getSymbolTable()->addVariableUsage(ctx, varName, atomicExpression);
    }
    return CaramelBaseVisitor::visitAtomicExpression(ctx);
}

// Return Expression::Ptr
antlrcpp::Any AbstractSyntaxTreeVisitor::visitExpression(CaramelParser::ExpressionContext *ctx) {

    using caramel::dataStructure::statements::expressions::Expression;

    return std::make_shared<Expression>(ctx->getStart());
}

// Return Constant::Ptr
antlrcpp::Any AbstractSyntaxTreeVisitor::visitNumberConstant(CaramelParser::NumberConstantContext *ctx) {

    using caramel::dataStructure::statements::expressions::atomicExpression::Constant;

    long long value = std::stoll(ctx->getText());
    return std::make_shared<Constant>(value, ctx->start);
}

// Return Constant::Ptr
antlrcpp::Any AbstractSyntaxTreeVisitor::visitCharConstant(CaramelParser::CharConstantContext *ctx) {

    using caramel::dataStructure::statements::expressions::atomicExpression::Constant;

    char value = ctx->getText().at(0);
    return std::make_shared<Constant>(value, ctx->start);
}


// Private
void AbstractSyntaxTreeVisitor::pushNewContext() {

    using namespace caramel::dataStructure::symbolTable;

    using caramel::dataStructure::context::Context;

    logger.debug() << "Pushed a new context.";

    SymbolTable::Ptr parentTable;
    if(!mContextStack.empty()) {
        Context::Ptr parent = mContextStack.top();
        mContextStack.push(std::make_shared<Context>(parent));
    } else {
        mContextStack.push(std::make_shared<Context>());
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

caramel::dataStructure::context::Context::Ptr AbstractSyntaxTreeVisitor::currentContext() {
    return mContextStack.top();
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitArrayDefinition(CaramelParser::ArrayDefinitionContext *ctx) {
    Symbol::Ptr arraySymbol;
    auto arrayDeclaration;

    if (nullptr != ctx->arrayDeclarationVoidInner()) {
        arraySymbol = visitArrayDeclarationVoidInner(ctx->arrayDeclarationVoidInner());

        long arraySize = visitArrayBlock(ctx->arrayBlock());
        dynamic_cast<ArraySymbol*>(arraySymbol)->setSize(arraySize);

        arrayDeclaration = ArrayDeclaration::Create(arraySymbol, ctx->arrayDeclarationVoidInner()->validIdentifier()->getStart());

    } else if (nullptr != ctx->arrayDeclarationInner()) {
        arraySymbol = visitArrayDeclarationInner(ctx->arrayDeclarationInner());


        arrayDeclaration = ArrayDeclaration::Create(arraySymbol, ctx->arrayDeclarationInner()->validIdentifier()->getStart());

    }

    currentContext()->getSymbolTable()->addVariableDeclaration(ctx, arraySymbol->getType(), arraySymbol->getName(),
                                                               arrayDeclaration);


    logger.trace() << "New array declared " << arraySymbol->getName() << " of return type " << arraySymbol->getType()  << "and size " << dynamic_cast<ArraySymbol*>(arraySymbol)->getSize();

    //TODO : Cas d'erreur
    return arrayDeclaration;
}

// Return Symbol::Ptr
antlrcpp::Any
AbstractSyntaxTreeVisitor::visitArrayDeclarationVoidInner(CaramelParser::ArrayDeclarationVoidInnerContext *ctx) {
    logger.trace() << "Visiting array declaration void: " << ctx->getText();

    auto typeSymbol = visitTypeParameter(ctx->typeParameter()).as<TypeSymbol::Ptr>();
    std::string name = visitValidIdentifier(ctx->validIdentifier());
    auto arraySymbol{ArraySymbol::Create(name, typeSymbol, 0)};

    return arraySymbol;
}

// Return long
antlrcpp::Any AbstractSyntaxTreeVisitor::visitArrayBlock(CaramelParser::ArrayBlockContext *ctx) {
    return ctx->expression().size();
}

// Return Symbol::Ptr
antlrcpp::Any AbstractSyntaxTreeVisitor::visitArrayDeclarationInner(CaramelParser::ArrayDeclarationInnerContext *ctx) {
    logger.trace() << "Visiting array declaration : " << ctx->getText();

    auto typeSymbol = visitTypeParameter(ctx->typeParameter()).as<TypeSymbol::Ptr>();
    std::string name = visitValidIdentifier(ctx->validIdentifier());
    Constant::Ptr arraySize = visitArraySizeDeclaration(ctx->arraySizeDeclaration());

    auto arraySymbol{ArraySymbol::Create(name, typeSymbol, dynamic_cast<long>(arraySize->getValue()))};

    return arraySymbol;
}

// Return Constant::Ptr
antlrcpp::Any AbstractSyntaxTreeVisitor::visitArraySizeDeclaration(CaramelParser::ArraySizeDeclarationContext *ctx) {
    return visitPositiveConstant(ctx->positiveConstant());
}

// Return Constant::Ptr
antlrcpp::Any AbstractSyntaxTreeVisitor::visitPositiveConstant(CaramelParser::PositiveConstantContext *ctx) {
    long long value = std::stoll(ctx->getText());
    return Constant::Create(value, ctx->start);
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitFunctionDefinition(CaramelParser::FunctionDefinitionContext *ctx) {
    pushNewContext();
    return CaramelBaseVisitor::visitFunctionDefinition(ctx);
void AbstractSyntaxTreeVisitor::popContext() {
    logger.debug() << "Pop context.";
    mContextStack.pop();
}
