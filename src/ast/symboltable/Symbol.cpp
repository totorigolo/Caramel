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
#include "Common.h"

#include <utility>


namespace caramel::ast {

Symbol::Symbol(
        std::string mName,
        PrimaryType::Ptr mType,
        SymbolType symbolType
) : mDeclaration{}, mDefinition{}, mName{std::move(mName)}, mType{std::move(mType)}, mSymbolType{symbolType} {}

std::vector<std::weak_ptr<Statement>>
Symbol::getOccurrences() {
    return mOccurrences;
}

bool
Symbol::isDeclared() {
    return mDeclaration.use_count() > 0;
}

bool
Symbol::isDefined() {
    return mDefinition.use_count() > 0;
}

std::shared_ptr<Declaration>
Symbol::getDeclaration() {
    return mDeclaration.lock();
}

std::shared_ptr<Definition>
Symbol::getDefinition() {
    return mDefinition.lock();
}

void
Symbol::addDeclaration(const std::shared_ptr<Declaration> &declaration) {
    onDeclaration(declaration);
    mDeclaration = declaration;
}

void
Symbol::addDefinition(const std::shared_ptr<Definition> &definition) {
    if (!isDeclared()) {
        onDeclaration(definition);
        mDeclaration = definition;
    }
    onDefinition(definition);
    mDefinition = definition;
}

void
Symbol::addUsage(const std::shared_ptr<Statement> &statement) {
    onUsage(statement);
    mOccurrences.push_back(statement);
}

SymbolType
Symbol::getSymbolType() const {
    return mSymbolType;
}

std::string Symbol::getName() const {
    return mName;
}

void Symbol::acceptAstDotVisit() {
    logger.warning() << "Default accept() for " << thisId() << ", which is a " << mSymbolType << '.';
}

void Symbol::onDeclaration(caramel_unused const std::shared_ptr<Declaration> &declaration) {}

void Symbol::onDefinition(caramel_unused const std::shared_ptr<Definition> &definition) {}

void Symbol::onUsage(caramel_unused const std::shared_ptr<Statement> &expression) {}

std::shared_ptr<PrimaryType> Symbol::getType() const {
    return mType;
}

std::ostream &operator<<(std::ostream &os, const SymbolType &type) {
    switch (type) {
        case SymbolType::FunctionSymbol: return os << "FunctionSymbol";
        case SymbolType::VariableSymbol: return os << "VariableSymbol";
        case SymbolType::TypeSymbol: return os << "TypeSymbol";
        case SymbolType::ArraySymbol: return os << "ArraySymbol";
    }
    return os;
}

} // namespace caramel::ast
