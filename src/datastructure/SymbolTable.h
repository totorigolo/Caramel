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
#include "../exceptions/NotImplementedException.h"
#include "../datastructure/Symbol.h"

#include <CaramelBaseVisitor.h>

#include <map>
#include <string>
#include <memory>


namespace Caramel::DataStructure {

class SymbolTable {
public:
    using Ptr = std::shared_ptr<SymbolTable>;

    static Ptr Create() {
        return Ptr(new SymbolTable);
    }

    void addVariableDeclaration(antlr4::ParserRuleContext *antlrContext,
                                PrimaryType::Ptr const &primaryType,
                                std::string const &name,
                                Declaration::Ptr const &declaration);
    void addVariableDefinition(PrimaryType::Ptr const &primaryType,
                               std::string const &name,
                               Definition::Ptr const &definition);
    void addVariableUsage(std::string const &name,
                          Expression::Ptr const &expression);

    void addFunctionDeclaration(antlr4::ParserRuleContext *antlrContext,
                                PrimaryType::Ptr const &returnType,
                                std::string const &name,
                                std::vector<Symbol::Ptr> namedParameters,
                                Declaration::Ptr const &declaration);
    void addFunctionDefinition(PrimaryType::Ptr const &returnType,
                               std::string const &name,
                               std::vector<Symbol::Ptr> namedParameters,
                               Definition::Ptr const &definition);
    void addFunctionCall(std::string const &name,
                         std::vector<Symbol::Ptr> const &valueParameters,
                         Expression::Ptr const &expression);

    void addType(PrimaryType::Ptr const &primaryType,
                 std::string const &name,
                 Definition::Ptr const &definition);

    bool hasSymbol(std::string const &name);
    Symbol::Ptr getSymbol(std::string const &name);

private:
    std::map<std::string, Symbol::Ptr> mSymbolMap;

private:
    SymbolTable() = default;

    bool isDeclared(std::string const &name);
    bool isDefined(std::string const &name);

    inline bool isNotDeclared(std::string const &name) { return !isDeclared(name); }
    inline bool isNotDefined(std::string const &name) { return !isDefined(name); }
};

} // namespace Caramel::DataStructure
