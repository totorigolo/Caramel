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
#include "../exceptions/DeclarationMismatchException.h"
#include "../exceptions/SymbolAlreadyDefinedException.h"
#include "../exceptions/UndefinedSymbolException.h"


namespace Caramel::DataStructure {

void SymbolTable::addVariableDeclaration(const PrimaryType::Ptr &primaryType, const std::string &name,
                                         const Declaration::Ptr &declaration) {
    using namespace Caramel::Exceptions;
    if (isDefined(name)) {
        throw SymbolAlreadyDefinedException(buildAlreadyDefinedErrorMessage(name));
    } else if (isDeclared(name)) {
        throw SymbolAlreadyDeclaredException(buildAlreadyDeclaredErrorMessage(name));
    } else {
        mSymbolMap[name] = VariableSymbol::Create(name, primaryType);
        mSymbolMap[name]->addDeclaration(declaration);
    }

}

void SymbolTable::addVariableDefinition(const PrimaryType::Ptr &primaryType, const std::string &name,
                                        const Definition::Ptr &definition) {

    using namespace Caramel::Exceptions;
    if (isDefined(name)) {
        throw SymbolAlreadyDefinedException(buildAlreadyDefinedErrorMessage(name));
    } else if (isDeclared(name)) {
        if (mSymbolMap[name]->getSymbolType() != SymbolType::VariableSymbol) {
            throw DeclarationMismatchException(buildMismatchSymbolTypeErrorMessage(name, SymbolType::VariableSymbol));
        }
        if (!mSymbolMap[name]->getType()->equals(primaryType)) {
            throw DeclarationMismatchException(buildMismatchTypeErrorMessage(name, primaryType));
        }
        mSymbolMap[name]->addDefinition(definition);
    } else {
        mSymbolMap[name] = VariableSymbol::Create(name, primaryType);
        mSymbolMap[name]->addDefinition(definition);
    }

}

void SymbolTable::addVariableUsage(const std::string &name, const Expression::Ptr &expression) {

    using namespace Caramel::Exceptions;
    if (isDefined(name)) {
        mSymbolMap[name]->addUsage(expression);
    } else {
        // Fixme : Try to find the variable in the parent context or throw a VariableUndefinedException
        throw UndefinedSymbolException(buildUndefinedSymbolErrorMessage(name, SymbolType::VariableSymbol));
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
                                        std::vector<Symbol::Ptr> namedParameters,
                                        const Definition::Ptr &definition) {
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
SymbolTable::addType(const PrimaryType::Ptr &primaryType, const std::string &name,
                     const Definition::Ptr &definition) {
    // FIXME: Use the right AST type instead of definition, when typedef will be handled.

    // Not declared and not defined
    if (isNotDeclared(name)) {
        mSymbolMap[name] = TypeSymbol::Create(name, primaryType);
        mSymbolMap[name]->addDefinition(definition);
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

std::string SymbolTable::buildAlreadyDefinedErrorMessage(std::string const &variableName) {

    std::stringstream res;
    res << "Cannot use identifier: " << variableName << " because ";
    Symbol::Ptr previousDeclaration = mSymbolMap[variableName];
    switch (previousDeclaration->getSymbolType()) {
        case SymbolType::FunctionSymbol:
            res << "a function with the same name is already defined at line "
                << previousDeclaration->getOccurrences().at(0)->getLine();
            break;
        case SymbolType::VariableSymbol:
            res << "a variable with the same name is already defined at line "
                << previousDeclaration->getOccurrences().at(0)->getLine();
            break;
        case SymbolType::TypeSymbol:
            res << variableName << " is a reserved type identifier";
            break;
    }
    return res.str();

}

std::string SymbolTable::buildAlreadyDeclaredErrorMessage(std::string const &variableName) {

    std::stringstream res;
    res << "Cannot use identifier: " << variableName << " because ";
    Symbol::Ptr previousDeclaration = mSymbolMap[variableName];
    switch (previousDeclaration->getSymbolType()) {
        case SymbolType::FunctionSymbol:
            res << "a function with the same name is already declared at line "
                << previousDeclaration->getOccurrences().at(0)->getLine();
            break;
        case SymbolType::VariableSymbol:
            res << "a variable with the same name is already declared at line "
                << previousDeclaration->getOccurrences().at(0)->getLine();
            break;
        case SymbolType::TypeSymbol:
            res << variableName << " is a reserved type identifier";
            break;
    }
    return res.str();

}

std::string SymbolTable::buildMismatchSymbolTypeErrorMessage(std::string const &variableName,
                                                             SymbolType requiredSymbolType) {
    std::stringstream res;
    res << "Cannot defined a ";
    switch (requiredSymbolType) {
        case SymbolType::VariableSymbol:
            res << "variable";
            break;
        case SymbolType::FunctionSymbol:
            res << "function";
            break;
        case SymbolType::TypeSymbol:
            res << "type";
            break;
    }
    res << " which has a previous occurrence as a " << mSymbolMap[variableName]->getSymbolTypeAsString();
    return res.str();
}

std::string
SymbolTable::buildMismatchTypeErrorMessage(std::string const &variableName, PrimaryType::Ptr const &requiredType) {
    std::stringstream res;
    res << "Mismatch type for " << variableName << " between the type definition " << requiredType->getIdentifier();
    res << " and declaration type " << mSymbolMap[variableName]->getType()->getIdentifier();
    return res.str();
}

std::string SymbolTable::buildUndefinedSymbolErrorMessage(std::string const &name, SymbolType symbolType) {
    std::stringstream res;
    res << "The ";
    switch (symbolType) {
        case SymbolType::VariableSymbol:
            res << "variable";
            break;
        case SymbolType::FunctionSymbol:
            res << "function";
            break;
        case SymbolType::TypeSymbol:
            res << "type";
            break;
    }
    res << " is never defined before";
    return res.str();
}

} //namespace Caramel::DataStructure
