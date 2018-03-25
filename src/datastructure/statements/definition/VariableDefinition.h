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

#include "../declaration/Declaration.h"
#include "Definition.h"
#include "../expressions/Expression.h"

namespace caramel::dataStructure::statements::definition {

class VariableDefinition : public caramel::dataStructure::statements::definition::Definition {
public:
    using Ptr = std::shared_ptr<VariableDefinition>;

    VariableDefinition(
            std::shared_ptr<caramel::dataStructure::symbolTable::VariableSymbol> symbol,
            antlr4::Token *startToken
    );

    static Ptr Create(
            std::shared_ptr<caramel::dataStructure::symbolTable::VariableSymbol> symbol,
            antlr4::Token *startToken
    ) {
        // Fixme : replace nullptr par numeric constant = 0
        return std::make_shared<VariableDefinition>(symbol, startToken);
    }

    VariableDefinition(
            std::shared_ptr<caramel::dataStructure::symbolTable::VariableSymbol> symbol,
            std::shared_ptr<caramel::dataStructure::statements::expressions::Expression> initializer,
            antlr4::Token *startToken
    );

    static Ptr Create(
            std::shared_ptr<caramel::dataStructure::symbolTable::VariableSymbol> symbol,
            std::shared_ptr<caramel::dataStructure::statements::expressions::Expression> initializer,
            antlr4::Token *startToken
    ) {
        return std::make_shared<VariableDefinition>(symbol, initializer, startToken);
    }

public:
    std::weak_ptr<caramel::dataStructure::symbolTable::Symbol> getSymbol() override {
        throw std::runtime_error("Cannot return a valid symbol");
    };
    std::weak_ptr<caramel::dataStructure::symbolTable::VariableSymbol> getVariableSymbol();

private:
    std::weak_ptr<caramel::dataStructure::symbolTable::VariableSymbol> mSymbol;
    std::weak_ptr<caramel::dataStructure::statements::expressions::Expression> mInitializer;
};

} // namespace caramel::dataStructure::statements::definition
