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

#include "VariableSymbol.h"

Caramel::DataStructure::VariableSymbol::VariableSymbol(const std::string &mName,
                                                       const Caramel::DataStructure::PrimaryType::Ptr &mType) : Symbol(
        mName, mType) {}

Caramel::DataStructure::VariableSymbol::~VariableSymbol() = default;

void Caramel::DataStructure::VariableSymbol::addDeclaration(const Caramel::DataStructure::Declaration &declaration) {
    mIsDeclared = true;
}

void Caramel::DataStructure::VariableSymbol::addDefinition(const Caramel::DataStructure::Definition &definition) {
    mIsDeclared = true;
    mIsDefined = true;
}

void Caramel::DataStructure::VariableSymbol::addUsage(const Caramel::DataStructure::Expression &expression) {

}
