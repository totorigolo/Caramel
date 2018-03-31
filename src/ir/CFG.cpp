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

#include "CFG.h"
#include "BasicBlock.h"


namespace caramel::ir {

CFG::CFG(
        std::string const &fileName,
        ast::Context::Ptr treeContext
) : mFileName{fileName},
    mRootContext{std::move(treeContext)},
    mSymbols{},
    mSymbolIndex{},
    stackLength{0},
    mBasicBlocks{},
    nextBasicBlockNumber{0} {
    mBasicBlocks.push_back(std::make_shared<BasicBlock>(
            nextBasicBlockNumber,
            this,
            ""
    ));
}

void CFG::addBasicBlock(std::shared_ptr<BasicBlock> basicBlock) {
    mBasicBlocks.push_back(basicBlock);
}

void CFG::generateAssembly(std::ostream &output) {

    for (ast::Statement::Ptr const &statement : mRootContext->getStatements()) {
        if (statement->shouldReturnAnIR()) {
            mBasicBlocks[0]->addInstruction(statement->getIR(mBasicBlocks[0]));
        } else if (statement->shouldReturnABasicBlock()) {
            mBasicBlocks.push_back(statement->getBasicBlock(this));
        } else {
            if (statement->getType() != ast::StatementType::FunctionDeclaration) {
                logger.warning() << "Statement return neither IR nor BB: " << statement->getType();
            }
        }
    }

    generateAssemblyPrologue(output);
    output << std::endl;
    output.flush();

    for (BasicBlock::Ptr const &bb : mBasicBlocks) {
        bb->generateAssembly(output);
    }

    output << std::endl;
    generateAssemblyEpilogue(output);

}

void CFG::generateAssemblyPrologue(std::ostream &output) {
    output << ".file  \"" << mFileName << "\"" << std::endl;
    output << ".text";
}

void CFG::generateAssemblyEpilogue(std::ostream &output) {}

bool CFG::hasSymbol(int controlBlockId, std::string const &symbolName) {
    return mSymbols[controlBlockId].find(symbolName) != mSymbols[controlBlockId].end()
           && mSymbols[0].find(symbolName) != mSymbols[0].end();
}

void CFG::addSymbol(int controlBlockId, std::string const &symbolName, caramel::ast::PrimaryType::Ptr type) {
    mSymbols[controlBlockId][symbolName] = type;
    stackLength = stackLength - type->getMemoryLength() / 8U;
    mSymbolIndex[controlBlockId][symbolName] = stackLength;
}

long CFG::getSymbolIndex(int controlBlockId, std::string const &symbolName) {
    if (mSymbolIndex[controlBlockId].find(symbolName) != mSymbolIndex[controlBlockId].end()) {
        return mSymbolIndex[controlBlockId][symbolName];
    } else if (mSymbolIndex[0].find(symbolName) != mSymbolIndex[0].end()) {
        return mSymbolIndex[0][symbolName];
    } else {
        return 0;
    }
}

void CFG::enterFunction() {
    stackLengthMemory = stackLength;
    stackLength = 0;
}

void CFG::exitFunction() {
    stackLength = stackLengthMemory;
}

std::shared_ptr<BasicBlock> CFG::generateBasicBlock(std::string const &entryName) {
    return std::make_shared<BasicBlock>(nextBasicBlockNumber, this, entryName);
}

std::shared_ptr<BasicBlock> CFG::generateFunctionBlock(std::string const &entryName) {
    return std::make_shared<BasicBlock>(++nextBasicBlockNumber, this, entryName);
}


} // namespace caramel::ir
