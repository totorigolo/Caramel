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

#include "Symbol.h"

#include <utility>


Caramel::DataStructure::Symbol::Symbol(std::string mName,
                                       Caramel::DataStructure::PrimaryType::Ptr mType,
                                       SymbolType symbolType)
        : mIsDeclared{false},
          mIsDefined{false},
          mName{std::move(mName)},
          mType{std::move(mType)},
          mSymbolType{symbolType} {
}

std::vector<Caramel::DataStructure::Statement::Ptr> Caramel::DataStructure::Symbol::getOccurrences() {
    return mOccurrences;
}

bool Caramel::DataStructure::Symbol::isDeclared() {
    return mIsDeclared;
}

bool Caramel::DataStructure::Symbol::isDefined() {
    return mIsDefined;
}

void Caramel::DataStructure::Symbol::addDeclaration(const Caramel::DataStructure::Declaration::Ptr &declaration) {
    onDeclaration(declaration);
    mIsDeclared = true;
    mOccurrences.push_back(declaration);
}

void Caramel::DataStructure::Symbol::addDefinition(const Caramel::DataStructure::Statement::Ptr &definition) {
    onDefinition(definition);
    mIsDeclared = true;
    mIsDefined = true;
    mOccurrences.push_back(definition);
}

void Caramel::DataStructure::Symbol::addUsage(const Caramel::DataStructure::Statement::Ptr &expression) {
    onUsage(expression);
    mOccurrences.push_back(expression);
}

Caramel::DataStructure::SymbolType Caramel::DataStructure::Symbol::getSymbolType() const {
    return mSymbolType;
}

std::string Caramel::DataStructure::Symbol::getName() const {
    return mName;
}

void Caramel::DataStructure::Symbol::onDeclaration(const Caramel::DataStructure::Statement::Ptr &declaration) {}

void Caramel::DataStructure::Symbol::onDefinition(const Caramel::DataStructure::Statement::Ptr &definition) {}

void Caramel::DataStructure::Symbol::onUsage(const Caramel::DataStructure::Statement::Ptr &expression) {}

Caramel::DataStructure::PrimaryType::Ptr Caramel::DataStructure::Symbol::getType() const {
    return mType;
}


