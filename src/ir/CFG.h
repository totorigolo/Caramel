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

#include <ostream>
#include "../ast/statements/Statement.h"
#include "../ast/symboltable/Symbol.h"
#include "../ast/context/Context.h"
#include "BasicBlock.h"

namespace caramel::ir {

class BasicBlock;

class CFG {
public:
    using Ptr = std::shared_ptr<CFG>;
    using WeakPtr = std::weak_ptr<CFG>;

public:
    explicit CFG(std::string const &fileName, ast::Context::Ptr ast);
    virtual ~CFG() = default;

public:

    std::shared_ptr<BasicBlock> generateBasicBlock(std::string entryName = "");
    std::shared_ptr<BasicBlock> generateFunctionBlock(std::string entryName);
    std::shared_ptr<BasicBlock> generateNamedBasicBlock();


    void addBasicBlock(
            std::shared_ptr<BasicBlock> basicBlock
    );
    std::vector<std::shared_ptr<BasicBlock>> & getBasicBlocks();

    bool hasSymbol(size_t controlBlockId, std::string const &symbolName);
    bool isSymbolParamArray(size_t controlBlockId, std::string const &symbolName);
    long addParamArraySymbol(size_t controlBlockId, std::string const &symbolName, ast::PrimaryType::Ptr type);
    long addArraySymbol(size_t controlBlockId, std::string const &symbolName, ast::PrimaryType::Ptr type, size_t length);
    long addSymbol(size_t controlBlockId, std::string const &symbolName, ast::PrimaryType::Ptr type);
    long addSymbol(size_t controlBlockId, std::string const &symbolName, ast::PrimaryType::Ptr type, long index);
    long getSymbolIndex(size_t controlBlockId, std::string const &symbolName);

    std::shared_ptr<BasicBlock> getFunctionBasicBlock(size_t functionBasicBlockIndex);
    std::shared_ptr<BasicBlock> getFunctionEndBasicBlock(size_t functionBasicBlockIndex);


    void addFunctionBBEnd(size_t functionId, std::shared_ptr<BasicBlock> functionBBEnd);

    void enterFunction(size_t controlBlockId);
    void leaveFunction(size_t controlBlockId);

    std::string & getFileName();

    friend std::ostream &operator<<(std::ostream &os, CFG const &cfg);

protected:
    std::string mFileName;
    ast::Context::Ptr mRootContext;

    std::map<size_t, std::map<std::string, ast::PrimaryType::Ptr>> mSymbols;
    std::map<size_t, std::map<std::string, long>> mSymbolIndex;
    std::map<size_t, std::map<std::string, bool>> mSymbolIsParamArray;
    std::map<size_t, long> mStackSize;

    int mNextBasicBlockNumber;
    int mNextFunctionContext;

    std::vector<std::shared_ptr<BasicBlock>> mFunctionsBasicBlocks;
    std::map<size_t, std::shared_ptr<BasicBlock>> mFunctionBBEnd;
    std::vector<caramel::ast::Declaration::Ptr> mGlobalContext;
};

} // namespace caramel::ast


