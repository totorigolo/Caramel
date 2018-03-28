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
#include "VariableSymbol.h"
#include "FunctionSymbol.h"
#include "../../exceptions/NotImplementedException.h"
#include "../statements/definition/TypeDefinition.h"

#include <CaramelBaseVisitor.h>

#include <map>
#include <string>
#include <memory>


namespace caramel::ast {

class SymbolTable {
public:
    using Ptr = std::shared_ptr<SymbolTable>;
    using WeakPtr = std::weak_ptr<SymbolTable>;

    static std::shared_ptr<SymbolTable> Create() {
        return std::make_shared<SymbolTable>();
    }

    SymbolTable() = default;

    explicit SymbolTable(SymbolTable::Ptr const &parentTable);

    VariableSymbol::Ptr addVariableDeclaration(
            antlr4::ParserRuleContext *antlrContext,
            std::shared_ptr<caramel::ast::PrimaryType> const &primaryType,
            std::string const &name,
            const std::shared_ptr<Declaration> &declaration
    );

    VariableSymbol::Ptr addVariableDefinition(
            antlr4::ParserRuleContext *antlrContext,
            std::shared_ptr<PrimaryType> const &primaryType,
            std::string const &name,
            const std::shared_ptr<Definition> &definition
    );

    Symbol::Ptr addVariableUsage(
            antlr4::ParserRuleContext *antlrContext,
            std::string const &name,
            const std::shared_ptr<Statement> &expression
    );

    FunctionSymbol::Ptr addFunctionDeclaration(
            antlr4::ParserRuleContext *antlrContext,
            std::shared_ptr<caramel::ast::PrimaryType> const &returnType,
            std::string const &name,
            std::vector<std::shared_ptr<caramel::ast::Symbol>> namedParameters,
            const std::shared_ptr<Declaration> &declaration
    );

    FunctionSymbol::Ptr addFunctionDefinition(
            antlr4::ParserRuleContext *antlrContext,
            std::shared_ptr<caramel::ast::PrimaryType> const &returnType,
            std::string const &name,
            std::vector<std::shared_ptr<caramel::ast::Symbol>> namedParameters,
            const std::shared_ptr<Definition> &definition
    );

    void addFunctionCall(
            antlr4::ParserRuleContext *antlrContext,
            std::string const &name,
            std::vector<std::shared_ptr<caramel::ast::Symbol>> const &valueParameters,
            const std::shared_ptr<Statement> &statement
    );

    void addPrimaryType(
            std::shared_ptr<caramel::ast::PrimaryType> const &primaryType,
            std::string const &name
    );

    TypeSymbol::Ptr addType(
            antlr4::ParserRuleContext *antlrContext,
            std::shared_ptr<TypeDefinition> definition
    );

    bool hasSymbol(std::string const &name);

    std::shared_ptr<Symbol> getSymbol(std::string const &name);

    std::shared_ptr<SymbolTable> getParentTable();

private:
    std::map<std::string, Symbol::Ptr> mSymbolMap;

private:
    std::shared_ptr<SymbolTable> mParentTable;

    bool isDeclared(std::string const &name);

    bool isDefined(std::string const &name);

    inline bool isNotDeclared(std::string const &name) { return !isDeclared(name); }

    inline bool isNotDefined(std::string const &name) { return !isDefined(name); }

    std::string buildAlreadyDefinedErrorMessage(std::string const &variableName);
    std::string buildAlreadyDeclaredErrorMessage(std::string const &variableName);
    std::string buildMismatchSymbolTypeErrorMessage(std::string const &variableName, SymbolType requiredSymbolType);
    std::string buildMismatchTypeErrorMessage(std::string const &variableName, std::shared_ptr<PrimaryType> const &requiredType);
    std::string buildUndefinedSymbolErrorMessage(std::string const &name, SymbolType symbolType);
    std::string buildFunctionDefinitionNumberOfParametersMismatchErrorMessage(const std::string &name, unsigned long declaredSize,
                                                                                  unsigned long definedSize);
    std::string buildFunctionDefinitionParameterNameMismatchErrorMessage(const std::string &name, std::string declaredName,
                                                                         std::string definedName);

    std::string buildFunctionDefinitionParameterTypeMismatchErrorMessage(const std::string &basic_string,
                                                                         std::shared_ptr<PrimaryType> shared_ptr,
                                                                         std::shared_ptr<PrimaryType> sharedPtr);
};

} // namespace caramel::dataStructure::symbolTable
