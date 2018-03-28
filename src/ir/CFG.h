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

#include "../datastructure/statements/Statement.h"
#include "../datastructure/symboltable/Symbol.h"

namespace caramel::ir {

class BasicBlock;

class CFG {
public:
    using Ptr = std::shared_ptr<CFG>;
    using WeakPtr = std::weak_ptr<CFG>;

public:
    explicit CFG(caramel::ast::Statement::Ptr ast);
    virtual ~CFG() = default;

public:
    void addBasicBlock(
            std::shared_ptr<BasicBlock> basicBlock
    );

    void generateAssembly(std::ostream &output);

    std::string IRToAssembly(std::string register_) { throw caramel::exceptions::NotImplementedException(__FILE__); };

    void generateAssemblyPrologue(std::ostream &output) { throw caramel::exceptions::NotImplementedException(__FILE__); };

    void generateAssemblyEpilogue(std::ostream &output) { throw caramel::exceptions::NotImplementedException(__FILE__); };


protected:
    caramel::ast::Statement::Ptr mAst;
    std::map<std::string, caramel::ast::Symbol> mSymbols;
    std::map<std::string, int> mSymbolIndex;
    int nextBasicBlockNumber;
    
    std::vector<std::shared_ptr<BasicBlock>> mBasicBlocks;
    
};

} // namespace caramel::ast


