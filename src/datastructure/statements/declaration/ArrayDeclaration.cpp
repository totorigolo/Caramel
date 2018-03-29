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

#include "ArrayDeclaration.h"
#include "../../symboltable/ArraySymbol.h"
#include "../../../util/Common.h"


namespace caramel::ast {

ArrayDeclaration::ArrayDeclaration(
        std::shared_ptr<ArraySymbol> symbol,
        antlr4::Token *startToken
) : Declaration(startToken, StatementType::ArrayDeclaration), mSymbol(symbol) {}

std::weak_ptr<ArraySymbol> ArrayDeclaration::getArraySymbol() {
    return mSymbol;
}

void ArrayDeclaration::acceptAstDotVisit() {
    using namespace caramel::util;
    if (!mSymbol.lock()) {
        addErrorNode(thisId(), "ArrayDeclaration", "ArraySymbol is null.");
        return;
    }
    auto arraySymbol = castTo<ArraySymbol::Ptr>(mSymbol.lock());
    addNode(thisId(), "ArrayDeclaration: " + std::to_string(arraySymbol->getSize()));
}

void ArrayDeclaration::visitChildrenAstDot() {

}

} // caramel::ast
