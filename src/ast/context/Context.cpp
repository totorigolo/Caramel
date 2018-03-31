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

#include "Context.h"


namespace caramel::ast {

Context::Context()
        : mSymbolTable(std::make_shared<SymbolTable>()) {}

Context::Context(const std::shared_ptr<Context> &parent)
        : mSymbolTable(std::make_shared<SymbolTable>(parent->getSymbolTable())) {}

SymbolTable::Ptr Context::getSymbolTable() const {
    return mSymbolTable;
}

void Context::addStatements(std::vector<Statement::Ptr> &&statements) {
    std::move(statements.begin(), statements.end(), std::back_inserter(mStatements));
}

void Context::acceptAstDotVisit() {
    addNode(thisId(), "Context", "house");
    visitChildrenAstDot();

    addEdge(thisId(), mSymbolTable->thisId());
    mSymbolTable->acceptAstDotVisit();
}

void Context::visitChildrenAstDot() {
    for (auto const &statement : mStatements) {
        addEdge(thisId(), statement->thisId());
        statement->acceptAstDotVisit();
    }
}

std::vector<Statement::Ptr> Context::getStatements() {
    return mStatements;
}

std::ostream &operator<<(std::ostream &os, Context const &context) {
    os << "Context: " << context.mStatements.size() << " statements, and "
       << context.mSymbolTable->getNumberOfSymbols() << " symbols:";
    for (auto const&[name, symbol] : context.mSymbolTable->getSymbols()) {
        os << "\n  - " << name << ", " << symbol->getSymbolType();
    }
    return os;
}

} // namespace caramel::ast
