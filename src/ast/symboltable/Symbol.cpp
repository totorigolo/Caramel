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


caramel::ast::Symbol::Symbol(
        std::string mName,
        caramel::ast::PrimaryType::Ptr mType,
        caramel::ast::SymbolType symbolType
) : mDeclaration{}, mDefinition{}, mName{std::move(mName)}, mType{std::move(mType)}, mSymbolType{symbolType} {}

std::vector<std::weak_ptr<caramel::ast::Statement>>
caramel::ast::Symbol::getOccurrences() {
    return mOccurrences;
}

bool
caramel::ast::Symbol::isDeclared() {
    return mDeclaration.use_count() > 0;
}

bool
caramel::ast::Symbol::isDefined() {
    return mDefinition.use_count() > 0;
}

std::shared_ptr<caramel::ast::Declaration>
caramel::ast::Symbol::getDeclaration() {
    return mDeclaration.lock();
}

std::shared_ptr<caramel::ast::Definition>
caramel::ast::Symbol::getDefinition() {
    return mDefinition.lock();
}

void
caramel::ast::Symbol::addDeclaration(const std::shared_ptr<caramel::ast::Declaration> &declaration) {
    onDeclaration(declaration);
    mDeclaration = declaration;
}

void
caramel::ast::Symbol::addDefinition(const std::shared_ptr<caramel::ast::Definition> &definition) {
    if (!isDeclared()) {
        onDeclaration(definition);
        mDeclaration = definition;
    }
    onDefinition(definition);
    mDefinition = definition;
}

void
caramel::ast::Symbol::addUsage(const std::shared_ptr<caramel::ast::Statement> &statement) {
    onUsage(statement);
    mOccurrences.push_back(statement);
}
    
caramel::ast::SymbolType
caramel::ast::Symbol::getSymbolType() const {
    return mSymbolType;
}

std::string caramel::ast::Symbol::getName() const {
    return mName;
}

void caramel::ast::Symbol::acceptAstDotVisit() {
    logger.warning() << "Default accept() for " << thisId() << ", which is a " << getSymbolTypeAsString() << '.';
}

void caramel::ast::Symbol::onDeclaration(caramel_unused const std::shared_ptr<caramel::ast::Declaration> &declaration) {}

void caramel::ast::Symbol::onDefinition(caramel_unused const std::shared_ptr<caramel::ast::Definition> &definition) {}

void caramel::ast::Symbol::onUsage(caramel_unused const std::shared_ptr<caramel::ast::Statement> &expression) {}

std::shared_ptr<caramel::ast::PrimaryType> caramel::ast::Symbol::getType() const {
    return mType;
}

std::string caramel::ast::Symbol::getSymbolTypeAsString() const {
    // TODO: Change to a stream operator
    switch (mSymbolType) {
        case SymbolType::VariableSymbol:
            return "VariableSymbol";
        case SymbolType::TypeSymbol:
            return "TypeSymbol";
        case SymbolType::FunctionSymbol:
            return "FunctionSymbol";
        case SymbolType::ArraySymbol:
            return "ArraySymbol";
    }
}
