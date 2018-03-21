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

#include "SymbolTable.h"
#include "TypeSymbol.h"
#include "VariableSymbol.h"
#include "FunctionSymbol.h"

#include "../exceptions/SymbolAlreadyDeclaredException.h"
#include "../Logger.h"


namespace Caramel::DataStructure {

void SymbolTable::addVariableDeclaration(const PrimaryType::Ptr &primaryType, const std::string &name,
                                         const Declaration::Ptr &declaration) {
    if (isNotDeclared(name)) {
        mSymbolMap[name] = VariableSymbol::Create(name, primaryType);
        mSymbolMap[name]->addDeclaration(declaration);
    } else {
        using namespace Caramel::Exceptions;
        throw SymbolAlreadyDeclaredException("A symbol named '" + name + "' is already declared");
    }
}

void SymbolTable::addVariableDefinition(const PrimaryType::Ptr &primaryType, const std::string &name,
                                        const Definition::Ptr &definition) {
    // Not declared and not defined
    if (isNotDeclared(name)) {
        mSymbolMap[name] = VariableSymbol::Create(name, primaryType);
        mSymbolMap[name]->addDefinition(definition);

        // Declared but not defined
    } else if (isNotDefined(name)) {
        // Todo : check if the definition matches with the declaration
        // Todo : if check is valid, add the definition, else throw an Exception


        // Declared and defined
    } else {
        // Todo : throws SymbolAlreadyDefinedException
    }
}

void SymbolTable::addVariableUsage(const std::string &name, const Expression::Ptr &expression) {

    if (isDefined(name)) {
        mSymbolMap[name]->addUsage(expression);
    } else {
        // Todo : Try to find the variable in the parent context or throw a VariableUndefinedException
    }

}

void SymbolTable::addFunctionDeclaration(const PrimaryType::Ptr &returnType, const std::string &name,
                                         std::vector<Symbol::Ptr> namedParameters,
                                         const Declaration::Ptr &declaration) {

    if (isNotDeclared(name)) {
        mSymbolMap[name] = FunctionSymbol::Create(name, returnType);
        mSymbolMap[name]->addDeclaration(declaration);
    } else {
        using namespace Caramel::Exceptions;
        throw SymbolAlreadyDeclaredException("A symbol named '" + name + "' is already declared");
    }

}

void SymbolTable::addFunctionDefinition(const PrimaryType::Ptr &returnType, const std::string &name,
                                        std::vector<Symbol::Ptr> namedParameters, const Definition::Ptr &definition) {
    // NotDeclared and not defined
    if (isNotDeclared(name)) {
        mSymbolMap[name] = FunctionSymbol::Create(name, returnType);
        mSymbolMap[name]->addDefinition(definition);

        // Declared but not defined
    } else if (isNotDefined(name)) {
        // Todo : check if the definition matches with the declaration
        // Todo : if check is valid, add the definition, else throw an Exception


        // Declared and defined
    } else {

    }

}

void SymbolTable::addFunctionCall(const std::string &name, const std::vector<Symbol::Ptr> &valueParameters,
                                  const Expression::Ptr &expression) {

    if (isDefined(name)) {
        // Todo : maybe checks if the valueParameters may match with the namedParameters of the function
        mSymbolMap[name]->addUsage(expression);
    } else {
        // Todo : Try to find the function in the parent context or throw a FunctionUndefinedException
    }

}

void
SymbolTable::addType(const PrimaryType::Ptr &primaryType, const std::string &name, const Definition::Ptr &definition) {
    // FIXME: Use the right AST type instead of definition, when typedef will be handled.

    // Not declared and not defined
    if (isNotDeclared(name)) {
        mSymbolMap[name] = TypeSymbol::Create(name, primaryType);
        mSymbolMap[name]->addDefinition(definition);

        // Declared but not defined
    } else if (isNotDefined(name)) {
        // Todo : check if the definition matches with the declaration
        // Todo : if check is valid, add the definition, else throw an Exception


        // Declared and defined
    } else {
        // Todo : throws SymbolAlreadyDefinedException
    }
}

bool SymbolTable::hasSymbol(std::string const &name) {
    return mSymbolMap.find(name) != mSymbolMap.end();
}

Symbol::Ptr SymbolTable::getSymbol(std::string const &name) {

    // FIXME: Lookup in parent tables

    auto it{mSymbolMap.find(name)};
    if (it == mSymbolMap.end()) {
        logger.fatal() << "Symbol " << name << " not found in the symbol table.";
        std::terminate();
    }
    return it->second;
}

bool SymbolTable::isDeclared(const std::string &name) {
    return hasSymbol(name) && mSymbolMap[name]->isDeclared();
}

bool SymbolTable::isDefined(const std::string &name) {
    return hasSymbol(name) && mSymbolMap[name]->isDefined();
}

} //namespace Caramel::DataStructure
