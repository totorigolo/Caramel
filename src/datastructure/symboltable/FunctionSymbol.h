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
#include <vector>


namespace caramel::dataStructure::symbolTable {

class FunctionSymbol : public Symbol {
public:
    using Ptr = std::shared_ptr<FunctionSymbol>;
    using WeakPtr = std::weak_ptr<FunctionSymbol>;

    static std::shared_ptr<FunctionSymbol> Create(const std::string &mName, const PrimaryType::Ptr &mType) {
        return std::make_shared<FunctionSymbol>(mName, mType);
    }

public:
    FunctionSymbol(
            const std::string &mName,
            const std::shared_ptr<caramel::dataStructure::symbolTable::PrimaryType> &mType
    );

    ~FunctionSymbol() override = default;

    void onDeclaration(
            const std::shared_ptr<caramel::dataStructure::statements::declaration::Declaration> &declaration
    ) override { Symbol::onDeclaration(declaration); };

    void onDefinition(
            const std::shared_ptr<caramel::dataStructure::statements::definition::Definition> &definition
    ) override { Symbol::onDefinition(definition); };

    void
    onUsage(
            const std::shared_ptr<caramel::dataStructure::statements::Statement> &statement
    ) override { Symbol::onUsage(statement); };

    std::vector<std::shared_ptr<caramel::dataStructure::symbolTable::Symbol>> getNamedParameters() const;

    void setParameters(
            const std::vector<std::shared_ptr<caramel::dataStructure::symbolTable::Symbol>> &namedParameters
    );

private:
    std::vector<std::shared_ptr<caramel::dataStructure::symbolTable::Symbol>> mParameters;
};

} // namespace caramel::dataStructure::symbolTable


