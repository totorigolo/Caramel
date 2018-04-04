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

#include "Common.h"
#include "Symbol.h"
#include "TypeSymbol.h"
#include "ArraySymbol.h"
#include "VariableSymbol.h"
#include "FunctionSymbol.h"
#include "../statements/definition/TypeDefinition.h"
#include "../statements/expressions/atomicexpression/FunctionCall.h"
#include "../../exceptions/NotImplementedException.h"

#include <CaramelBaseVisitor.h>

#include <map>
#include <string>
#include <memory>


namespace caramel::ast {

class Context;

class SymbolTable : public AstDotNode {
public:
    using Ptr = std::shared_ptr<SymbolTable>;
    using WeakPtr = std::weak_ptr<SymbolTable>;

    SymbolTable() = default;
    explicit SymbolTable(SymbolTable::Ptr const &parentTable);

    //------------------------------------------------------------------------------------------------------------------
    // Variables

    VariableSymbol::Ptr addVariableDeclaration(
            antlr4::ParserRuleContext *antlrContext,
            std::shared_ptr<PrimaryType> const &primaryType,
            std::string const &name,
            const std::shared_ptr<Declaration> &declaration
    );

    VariableSymbol::Ptr addVariableDefinition(
            antlr4::ParserRuleContext *antlrContext,
            std::shared_ptr<PrimaryType> const &primaryType,
            std::string const &name,
            const std::shared_ptr<Definition> &definition
    );

    VariableSymbol::Ptr addVariableUsage(
            antlr4::ParserRuleContext *antlrContext,
            std::string const &name,
            const std::shared_ptr<Statement> &statement
    );

    //------------------------------------------------------------------------------------------------------------------
    // Arrays

    ArraySymbol::Ptr addArrayDeclaration(
            antlr4::ParserRuleContext *antlrContext,
            std::shared_ptr<PrimaryType> const &primaryType,
            std::string const &name,
            bool sized, size_t size,
            const std::shared_ptr<Declaration> &declaration
    );

    ArraySymbol::Ptr addArrayDefinition(
            antlr4::ParserRuleContext *antlrContext,
            std::shared_ptr<PrimaryType> const &primaryType,
            std::string const &name,
            std::vector<std::shared_ptr<Expression>> &&content,
            const std::shared_ptr<Definition> &definition
    );

    ArraySymbol::Ptr addArrayAccess(
            antlr4::ParserRuleContext *antlrContext,
            std::string const &name,
            const std::shared_ptr<Statement> &statement
    );

    //------------------------------------------------------------------------------------------------------------------
    // Functions

    FunctionSymbol::Ptr addFunctionDeclaration(
            antlr4::ParserRuleContext *antlrContext,
            std::shared_ptr<PrimaryType> const &returnType,
            std::string const &name,
            std::vector<FunctionParameterSignature> parameters,
            const std::shared_ptr<Declaration> &declaration
    );

    FunctionSymbol::Ptr addFunctionDefinition(
            antlr4::ParserRuleContext *antlrContext,
            std::shared_ptr<Context> functionContext,
            std::shared_ptr<PrimaryType> const &returnType,
            std::string const &name,
            std::vector<std::shared_ptr<Symbol>> parameters,
            const std::shared_ptr<Definition> &definition
    );

    Symbol::Ptr addFunctionParameter(
            antlr4::ParserRuleContext *antlrContext,
            std::string const &name,
            std::shared_ptr<PrimaryType> const &primaryType,
            SymbolType parameterType
    );

    FunctionSymbol::Ptr addFunctionCall(
            antlr4::ParserRuleContext *antlrContext,
            std::string const &name,
            FunctionCall::Ptr const &functionCall
    );

    //------------------------------------------------------------------------------------------------------------------
    // Types

    void addPrimaryType(std::shared_ptr<PrimaryType> const &primaryType, std::string const &name);

    TypeSymbol::Ptr addType(
            antlr4::ParserRuleContext *antlrContext,
            std::shared_ptr<TypeDefinition> definition
    );

    //------------------------------------------------------------------------------------------------------------------
    // Misc.

    bool hasSymbol(std::string const &name);
    bool thisHasSymbol(std::string const &name);
    bool parentHasSymbol(std::string const &name);
    std::shared_ptr<Symbol> getSymbol(antlr4::ParserRuleContext *antlrContext, std::string const &name);
    std::shared_ptr<SymbolTable> getParentTable();

    size_t getNumberOfSymbols() const;
    std::map<std::string, Symbol::Ptr> const &getSymbols() const;

    void acceptAstDotVisit() override;
    void visitChildrenAstDot() override;

private:
    bool isDeclared(std::string const &name);
    bool isDefined(std::string const &name);
    inline bool isNotDeclared(std::string const &name) { return !isDeclared(name); }
    inline bool isNotDefined(std::string const &name) { return !isDefined(name); }

private:
    std::map<std::string, Symbol::Ptr> mSymbolMap;
    std::shared_ptr<SymbolTable> mParentTable;
};

} // namespace caramel::dataStructure::symbolTable
