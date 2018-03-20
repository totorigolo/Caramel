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

#pragma once

#include "Common.h"
#include "../exceptions/NotImplementedException.h"
#include <CaramelBaseVisitor.h>
#include <string>
#include <map>


namespace Caramel::DataStructure {

class SymbolTable {
public:
    SymbolTable() {

    }

    void addVariable(std::string const& name, caramel_unused antlrcpp::Any todo);
    void addType(std::string const& name, caramel_unused antlrcpp::Any todo);
    void addFunction(std::string const& name, caramel_unused antlrcpp::Any todo);

    bool hasVariable(std::string const& name);
    bool hasType(std::string const& name);
    bool hasFunction(std::string const& name);

    antlrcpp::Any getVariable(std::string const& name);
    antlrcpp::Any getType(std::string const& name);
    antlrcpp::Any getFunction(std::string const& name);

private:

    std::map<std::string, antlrcpp::Any > variableMap;
    std::map<std::string, antlrcpp::Any > typeMap;
    std::map<std::string, antlrcpp::Any > functionMap;
};

} // namespace Caramel::DataStructure
