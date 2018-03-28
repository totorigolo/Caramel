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

#include "PrimaryType.h"
#include "../statements/Statement.h"
#include "../statements/declaration/Declaration.h"
#include "../statements/expressions/Expression.h"
#include "../statements/definition/Definition.h"
#include <iostream>
#include <vector>
#include <memory>


namespace caramel::ast {

enum class SymbolType {
    FunctionSymbol,
    VariableSymbol,
    TypeSymbol,
    ArraySymbol
};

class Symbol {
public:
    using Ptr = std::shared_ptr<Symbol>;
    using WeakPtr = std::weak_ptr<Symbol>;

    virtual ~Symbol() = default;

    std::vector<std::weak_ptr<Statement>> getOccurrences();

    virtual bool isDeclared();

    virtual bool isDefined();

    std::shared_ptr<Declaration> getDeclaration();
    std::shared_ptr<Definition> getDefinition();

    std::shared_ptr<PrimaryType> getType() const;

    void addDeclaration(const std::shared_ptr<Declaration> &declaration);
    void addDefinition(const std::shared_ptr<Definition> &definition);
    void addUsage(const std::shared_ptr<Statement> &statement);

    SymbolType getSymbolType() const;
    std::string getSymbolTypeAsString() const;

    std::string getName() const;

protected:
    Symbol(
            std::string mName,
            std::shared_ptr<PrimaryType> mType,
            SymbolType symbolType
    );

    virtual void onDeclaration(const std::shared_ptr<caramel::ast::Declaration> &declaration);
    virtual void onDefinition(const std::shared_ptr<caramel::ast::Definition> &definition);
    virtual void onUsage(const std::shared_ptr<caramel::ast::Statement> &statement);

protected:
    std::weak_ptr<Declaration> mDeclaration;
    std::weak_ptr<Definition> mDefinition;
    std::vector<std::weak_ptr<Statement>> mOccurrences;

private:
    std::string mName;
    PrimaryType::Ptr mType;
    SymbolType mSymbolType;
};

} // namespace caramel::dataStructure::symbolTable



