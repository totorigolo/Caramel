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

#include "ArrayDefinition.h"

#include "../../symboltable/ArraySymbol.h"
#include "../expressions/atomicexpression/Constant.h"
#include "../../../utils/Common.h"


namespace caramel::ast {

ArrayDefinition::ArrayDefinition(antlr4::Token *startToken)
        : Definition(startToken, StatementType::ArrayDefinition),
          mStartToken{startToken}, mSymbol{} {}

std::weak_ptr<caramel::ast::Symbol> ArrayDefinition::getSymbol() {
    return mSymbol;
}

void ArrayDefinition::setSymbol(std::shared_ptr<ArraySymbol> const &symbol) {
    mSymbol = symbol;
}

void ArrayDefinition::acceptAstDotVisit() {
    using namespace caramel::utils;
    if (!mSymbol.lock()) {
        addErrorNode(thisId(), "ArrayDefinition", "ArraySymbol is null.");
        return;
    }
    auto arraySymbol = castTo<ArraySymbol::Ptr>(mSymbol.lock());
    addNode(thisId(), "ArraySymbol: " + arraySymbol->getName());
    addEdge(thisId(), arraySymbol->thisId());
}

} // namespace caramel::ast
