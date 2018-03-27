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

#include "../../Logger.h"
#include "../../exceptions/SymbolAlreadyDeclaredError.h"
#include "../../exceptions/SymbolAlreadyDefinedError.h"
#include "../../exceptions/UndefinedSymbolError.h"
#include "../../exceptions/DeclarationMismatchException.h"
#include "../../exceptions/FunctionDefinitionNumberOfParametersMismatchError.h"
#include "../../exceptions/FunctionDefinitionParameterNameMismatchError.h"
#include "../../exceptions/FunctionDefinitionParameterTypeMismatchError.h"


namespace caramel::ast {

SymbolTable::SymbolTable(SymbolTable::Ptr const &parentTable) : mParentTable(parentTable) {}

VariableSymbol::Ptr
SymbolTable::addVariableDeclaration(
        antlr4::ParserRuleContext *antlrContext,
        std::shared_ptr<caramel::ast::PrimaryType> const &primaryType,
        std::string const &name,
        const std::shared_ptr<Declaration> &declaration
) {
    using namespace caramel::exceptions;
    if (isDefined(name)) {
        throw SymbolAlreadyDefinedError(
                name,
                mSymbolMap[name],
                antlrContext,
                getSymbol(name)->getDefinition(),
                declaration
        );
    } else if (isDeclared(name)) {
        throw SymbolAlreadyDeclaredError(
                name,
                mSymbolMap[name],
                antlrContext,
                getSymbol(name)->getDeclaration(),
                declaration
        );
    } else {
        mSymbolMap[name] = std::make_shared<VariableSymbol>(name, primaryType);
        mSymbolMap[name]->addDeclaration(declaration);
    }
}

VariableSymbol::Ptr
SymbolTable::addVariableDefinition(
        antlr4::ParserRuleContext *antlrContext,
        std::shared_ptr<caramel::ast::PrimaryType> const &primaryType,
        std::string const &name,
        const std::shared_ptr<Definition> &definition
) {
    using namespace caramel::exceptions;
    if (isDefined(name)) {
        throw SymbolAlreadyDefinedError(
                name,
                mSymbolMap[name],
                antlrContext,
                getSymbol(name)->getDefinition(),
                definition
        );
    } else if (isDeclared(name)) {
        Symbol::Ptr recordedSymbol = mSymbolMap[name];
        if (recordedSymbol->getSymbolType() != SymbolType::VariableSymbol) {
            throw DeclarationMismatchException(
                    buildMismatchSymbolTypeErrorMessage(name, SymbolType::VariableSymbol)
            );
        }
        if (!recordedSymbol->getType()->equals(primaryType)) {
            throw DeclarationMismatchException(
                    buildMismatchTypeErrorMessage(name, primaryType)
            );
        }
        recordedSymbol->addDefinition(definition);
        return std::dynamic_pointer_cast<VariableSymbol>(recordedSymbol);
    } else {
        VariableSymbol::Ptr variableSymbol = std::make_shared<VariableSymbol>(name, primaryType);
        mSymbolMap[name] = variableSymbol;
        variableSymbol->addDefinition(definition);
        return variableSymbol;
    }
}

void
SymbolTable::addVariableUsage(
        antlr4::ParserRuleContext *antlrContext,
        std::string const &name,
        const std::shared_ptr<Statement> &statement
) {

    using namespace caramel::exceptions;
    if (isDefined(name)) {
        mSymbolMap[name]->addUsage(statement);
    } else {
        // Fixme : Try to find the variable in the parent context or throw a VariableUndefinedException
        SymbolTable::Ptr parent = getParentTable();
        while (nullptr != parent && parent->isNotDeclared(name)) {
            parent = parent->getParentTable();
        }
        if (nullptr == parent) {
            throw UndefinedSymbolError(
                    buildUndefinedSymbolErrorMessage(name, SymbolType::VariableSymbol),
                    antlrContext
            );
        }
    }
}

FunctionSymbol::Ptr
SymbolTable::addFunctionDeclaration(
        antlr4::ParserRuleContext *antlrContext,
        std::shared_ptr<caramel::ast::PrimaryType> const &returnType,
        std::string const &name,
        std::vector<std::shared_ptr<caramel::ast::Symbol>> namedParameters,
        const std::shared_ptr<Declaration> &declaration
) {
    if (isNotDeclared(name)) {
        FunctionSymbol::Ptr functionSymbol = std::make_shared<FunctionSymbol>(name, returnType);
        mSymbolMap[name] = functionSymbol;
        functionSymbol->addDeclaration(declaration);
        functionSymbol->setParameters(namedParameters);
        return functionSymbol;
    } else {
        using namespace caramel::exceptions;
        throw SymbolAlreadyDeclaredError(
                name,
                mSymbolMap[name],
                antlrContext,
                getSymbol(name)->getDeclaration(),
                declaration
        );
    }
}

FunctionSymbol::Ptr
SymbolTable::addFunctionDefinition(
        antlr4::ParserRuleContext *antlrContext,
        std::shared_ptr<caramel::ast::PrimaryType> const &returnType,
        std::string const &name,
        std::vector<std::shared_ptr<caramel::ast::Symbol>> namedParameters,
        const std::shared_ptr<Definition> &definition
) {
    using namespace caramel::exceptions;
    if (isDefined(name)) {
        throw SymbolAlreadyDefinedError(
                name,
                mSymbolMap[name],
                antlrContext,
                getSymbol(name)->getDefinition(),
                definition
        );
    } else if (!isDeclared(name)) {
        FunctionSymbol::Ptr functionSymbol = std::make_shared<FunctionSymbol>(name, returnType);
        functionSymbol->setParameters(namedParameters);
        mSymbolMap[name] = functionSymbol;
    } else {
        FunctionSymbol::Ptr declaredSymbol = std::dynamic_pointer_cast<FunctionSymbol>(mSymbolMap[name]);

        std::vector<std::shared_ptr<caramel::ast::Symbol>> declaredParameters = declaredSymbol->getNamedParameters();
        if (declaredParameters.size() == namedParameters.size()){
            for (int i = 0 ; i < declaredSymbol->getNamedParameters().size(); i++ ){
                std::string declaredParameterName = declaredParameters.at(i)->getName();
                std::string const declaredParameterTypeIdentifier = declaredParameters.at(i)->getType()->getIdentifier();

                std::string parameterName = namedParameters.at(i)->getName();
                std::string const parameterTypeIdentifier = namedParameters.at(i)->getType()->getIdentifier();

                if(declaredParameterName != parameterName){
                    throw FunctionDefinitionParameterNameMismatchError(
                            buildFunctionDefinitionParameterNameMismatchErrorMessage(name,declaredParameterName,parameterName),
                            antlrContext,
                            declaredParameterName,
                            parameterName
                    );
                }
                if(declaredParameterTypeIdentifier != parameterTypeIdentifier){
                    throw FunctionDefinitionParameterTypeMismatchError(
                            buildFunctionDefinitionParameterTypeMismatchErrorMessage(name,declaredParameters.at(i)->getType(),namedParameters.at(i)->getType()),
                            antlrContext,
                            declaredParameters.at(i)->getType(),
                            namedParameters.at(i)->getType()
                    );
                }
            }
        } else {
            throw FunctionDefinitionNumberOfParametersMismatchError(
                    buildFunctionDefinitionNumberOfParametersMismatchErrorMessage(name, declaredParameters.size(),
                                                                                  namedParameters.size()),
                    antlrContext,
                    declaredParameters.size(),
                    namedParameters.size()
            );
        }
    }
    FunctionSymbol::Ptr symbol = std::dynamic_pointer_cast<FunctionSymbol>(mSymbolMap[name]);
    symbol->addDefinition(definition);
    return symbol;
}

void
SymbolTable::addFunctionCall(
        antlr4::ParserRuleContext *antlrContext,
        std::string const &name,
        std::vector<std::shared_ptr<caramel::ast::Symbol>> const &valueParameters,
        const std::shared_ptr<Statement> &statement
) {

    if (isDefined(name)) {
        // Todo : maybe checks if the valueParameters may match with the namedParameters of the function
        mSymbolMap[name]->addUsage(statement);
    } else {
        // Todo : Try to find the function in the parent context or throw a FunctionUndefinedException
    }

}

void
SymbolTable::addPrimaryType(
        std::shared_ptr<caramel::ast::PrimaryType> const &primaryType,
        std::string const &name
) {
    // Not declared and not defined
    if (isNotDeclared(name)) {
        mSymbolMap[name] = std::make_shared<TypeSymbol>(name, primaryType);
        mSymbolMap[name]->addDefinition(nullptr); // TODO: Is it safe?
    } else {
        // Todo : throws SymbolAlreadyDefinedError
    }
}

TypeSymbol::Ptr
SymbolTable::addType(
        antlr4::ParserRuleContext *antlrContext,
        std::shared_ptr<caramel::ast::PrimaryType> const &primaryType,
        std::string const &name,
        std::weak_ptr<Definition> const &definition
) {
    // FIXME: Use the right AST type instead of definition, when typedef will be handled.

    // Not declared and not defined
    if (isNotDeclared(name)) {
        TypeSymbol::Ptr typeSymbol = std::make_shared<TypeSymbol>(name, primaryType);
        mSymbolMap[name] = typeSymbol;
        typeSymbol->addDefinition(definition.lock());
        return typeSymbol;
    } else {
        // Todo : throws SymbolAlreadyDefinedError
    }
}

bool
SymbolTable::hasSymbol(std::string const &name) {
    return mSymbolMap.find(name) != mSymbolMap.end();
}

std::shared_ptr<Symbol>
SymbolTable::getSymbol(std::string const &name) {
    // FIXME: Lookup in parent tables
    auto it{mSymbolMap.find(name)};
    if (it == mSymbolMap.end()) {
        logger.fatal() << "Symbol " << name << " not found in the symbol table.";
        std::terminate();
    }
    return it->second;
}

bool
SymbolTable::isDeclared(const std::string &name) {
    return hasSymbol(name) && mSymbolMap[name]->isDeclared();
}

bool
SymbolTable::isDefined(const std::string &name) {
    return hasSymbol(name) && mSymbolMap[name]->isDefined();
}


std::string
SymbolTable::buildMismatchSymbolTypeErrorMessage(std::string const &variableName,
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


std::string
SymbolTable::buildUndefinedSymbolErrorMessage(std::string const &name, SymbolType symbolType) {
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
        case SymbolType::ArraySymbol:
            res << "array";
            break;
    }
    res << " '" << name << "' is not defined before";
    return res.str();
}

std::shared_ptr<SymbolTable>
SymbolTable::getParentTable() {
    return mParentTable;
}

    std::string
    SymbolTable::buildFunctionDefinitionNumberOfParametersMismatchErrorMessage(const std::string &name, unsigned long declaredSize,
                                                                                   unsigned long definedSize) {
        std::stringstream res;
        res << "The function: ";
        res << name;
        res << " was previously declared with ";
        res << declaredSize;
        res << " parameter(s).\n";
        res << "Actual definition has ";
        res << definedSize;
        res << " parameter(s).\n";
        return res.str();
    }

    std::string SymbolTable::buildFunctionDefinitionParameterNameMismatchErrorMessage(const std::string &name,
                                                                                      std::string declaredName,
                                                                                      std::string definedName) {
        std::stringstream res;
        res << "The function: ";
        res << name;
        res << " was previously declared with a parameter called ";
        res << declaredName;
        res << "  .\n";
        res << "Actual parameter name is ";
        res << definedName;
        res << " .\n";
        return res.str();
    }

    std::string SymbolTable::buildFunctionDefinitionParameterTypeMismatchErrorMessage(const std::string &basic_string,
                                                                               std::shared_ptr<PrimaryType> declaredType,
                                                                               std::shared_ptr<PrimaryType> declaredName) {
        std::stringstream res;
        res << "The function: ";
        res << declaredType->getIdentifier();
        res << " was previously declared with a parameter of type ";
        res << declaredName;
        res << " .\n";
        res << "Actual parameter type is ";
        res << declaredName->getIdentifier();
        res << " .\n";
        return res.str();

    }

} // namespace caramel::ast
