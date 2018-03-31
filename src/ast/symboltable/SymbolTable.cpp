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
#include "../../utils/Common.h"
#include "../context/Context.h"
#include "../statements/definition/TypeDefinition.h"
#include "../../exceptions/UndefinedSymbolError.h"
#include "../../exceptions/SymbolAlreadyDefinedError.h"
#include "../../exceptions/SymbolAlreadyDeclaredError.h"
#include "../../exceptions/DeclarationMismatchException.h"
#include "../../exceptions/FunctionCallArgumentsTypeMismatchException.h"
#include "../../exceptions/FunctionDefinitionParameterNameMismatchError.h"
#include "../../exceptions/FunctionDefinitionParameterTypeMismatchError.h"
#include "../../exceptions/FunctionCallArgumentsNumberMismatchException.h"
#include "../../exceptions/FunctionDefinitionNumberOfParametersMismatchError.h"


namespace caramel::ast {

using namespace utils;
using namespace exceptions;

SymbolTable::SymbolTable(SymbolTable::Ptr const &parentTable) : mParentTable(parentTable) {}

VariableSymbol::Ptr SymbolTable::addVariableDeclaration(
        antlr4::ParserRuleContext *antlrContext,
        PrimaryType::Ptr const &primaryType,
        std::string const &name,
        const Declaration::Ptr &declaration
) {
    logger.trace() << "SymbolTable::addVariableDeclaration(" << name << ", " << primaryType->getIdentifier() << ")";

    if (isDefined(name)) {
        auto const &symbol = getSymbol(antlrContext, name);
        throw SymbolAlreadyDefinedError(
                name,
                symbol,
                antlrContext,
                symbol->getDefinition(),
                declaration
        );
    } else if (isDeclared(name)) {
        auto const &symbol = getSymbol(antlrContext, name);
        throw SymbolAlreadyDeclaredError(
                name,
                symbol,
                antlrContext,
                symbol->getDeclaration(),
                declaration
        );
    } else {
        VariableSymbol::Ptr variableSymbol = std::make_shared<VariableSymbol>(name, primaryType);
        mSymbolMap[name] = variableSymbol;
        variableSymbol->addDeclaration(declaration);
        return variableSymbol;
    }
}

VariableSymbol::Ptr SymbolTable::addVariableDefinition(
        antlr4::ParserRuleContext *antlrContext,
        PrimaryType::Ptr const &primaryType,
        std::string const &name,
        const Definition::Ptr &definition
) {
    logger.trace() << "SymbolTable::addVariableDefinition(" << name << ", " << primaryType->getIdentifier() << ")";

    if (isDefined(name)) {
        auto const &symbol = getSymbol(antlrContext, name);
        throw SymbolAlreadyDefinedError(
                name,
                symbol,
                antlrContext,
                symbol->getDefinition(),
                definition
        );
    } else if (isDeclared(name)) {
        Symbol::Ptr recordedSymbol = getSymbol(antlrContext, name);
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

VariableSymbol::Ptr SymbolTable::addVariableUsage(
        antlr4::ParserRuleContext *antlrContext,
        std::string const &name,
        const Statement::Ptr &statement
) {
    logger.trace() << "SymbolTable::addVariableUsage(" << name << ")";

    if (isDefined(name)) {
        auto const &symbol = getSymbol(antlrContext, name);
        if (symbol->getSymbolType() != SymbolType::VariableSymbol) {
            // TODO: Create the right error
            throw DeclarationMismatchException(
                    buildMismatchSymbolTypeErrorMessage(name, SymbolType::VariableSymbol)
            );
        }
        auto const &variableSymbol = castTo<VariableSymbol::Ptr>(symbol);
        variableSymbol->addUsage(statement);
        return variableSymbol;
    } else {
        SymbolTable::Ptr parent = getParentTable();
        if (parent) {
            return parent->addVariableUsage(antlrContext, name, statement);
        } else {
            throw UndefinedSymbolError(
                    name,
                    SymbolType::VariableSymbol,
                    antlrContext
            );
        }
    }
}

ArraySymbol::Ptr SymbolTable::addArrayDeclaration(
        antlr4::ParserRuleContext *antlrContext,
        PrimaryType::Ptr const &primaryType,
        std::string const &name,
        bool sized, size_t size,
        const Declaration::Ptr &declaration
) {
    logger.trace() << "SymbolTable::addArrayDeclaration(" << name << ", " << primaryType->getIdentifier() << ")";

    if (isDefined(name)) {
        auto const &symbol = getSymbol(antlrContext, name);
        throw SymbolAlreadyDefinedError(
                name,
                symbol,
                antlrContext,
                symbol->getDefinition(),
                declaration
        );
    } else if (isDeclared(name)) {
        auto const &symbol = getSymbol(antlrContext, name);
        throw SymbolAlreadyDeclaredError(
                name,
                symbol,
                antlrContext,
                symbol->getDeclaration(),
                declaration
        );
    } else {
        ArraySymbol::Ptr arraySymbol;
        if (sized) {
            arraySymbol = std::make_shared<ArraySymbol>(name, primaryType, size);
        } else {
            arraySymbol = std::make_shared<ArraySymbol>(name, primaryType);
        }
        mSymbolMap[name] = arraySymbol;
        arraySymbol->addDeclaration(declaration);
        return arraySymbol;
    }
}

ArraySymbol::Ptr SymbolTable::addArrayDefinition(
        antlr4::ParserRuleContext *antlrContext,
        PrimaryType::Ptr const &primaryType,
        std::string const &name,
        std::vector<Expression::Ptr> &&content,
        const Definition::Ptr &definition
) {
    logger.trace() << "SymbolTable::addArrayDefinition(" << name << ", " << primaryType->getIdentifier() << ")";

    if (isDefined(name)) {
        auto const &symbol = getSymbol(antlrContext, name);
        throw SymbolAlreadyDefinedError(
                name,
                symbol,
                antlrContext,
                symbol->getDefinition(),
                definition
        );
    } else if (isDeclared(name)) {
        Symbol::Ptr recordedSymbol = getSymbol(antlrContext, name);
        if (recordedSymbol->getSymbolType() != SymbolType::ArraySymbol) {
            throw DeclarationMismatchException(
                    buildMismatchSymbolTypeErrorMessage(name, SymbolType::ArraySymbol)
            );
        }
        if (!recordedSymbol->getType()->equals(primaryType)) {
            throw DeclarationMismatchException(
                    buildMismatchTypeErrorMessage(name, primaryType)
            );
        }
        recordedSymbol->addDefinition(definition);
        return castTo<ArraySymbol::Ptr>(recordedSymbol);
    } else {
        ArraySymbol::Ptr arraySymbol = std::make_shared<ArraySymbol>(name, primaryType, std::move(content));
        mSymbolMap[name] = arraySymbol;
        arraySymbol->addDefinition(definition);
        return arraySymbol;
    }
}

ArraySymbol::Ptr SymbolTable::addArrayAccess(
        antlr4::ParserRuleContext *antlrContext,
        std::string const &name,
        const Statement::Ptr &statement
) {
    logger.trace() << "SymbolTable::addArrayAccess(" << name << ")";

    if (isDefined(name)) {
        auto const &symbol = getSymbol(antlrContext, name);
        if (symbol->getSymbolType() != SymbolType::ArraySymbol) {
            // TODO: Create the right error
            throw DeclarationMismatchException(
                    buildMismatchSymbolTypeErrorMessage(name, SymbolType::ArraySymbol)
            );
        }
        auto const &arraySymbol = castTo<ArraySymbol::Ptr>(symbol);
        arraySymbol->addUsage(statement);
        return arraySymbol;
    } else {
        SymbolTable::Ptr parent = getParentTable();
        if (parent) {
            return parent->addArrayAccess(antlrContext, name, statement);
        } else {
            throw UndefinedSymbolError(
                    name,
                    SymbolType::ArraySymbol,
                    antlrContext
            );
        }
    }
}

FunctionSymbol::Ptr SymbolTable::addFunctionDeclaration(
        antlr4::ParserRuleContext *antlrContext,
        PrimaryType::Ptr const &returnType,
        std::string const &name,
        std::vector<std::tuple<std::string, PrimaryType::Ptr, SymbolType>> parameters,
        const Declaration::Ptr &declaration
) {
    logger.trace() << "SymbolTable::addFunctionDeclaration(" << name << ", " << returnType->getIdentifier() << ")";

    if (isDeclared(name)) { // or defined

        // TODO: Duplicated code in addFunctionDefinition() -> move to a helper function

        auto const &symbol = getSymbol(antlrContext, name);
        if (symbol->getSymbolType() != SymbolType::FunctionSymbol) {
            // TODO: Create true error
            throw std::runtime_error(
                    name + " previously declared as a XXX, so it's impossible to redeclare it as a function.");
        }
        FunctionSymbol::Ptr const &functionSymbol = castTo<FunctionSymbol::Ptr>(symbol);

        auto declaredParameters = functionSymbol->getParameters();
        if (declaredParameters.size() == parameters.size()) {
            for (size_t i = 0; i < declaredParameters.size(); i++) {
                std::string const declaredParameterName = std::get<0>(declaredParameters[i]);
                std::string const declaredParameterTypeIdentifier =
                        std::get<1>(declaredParameters[i])->getIdentifier();

                std::string const parameterName = std::get<0>(parameters[i]);
                std::string const parameterTypeIdentifier = std::get<1>(parameters[i])->getIdentifier();

                if (declaredParameterName != parameterName) {
                    throw FunctionDefinitionParameterNameMismatchError(
                            buildFunctionDefinitionParameterNameMismatchErrorMessage(
                                    name, declaredParameterName, parameterName),
                            antlrContext,
                            declaredParameterName,
                            parameterName
                    );
                }
                if (declaredParameterTypeIdentifier != parameterTypeIdentifier) {
                    throw FunctionDefinitionParameterTypeMismatchError(
                            antlrContext,
                            std::get<1>(declaredParameters[i]),
                            std::get<1>(parameters[i])
                    );
                }
            }
        } else {
            throw FunctionDefinitionNumberOfParametersMismatchError(
                    buildFunctionDefinitionNumberOfParametersMismatchErrorMessage(name, declaredParameters.size(),
                                                                                  parameters.size()),
                    antlrContext,
                    declaredParameters.size(),
                    parameters.size()
            );
        }

        functionSymbol->addDeclaration(declaration);
        return functionSymbol;
    } else {
        FunctionSymbol::Ptr functionSymbol = std::make_shared<FunctionSymbol>(name, returnType);
        mSymbolMap[name] = functionSymbol;
        functionSymbol->setParameters(std::move(parameters));
        functionSymbol->addDeclaration(declaration);
        return functionSymbol;
    }
}

FunctionSymbol::Ptr SymbolTable::addFunctionDefinition(
        antlr4::ParserRuleContext *antlrContext,
        Context::Ptr functionContext,
        PrimaryType::Ptr const &returnType,
        std::string const &name,
        std::vector<Symbol::Ptr> parameters,
        const Definition::Ptr &definition
) {
    logger.trace() << "SymbolTable::addFunctionDefinition(" << name << ", " << returnType->getIdentifier() << ")";

    if (isDefined(name)) {
        auto const &symbol = getSymbol(antlrContext, name);
        throw SymbolAlreadyDefinedError(
                name,
                symbol,
                antlrContext,
                symbol->getDefinition(),
                definition
        );
    } else if (isDeclared(name)) {
        auto const &symbol = getSymbol(antlrContext, name);
        if (symbol->getSymbolType() != SymbolType::FunctionSymbol) {
            // TODO: Create true error
            throw std::runtime_error(
                    name + " previously declared as a XXX, so it's impossible to redeclare it as a function.");
        }
        FunctionSymbol::Ptr const &functionSymbol = castTo<FunctionSymbol::Ptr>(symbol);

        if (functionSymbol->getContext()) {
            logger.fatal() << "The function is only declared, but the function context isn't null:\n"
                           << "  - existing declaration: " << *functionSymbol->getContext() << '\n'
                           << "  - current definition: " << *functionContext;
            exit(1);
        }

        auto declaredParameters = functionSymbol->getParameters();
        if (declaredParameters.size() == parameters.size()) {
            for (size_t i = 0; i < declaredParameters.size(); i++) {
                std::string const declaredParameterName = std::get<0>(declaredParameters[i]);
                std::string const declaredParameterTypeIdentifier =
                        std::get<1>(declaredParameters[i])->getIdentifier();

                std::string const parameterName = parameters[i]->getName();
                std::string const parameterTypeIdentifier = parameters[i]->getType()->getIdentifier();

                if (declaredParameterName != parameterName) {
                    throw FunctionDefinitionParameterNameMismatchError(
                            buildFunctionDefinitionParameterNameMismatchErrorMessage(
                                    name, declaredParameterName, parameterName),
                            antlrContext,
                            declaredParameterName,
                            parameterName
                    );

                }
                if (declaredParameterTypeIdentifier != parameterTypeIdentifier) {
                    throw FunctionDefinitionParameterTypeMismatchError(
                            antlrContext,
                            std::get<1>(declaredParameters[i]),
                            parameters[i]->getType()
                    );
                }
            }
        } else {
            throw FunctionDefinitionNumberOfParametersMismatchError(
                    buildFunctionDefinitionNumberOfParametersMismatchErrorMessage(name, declaredParameters.size(),
                                                                                  parameters.size()),
                    antlrContext,
                    declaredParameters.size(),
                    parameters.size()
            );
        }

        functionSymbol->setContext(functionContext);
        functionSymbol->addDefinition(definition);
        return functionSymbol;
    } else {
        FunctionSymbol::Ptr functionSymbol = std::make_shared<FunctionSymbol>(name, returnType);
        mSymbolMap[name] = functionSymbol;
        functionSymbol->setContext(functionContext);
        functionSymbol->setParameters(std::move(parameters));
        functionSymbol->addDefinition(definition);
        return functionSymbol;
    }
}

Symbol::Ptr SymbolTable::addFunctionParameter(
        antlr4::ParserRuleContext *antlrContext,
        std::string const &name,
        PrimaryType::Ptr const &primaryType,
        SymbolType parameterType
) {
    logger.trace() << "SymbolTable::addFunctionParameter(" << name << ", " << primaryType->getIdentifier() << ")";

    if (isDefined(name)) {
        auto const &symbol = getSymbol(antlrContext, name);
        // TODO: A proper error
        throw std::runtime_error("Two different parameters can't have the same name.");
    } else { // If it's defined, we just shadow it
        if (parameterType == SymbolType::ArraySymbol) {
            ArraySymbol::Ptr arraySymbol = std::make_shared<ArraySymbol>(name, primaryType);
            mSymbolMap[name] = arraySymbol;
            return castTo<Symbol::Ptr>(arraySymbol);
        } else if (parameterType == SymbolType::VariableSymbol) {
            VariableSymbol::Ptr variableSymbol = std::make_shared<VariableSymbol>(name, primaryType);
            mSymbolMap[name] = variableSymbol;
            return castTo<Symbol::Ptr>(variableSymbol);
        } else {
            throw std::runtime_error("This can't be. And it is. Have a cookie!");
        }
    }
}

FunctionSymbol::Ptr SymbolTable::addFunctionCall(
        antlr4::ParserRuleContext *antlrContext,
        std::string const &name,
        const FunctionCall::Ptr &functionCall
) {
    logger.trace() << "SymbolTable::addFunctionCall(" << name << ")";

    if (isDeclared(name)) {
        Symbol::Ptr symbol = getSymbol(antlrContext, name);
        if (symbol->getSymbolType() != SymbolType::FunctionSymbol) {
            throw DeclarationMismatchException(
                    buildMismatchSymbolTypeErrorMessage(name, SymbolType::FunctionSymbol)
            );
        }
        auto functionSymbol = castTo<FunctionSymbol::Ptr>(symbol);

        // Check if the arguments match with the function parameters types
        std::vector<PrimaryType::Ptr> const &argumentsTypes = functionCall->getArgumentsPrimaryTypes();
        auto const &parameters = functionSymbol->getParameters();
        if (argumentsTypes.size() != parameters.size()) {
            throw FunctionCallArgumentsNumberMismatchException(
                    "The function " + name + " takes " + std::to_string(parameters.size()) + " arguments, "
                    + "but only " + std::to_string(argumentsTypes.size()) + " were given."
            );
        }
        for (size_t i = 0; i < argumentsTypes.size(); ++i) {
            if (!std::get<1>(parameters[i])->greaterThan(argumentsTypes[i])) {
                std::stringstream errorMessage;
                errorMessage
                        << "The function " << name << " " << i << " parameter is of type "
                        << std::get<1>(parameters[i])->getIdentifier()
                        << ", but got a " << argumentsTypes[i]->getIdentifier() << ".";
                throw FunctionCallArgumentsTypeMismatchException(errorMessage.str());
            }
        }

        functionSymbol->addUsage(functionCall);
        return functionSymbol;
    } else {
        throw UndefinedSymbolError(
                name,
                SymbolType::FunctionSymbol,
                antlrContext
        );
    }
}

void SymbolTable::addPrimaryType(
        PrimaryType::Ptr const &primaryType,
        std::string const &name
) {
    logger.trace() << "SymbolTable::addPrimaryType(" << name << ", " << primaryType->getIdentifier() << ")";

    // Not declared and not defined
    if (isNotDeclared(name) && isNotDefined(name)) {
        mSymbolMap[name] = std::make_shared<TypeSymbol>(name, primaryType);
        mSymbolMap.at(name)->addDeclaration(nullptr);
        mSymbolMap.at(name)->addDefinition(nullptr);
    } else {
        logger.fatal() << "Can't add " << name << " as a primary type, because a symbol named " << name
                       << " already exists.";
        exit(1);
    }
}

TypeSymbol::Ptr SymbolTable::addType(
        antlr4::ParserRuleContext *antlrContext,
        TypeDefinition::Ptr definition
) {
    auto definitionSymbol = definition->getSymbol().lock();
    logger.trace() << "SymbolTable::addType(" << definitionSymbol->getName() << ", "
                   << definitionSymbol->getType()->getIdentifier() << ")";

    std::string typeAlias = definitionSymbol->getName();
    PrimaryType::Ptr primaryType = definitionSymbol->getType();

    // Not declared and not defined
    if (isNotDeclared(typeAlias)) {
        TypeSymbol::Ptr typeSymbol = std::make_shared<TypeSymbol>(typeAlias, primaryType);
        mSymbolMap[typeAlias] = typeSymbol;
        typeSymbol->addDefinition(definition);
        return typeSymbol;
    } else {
        throw caramel::exceptions::SymbolAlreadyDeclaredError(
                "Cannot execute typedef",
                mSymbolMap[typeAlias],
                antlrContext,
                mSymbolMap[typeAlias]->getDeclaration(),
                std::dynamic_pointer_cast<Declaration>(definition));
    }
}

bool SymbolTable::hasSymbol(std::string const &name) {
    return thisHasSymbol(name) || parentHasSymbol(name);
}

bool SymbolTable::thisHasSymbol(std::string const &name) {
    return mSymbolMap.find(name) != mSymbolMap.end();
}

bool SymbolTable::parentHasSymbol(std::string const &name) {
    return getParentTable() && getParentTable()->hasSymbol(name);
}

Symbol::Ptr
SymbolTable::getSymbol(antlr4::ParserRuleContext *antlrContext, std::string const &name) {
    logger.trace() << "SymbolTable::getSymbol(): " << grey << name;

    if (thisHasSymbol(name)) {
        return mSymbolMap.at(name);
    } else {
        SymbolTable::Ptr parent = getParentTable();
        if (parent) {
            return parent->getSymbol(antlrContext, name);
        } else {
            throw UndefinedSymbolError(name, antlrContext);
        }
    }
}

SymbolTable::Ptr SymbolTable::getParentTable() {
    return mParentTable;
}

size_t SymbolTable::getNumberOfSymbols() const {
    return mSymbolMap.size();
}

std::map<std::string, Symbol::Ptr> const &SymbolTable::getSymbols() const {
    return mSymbolMap;
}

void SymbolTable::acceptAstDotVisit() {
    addNode(thisId(), "SymbolTable: " + std::to_string(mSymbolMap.size()) + " symbols", "cylinder", "darkorange");
    visitChildrenAstDot();
}

void SymbolTable::visitChildrenAstDot() {
    for (auto const &symbol : mSymbolMap) {
        addEdge(thisId(), symbol.second->thisId(), symbol.first);
        symbol.second->acceptAstDotVisit();
    }
}

bool SymbolTable::isDeclared(const std::string &name) {
    return (thisHasSymbol(name) && mSymbolMap.at(name)->isDeclared()) ||
           (getParentTable() && getParentTable()->isDeclared(name));
}

bool SymbolTable::isDefined(const std::string &name) {
    return (thisHasSymbol(name) && mSymbolMap.at(name)->isDefined()) ||
           (getParentTable() && getParentTable()->isDefined(name));
}

std::string SymbolTable::buildMismatchSymbolTypeErrorMessage(std::string const &variableName,
                                                             SymbolType requiredSymbolType) {
    std::stringstream res;
    res << "Cannot defined a ";
    switch (requiredSymbolType) {
        case SymbolType::VariableSymbol:
            res << "variable";
            break;
        case SymbolType::ArraySymbol:
            res << "array";
            break;
        case SymbolType::FunctionSymbol:
            res << "function";
            break;
        case SymbolType::TypeSymbol:
            res << "type";
            break;
    }
    res << " which has a previous occurrence as a " << mSymbolMap[variableName]->getSymbolType();
    return res.str();
}

std::string
SymbolTable::buildMismatchTypeErrorMessage(std::string const &variableName, PrimaryType::Ptr const &requiredType) {
    std::stringstream res;
    res << "Mismatch type for " << variableName << " between the type definition " << requiredType->getIdentifier()
        << " and declaration type " << mSymbolMap[variableName]->getType()->getIdentifier() << '.';
    return res.str();
}

std::string
SymbolTable::buildFunctionDefinitionNumberOfParametersMismatchErrorMessage(const std::string &name,
                                                                           unsigned long declaredSize,
                                                                           unsigned long definedSize) {
    std::stringstream res;
    res << "The function: "
        << name
        << " was previously declared with "
        << declaredSize
        << " parameter(s).\n"
        << "Actual definition has "
        << definedSize
        << " parameter(s).";
    return res.str();
}

std::string SymbolTable::buildFunctionDefinitionParameterNameMismatchErrorMessage(const std::string &name,
                                                                                  std::string declaredName,
                                                                                  std::string definedName) {
    std::stringstream res;
    res << name << "'s parameter name " << definedName
        << " mismatches with the previously declared parameter " << declaredName << ".";
    return res.str();
}

} // namespace caramel::ast
