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
#include "TypeSymbol.h"

namespace caramel::ast {

class VariableSymbol : public caramel::ast::Symbol {
public:
    using Ptr = std::shared_ptr<VariableSymbol>;
    using WeakPtr = std::weak_ptr<VariableSymbol>;

    VariableSymbol(
            const std::string &mName,
            const std::shared_ptr<PrimaryType> &mType
    );

    static Ptr Create(
            const std::string &mName,
            const std::shared_ptr<PrimaryType> &mType
    ) {
        return std::make_shared<VariableSymbol>(mName, mType);
    }

    VariableSymbol(
            const std::string &name,
            const std::shared_ptr<TypeSymbol> &aliasType
    );

    static Ptr Create(const std::string &mName, const std::shared_ptr<TypeSymbol> &aliasType) {
        return std::make_shared<VariableSymbol>(mName, aliasType);
    }

public:

    ~VariableSymbol() override = default;

    void onDeclaration(
            const std::shared_ptr<caramel::ast::Declaration> &declaration
    ) override { Symbol::onDeclaration(declaration); };

    void onDefinition(
            const std::shared_ptr<caramel::ast::Definition> &definition
    ) override { Symbol::onDefinition(definition); };

    void onUsage(
            const std::shared_ptr<caramel::ast::Statement> &statement
    ) override { Symbol::onUsage(statement); };



};

} // namespace caramel::dataStructure::symbolTable


