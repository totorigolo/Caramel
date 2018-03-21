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

std::vector<Caramel::DataStructure::Statement::Ptr> Caramel::DataStructure::Symbol::getOccurences() {
    return mOccurences;
}

Caramel::DataStructure::Symbol::Symbol(const std::string &mName,
                                       const Caramel::DataStructure::PrimaryType::Ptr &mType)
        : mIsDeclared(false), mIsDefined(false), mName(mName), mType(std::move(mType)), mOccurences() {}

Caramel::DataStructure::Symbol::~Symbol() = default;

bool Caramel::DataStructure::Symbol::isDeclared() {
    return mIsDeclared;
}

bool Caramel::DataStructure::Symbol::isDefined() {
    return mIsDefined;
}

void Caramel::DataStructure::Symbol::addDeclaration(const Caramel::DataStructure::Declaration::Ptr &declaration) {
    onDeclaration(declaration);
    mIsDeclared = true;
    mOccurences.push_back(declaration);
}

void Caramel::DataStructure::Symbol::addDefinition(const Caramel::DataStructure::Statement::Ptr &definition) {
    onDefinition(definition);
    mIsDeclared = true;
    mIsDefined = true;
    mOccurences.push_back(definition);
}

void Caramel::DataStructure::Symbol::addUsage(const Caramel::DataStructure::Statement::Ptr &expression) {
    onUsage(expression);
    mOccurences.push_back(expression);
}

void Caramel::DataStructure::Symbol::onDeclaration(const Caramel::DataStructure::Statement::Ptr &declaration) {}

void Caramel::DataStructure::Symbol::onDefinition(const Caramel::DataStructure::Statement::Ptr &definition) {}

void Caramel::DataStructure::Symbol::onUsage(const Caramel::DataStructure::Statement::Ptr &expression) {}

Caramel::DataStructure::PrimaryType::Ptr Caramel::DataStructure::Symbol::getType() const {
    return mType;
}


