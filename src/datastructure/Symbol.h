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
#include "Statement.h"
#include "Declaration.h"
#include "Expression.h"
#include "Definition.h"
#include <iostream>
#include <vector>
#include <memory>


namespace Caramel::DataStructure {

enum class SymbolType {
    FunctionSymbol,
    VariableSymbol,
    TypeSymbol
};

class Symbol {
public:
    using Ptr = std::shared_ptr<Symbol>;
    using WeakPtr = std::weak_ptr<Symbol>;

    virtual ~Symbol() = default;

    std::vector<Statement::Ptr> getOccurrences();

    bool isDeclared();
    bool isDefined();

    PrimaryType::Ptr getType() const;

    void addDeclaration(const Declaration::Ptr &declaration);
    void addDefinition(const Definition::Ptr &definition);
    void addUsage(const Expression::Ptr &expression);

    SymbolType getSymbolType() const;

    std::string getName() const;

protected:
    Symbol(std::string mName, PrimaryType::Ptr mType, SymbolType symbolType);

    virtual void onDeclaration(const Declaration::Ptr &declaration);
    virtual void onDefinition(const Definition::Ptr &definition);
    virtual void onUsage(const Expression::Ptr &expression);

protected:
    bool mIsDeclared;
    bool mIsDefined;
    std::vector<Statement::Ptr> mOccurrences;

private:
    std::string mName;
    PrimaryType::Ptr mType;
    SymbolType mSymbolType;
};

} // namespace Caramel::DataStructure



