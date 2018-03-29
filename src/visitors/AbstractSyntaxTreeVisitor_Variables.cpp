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
#include "../datastructure/statements/declaration/VariableDeclaration.h"
#include "../datastructure/statements/definition/VariableDefinition.h"


using namespace caramel::ast;
using namespace caramel::util;
using namespace caramel::colors;
using namespace caramel::visitors;

antlrcpp::Any
AbstractSyntaxTreeVisitor::visitVariableDeclaration(CaramelParser::VariableDeclarationContext *ctx) {
    logger.trace() << "visiting variable declaration: " << grey <<ctx->getText();

    TypeSymbol::Ptr typeSymbol = visitTypeParameter(ctx->typeParameter()).as<TypeSymbol::Ptr>();
    std::vector<Statement::Ptr> variables;
    for (auto validIdentifierCtx : ctx->validIdentifier()) {
        std::string name = visitValidIdentifier(validIdentifierCtx);
        VariableSymbol::Ptr variableSymbol = std::make_shared<VariableSymbol>(name, typeSymbol);

        VariableDeclaration::Ptr variableDeclaration = std::make_shared<VariableDeclaration>(variableSymbol,
                                                                                             validIdentifierCtx->getStart());
        variables.push_back(variableDeclaration);

        currentContext()->getSymbolTable()->addVariableDeclaration(ctx, typeSymbol->getType(), name,
                                                                   variableDeclaration);

        logger.trace() << "New variable declared " << name << " of type " << typeSymbol->getType()->getIdentifier();
    }

    return variables;
}

antlrcpp::Any AbstractSyntaxTreeVisitor::visitVariableDefinition(CaramelParser::VariableDefinitionContext *ctx) {
    logger.trace() << "visiting variable definition: " << grey <<ctx->getText();

    TypeSymbol::Ptr typeSymbol = visitTypeParameter(ctx->typeParameter()).as<TypeSymbol::Ptr>();
    std::vector<Statement::Ptr> variables;
    for (auto varDefValue : ctx->validIdentifier()) {
        std::string name = visitValidIdentifier(varDefValue);
        logger.trace() << "New variable declared: '" << name << "' with default value";

        VariableDefinition::Ptr variableDef = std::make_shared<VariableDefinition>(varDefValue->getStart());
        variables.push_back(variableDef);

        VariableSymbol::Ptr variableSymbol = currentContext()->getSymbolTable()->addVariableDefinition(
                ctx, typeSymbol->getType(), name, variableDef);
        variableDef->setVariableSymbol(variableSymbol);
    }
    for (auto *varWithValue : ctx->variableDefinitionAssignment()) {
        std::string name = visitValidIdentifier(varWithValue->validIdentifier());

        Expression::Ptr expression = visitExpression(varWithValue->expression());
        logger.trace() << "New variable declared: '" << name << "' with value "
                       << varWithValue->expression()->getText();

        VariableDefinition::Ptr variableDef = std::make_shared<VariableDefinition>(
                expression, varWithValue->getStart());
        variables.push_back(variableDef);

        VariableSymbol::Ptr variableSymbol = currentContext()->getSymbolTable()->addVariableDefinition(
                ctx, typeSymbol->getType(), name, variableDef);
        variableDef->setVariableSymbol(variableSymbol);
    }

    return variables;
}
