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
#include "../Console.h"


namespace caramel::ir {

using namespace colors;

CFG::CFG(
        std::string const &fileName,
        ast::Context::Ptr treeContext
) : mFileName{fileName},
    mRootContext{std::move(treeContext)},
    mSymbols{},
    mSymbolIndex{},
    mStackLength{0},
    mNextBasicBlockNumber{0},
    mBasicBlocks{} {
    logger.debug() << "New CFG for " << mFileName << ".";

    mBasicBlocks.push_back(std::make_shared<BasicBlock>(
            mNextBasicBlockNumber,
            this,
            ""
    ));

    for (ast::Statement::Ptr const &statement : mRootContext->getStatements()) {
        if (statement->shouldReturnAnIR()) {
            mBasicBlocks[0]->addInstruction(statement->getIR(mBasicBlocks[0]));
        } else if (statement->shouldReturnABasicBlock()) {
            mBasicBlocks.push_back(statement->getBasicBlock(this));
        } else {
            if (statement->getType() != ast::StatementType::FunctionDeclaration) {
                logger.warning() << "[CFG] Statement return neither IR nor BB: " << statement->getType();
            }
        }
    }
}

void CFG::addBasicBlock(std::shared_ptr<BasicBlock> basicBlock) {
    mBasicBlocks.push_back(basicBlock);
}

bool CFG::hasSymbol(size_t controlBlockId, std::string const &symbolName) {
    return mSymbols[controlBlockId].find(symbolName) != mSymbols[controlBlockId].end()
           || mSymbols[0].find(symbolName) != mSymbols[0].end();
}

long CFG::addSymbol(size_t controlBlockId, std::string const &symbolName, caramel::ast::PrimaryType::Ptr type) {
    logger.trace() << "[CFG] Adding symbol " << yellow << "@" << controlBlockId << magenta << ": "
                   << grey << symbolName << " of type " << type->getIdentifier();

    if (hasSymbol(controlBlockId, symbolName)) {
        long existingIndex = getSymbolIndex(controlBlockId, symbolName);
        logger.warning() << "[CFG] Symbol " << symbolName << " already exists with index " << existingIndex << ".";
        return existingIndex;
    }

    mSymbols[controlBlockId][symbolName] = type;
    mStackLength = mStackLength - type->getMemoryLength() / 8U;
    mSymbolIndex[controlBlockId][symbolName] = mStackLength;

    logger.trace() << "[CFG] " << "  => " << mStackLength;
    return mStackLength;
}

long CFG::addSymbol(size_t controlBlockId, std::string const &symbolName, ast::PrimaryType::Ptr type, long index) {
    logger.trace() << "[CFG] Adding symbol: " << grey << symbolName << " of type " << type->getIdentifier()
                   << yellow << " at " << index;

    if (hasSymbol(controlBlockId, symbolName)) {
        long existingIndex = getSymbolIndex(controlBlockId, symbolName);
        logger.warning() << "[CFG] Symbol " << symbolName << "already exists with index " << existingIndex << ".";
        return existingIndex;
    }

    mSymbols[controlBlockId][symbolName] = type;
    mSymbolIndex[controlBlockId][symbolName] = index;
    if (index > 0) {
        logger.warning() << "[CFG] TODO: Check this statement.";
        mStackLength = size_t(index);
    }

    logger.trace() << "[CFG] " << "  => " << index;
    return index;
}

long CFG::getSymbolIndex(size_t controlBlockId, std::string const &symbolName) {
    if (mSymbolIndex[controlBlockId].find(symbolName) != mSymbolIndex[controlBlockId].end()) {
        return mSymbolIndex[controlBlockId][symbolName];
    } else if (mSymbolIndex[0].find(symbolName) != mSymbolIndex[0].end()) {
        return mSymbolIndex[0][symbolName];
    } else {
        return 0;
    }
}

void CFG::enterFunction() {
    mStackLengthMemory = mStackLength;
    mStackLength = 0;
}

void CFG::leaveFunction() {
    mStackLength = mStackLengthMemory;
}

std::shared_ptr<BasicBlock> CFG::generateBasicBlock(std::string const &entryName) {
    return std::make_shared<BasicBlock>(mNextBasicBlockNumber, this, entryName);
}

std::shared_ptr<BasicBlock> CFG::generateFunctionBlock(std::string const &entryName) {
    return std::make_shared<BasicBlock>(++mNextBasicBlockNumber, this, entryName);
}

std::vector<std::shared_ptr<BasicBlock>> &CFG::getBasicBlocks() {
    return mBasicBlocks;
}

std::string &CFG::getFileName() {
    return mFileName;
}

std::ostream &operator<<(std::ostream &os, CFG const &cfg) {
    os << "CFG:\n"
       << " - mFileName: " << cfg.mFileName << '\n'
       << " - mRootContext: " << *cfg.mRootContext << '\n'
       << " - mSymbols & mSymbolsIndex:\n";
    for (auto const &[basicBlockId, symbols] : cfg.mSymbols) {
        for (auto const &[name, type] : symbols) {
            os << "    - BB=" << basicBlockId
               << ", name=" << name
               << ", type=" << type->getIdentifier()
               << ", index=" << cfg.mSymbolIndex.at(basicBlockId).at(name)
               << '\n';
        }
    }
    os << " - mStackLength: " << cfg.mStackLength << '\n'
       << " - mStackLengthMemory: " << cfg.mStackLengthMemory << '\n'
       << " - mNextBasicBlockNumber: " << cfg.mNextBasicBlockNumber << '\n'
       << " - mBasicBlocks: " << cfg.mBasicBlocks.size() << " BBs";
    return os;
}

} // namespace caramel::ir
