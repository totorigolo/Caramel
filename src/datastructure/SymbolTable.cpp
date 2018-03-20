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

void Caramel::DataStructure::SymbolTable::addVariable(std::string const &name, antlrcpp::Any todo) {
    if(!hasVariable(name)){
        variableMap[name] = todo;
    }
}

void Caramel::DataStructure::SymbolTable::addType(std::string const &name, antlrcpp::Any todo) {
    if(!hasType(name)){
        typeMap[name] = todo;
    }
}

void Caramel::DataStructure::SymbolTable::addFunction(std::string const &name, antlrcpp::Any todo) {
    if(!hasFunction(name)){
        functionMap[name] = todo;
    }
}

bool Caramel::DataStructure::SymbolTable::hasVariable(std::string const &name) {
    return variableMap.find(name) != variableMap.end();
}

bool Caramel::DataStructure::SymbolTable::hasType(std::string const &name) {
    return typeMap.find(name) != typeMap.end();
}

bool Caramel::DataStructure::SymbolTable::hasFunction(std::string const &name) {
    return functionMap.find(name) != functionMap.end();
}

antlrcpp::Any Caramel::DataStructure::SymbolTable::getVariable(std::string const &name) {
    throw Caramel::Exceptions::NotImplementedException("SymbolTable::getVariable not currently supported !!");
}

antlrcpp::Any Caramel::DataStructure::SymbolTable::getType(std::string const &name) {
    throw Caramel::Exceptions::NotImplementedException("SymbolTable::getType not currently supported !!");
}

antlrcpp::Any Caramel::DataStructure::SymbolTable::getFunction(std::string const &name) {
    throw Caramel::Exceptions::NotImplementedException("SymbolTable::getFunction not currently supported !!");
}

Caramel::DataStructure::SymbolTable::SymbolTable() {

}
