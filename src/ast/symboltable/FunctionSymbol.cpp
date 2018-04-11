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

#include "FunctionSymbol.h"
#include "../context/Context.h"
#include "../symboltable/PrimaryType.h"


namespace caramel::ast {

FunctionSymbol::FunctionSymbol(const std::string &name, const PrimaryType::Ptr &type, bool variadic)
        : Symbol(name, type, SymbolType::FunctionSymbol), mContext{}, mVariadic{variadic}, mParameters{} {}

std::shared_ptr<Context> FunctionSymbol::getContext() {
    return mContext;
}

void FunctionSymbol::setContext(std::shared_ptr<Context> context) {
    mContext = std::move(context);
}

bool FunctionSymbol::isVariadic() const {
    return mVariadic;
}

std::vector<FunctionParameterSignature> FunctionSymbol::getParameters() const {
    return mParameters;
}

void FunctionSymbol::setParameters(std::vector<Symbol::Ptr> const &parameters) {
    mParameters.clear();
    for (auto const &parameter : parameters) {
        mParameters.emplace_back(parameter->getName(), parameter->getType(), parameter->getSymbolType());
    }
}

void FunctionSymbol::setParameters(std::vector<FunctionParameterSignature> &&parameters) {
    mParameters.clear();
    std::move(parameters.begin(), parameters.end(), std::back_inserter(mParameters));
}

void FunctionSymbol::acceptAstDotVisit() {
    addNode(thisId(), "FunctionSymbol: " + getName() + " as " + getType()->getIdentifier(), "note", "orange");
    visitChildrenAstDot();
}

void FunctionSymbol::visitChildrenAstDot() {
    size_t i = 0;
    for (auto const &[paramName, paramType, paramSymbolType] : mParameters) {
        addEdge(thisId(), thisId() + i + 1, std::to_string(i));
        addNode(thisId() + i + 1, paramName + ": " + paramType->getIdentifier()
                              + (paramSymbolType == SymbolType::ArraySymbol ? "[]" : ""));
        i++;
    }
}

} // namespace caramel::ast
