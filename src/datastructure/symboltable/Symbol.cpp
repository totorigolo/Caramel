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

caramel::dataStructure::symbolTable::Symbol::Symbol(
        std::string mName,
        std::shared_ptr<caramel::dataStructure::symbolTable::PrimaryType> mType,
        caramel::dataStructure::symbolTable::SymbolType symbolType
) : mDeclaration{}, mDefinition{}, mName{mName}, mType{mType}, mSymbolType{symbolType} {}

std::vector<std::weak_ptr<caramel::dataStructure::statements::Statement>>
caramel::dataStructure::symbolTable::Symbol::getOccurrences() {
    return mOccurrences;
}

bool
caramel::dataStructure::symbolTable::Symbol::isDeclared() {
    return (bool) mDeclaration.use_count() > 0;
}

bool
caramel::dataStructure::symbolTable::Symbol::isDefined() {
    return (bool) mDefinition.use_count() > 0;
}

std::shared_ptr<caramel::dataStructure::statements::declaration::Declaration>
caramel::dataStructure::symbolTable::Symbol::getDeclaration() {
    return mDeclaration.lock();
}

std::shared_ptr<caramel::dataStructure::statements::definition::Definition>
caramel::dataStructure::symbolTable::Symbol::getDefinition() {
    return mDefinition.lock();
}

void
caramel::dataStructure::symbolTable::Symbol::addDeclaration(
        const std::shared_ptr<caramel::dataStructure::statements::declaration::Declaration> &declaration
) {
    onDeclaration(declaration);
    mDeclaration = declaration;
}

void
caramel::dataStructure::symbolTable::Symbol::addDefinition(
        const std::shared_ptr<caramel::dataStructure::statements::definition::Definition> &definition
) {
    if (!isDeclared()) {
        onDeclaration(definition);
        mDeclaration = definition;
    }
    onDefinition(definition);
    mDefinition = definition;
}

void
caramel::dataStructure::symbolTable::Symbol::addUsage(const std::shared_ptr<caramel::dataStructure::statements::Statement> &statement) {
    onUsage(statement);
    mOccurrences.push_back(statement);
}
    
caramel::dataStructure::symbolTable::SymbolType
caramel::dataStructure::symbolTable::Symbol::getSymbolType() const {
    return mSymbolType;
}

std::string caramel::dataStructure::symbolTable::Symbol::getName() const {
    return mName;
}

void caramel::dataStructure::symbolTable::Symbol::onDeclaration(const std::shared_ptr<caramel::dataStructure::statements::declaration::Declaration> &declaration) {}

void caramel::dataStructure::symbolTable::Symbol::onDefinition(const std::shared_ptr<caramel::dataStructure::statements::definition::Definition> &definition) {}

void caramel::dataStructure::symbolTable::Symbol::onUsage(
        const std::shared_ptr<caramel::dataStructure::statements::Statement> &expression) {}

std::shared_ptr<caramel::dataStructure::symbolTable::PrimaryType> caramel::dataStructure::symbolTable::Symbol::getType() const {
    return mType;
}

std::string caramel::dataStructure::symbolTable::Symbol::getSymbolTypeAsString() const {
    switch (mSymbolType) {
        case SymbolType::VariableSymbol:
            return "VariableSymbol";
        case SymbolType::TypeSymbol:
            return "TypeSymbol";
        case SymbolType::FunctionSymbol:
            return "FunctionSymbol";
    }
}
