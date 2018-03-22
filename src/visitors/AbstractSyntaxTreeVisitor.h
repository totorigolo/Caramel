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

#pragma once

#include "../Console.h"
#include "../datastructure/Context.h"
#include "../util/SourceFileUtil.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wattributes"

#include <CaramelBaseVisitor.h>

#pragma GCC diagnostic pop

#include <stdexcept>
#include <iostream>

using namespace Caramel::DataStructure;

namespace Caramel::Visitors {

class AbstractSyntaxTreeVisitor : public CaramelBaseVisitor {

public:
    explicit AbstractSyntaxTreeVisitor(std::string const& sourceFileName);

    antlrcpp::Any visitR(CaramelParser::RContext *ctx) override;

    antlrcpp::Any visitStatements(CaramelParser::StatementsContext *ctx) override;

    antlrcpp::Any visitValidIdentifier(CaramelParser::ValidIdentifierContext *ctx) override;

    antlrcpp::Any visitTypeParameter(CaramelParser::TypeParameterContext *ctx) override;

    antlrcpp::Any visitVariableDeclaration(CaramelParser::VariableDeclarationContext *ctx) override;

    antlrcpp::Any visitNumberConstant(CaramelParser::NumberConstantContext *ctx) override;

    antlrcpp::Any visitCharConstant(CaramelParser::CharConstantContext *ctx) override;

    antlrcpp::Any visitFunctionDeclaration(CaramelParser::FunctionDeclarationContext *ctx) override;

    antlrcpp::Any visitFunctionArguments(CaramelParser::FunctionArgumentsContext *ctx) override;

    antlrcpp::Any visitFunctionArgument(CaramelParser::FunctionArgumentContext *ctx) override;

    antlrcpp::Any visitArrayDeclarationVoid(CaramelParser::ArrayDeclarationVoidContext *ctx) override ;

    antlrcpp::Any visitIfBlock(CaramelParser::IfBlockContext *ctx) override ;
    
private:
    void pushNewContext();

    Context::Ptr currentContext();

private:
    std::stack<Context::Ptr> mContextStack;
    SourceFileUtil mSourceFileUtil;
};

} // namespace Caramel::Visitors
