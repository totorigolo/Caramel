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

#include "Symbol.h"
#include "VariableSymbol.h"

namespace caramel::dataStructure::symbolTable {

class ArraySymbol : public VariableSymbol {
public:
    using Ptr = std::shared_ptr<ArraySymbol>;
    using WeakPtr = std::weak_ptr<ArraySymbol>;

    static Ptr Create(const std::string &mName, const PrimaryType::Ptr &mType, const long &mSize) {
        return Ptr(new ArraySymbol(mName, mType, mSize));
    }

    static Ptr Create(const std::string &mName, const TypeSymbol::Ptr &type, const long &mSize) {
        assert(type->getSymbolType() == SymbolType::TypeSymbol);
        return Ptr(new ArraySymbol(mName, type->getType(),mSize));
    }

public:
    ArraySymbol(const std::string &name, const PrimaryType::Ptr &primaryType);
    ArraySymbol(const std::string &name, const TypeSymbol::Ptr &aliasType);
    ArraySymbol(const std::string &mName, const PrimaryType::Ptr &mType, const long &mSize);
    ArraySymbol(const std::string &mName, const TypeSymbol::Ptr &mType, const long &mSize);

    virtual ~ArraySymbol() override = default;

    void onDeclaration(const caramel::dataStructure::statements::declaration::Declaration::Ptr &declaration) override {};
    void onDefinition(const caramel::dataStructure::statements::definition::Definition::Ptr &definition) override {};
    void onUsage(const caramel::dataStructure::statements::Statement::Ptr &statement) override {};

    void setSize(const long &size);
    long getSize();

private:
    long mSize;
};

} // namespace Caramel::DataStructure

