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

#include "BasicBlock.h"

namespace caramel::ir {

BasicBlock::BasicBlock(
        size_t id,
        CFG *cfg,
        std::string entryName
) : mID{id},
    mExitWhenTrue{},
    mExitWhenFalse{},
    mLabelName{std::move(entryName)},
    mCfg{cfg},
    mInstructions{} {}

std::shared_ptr<BasicBlock> BasicBlock::getNextWhenTrue() const {
    return mExitWhenTrue;
}

std::shared_ptr<BasicBlock> BasicBlock::getNextWhenFalse() const {
    return mExitWhenFalse;
}

void BasicBlock::setMExitWhenTrue(const std::shared_ptr<BasicBlock> &ExitWhenTrue) {
    mExitWhenTrue = ExitWhenTrue;
}

void BasicBlock::setMExitWhenFalse(const std::shared_ptr<BasicBlock> &ExitWhenFalse) {
    mExitWhenFalse = ExitWhenFalse;
}

std::string BasicBlock::addInstruction(std::shared_ptr<IR> const &instruction) {
    if (!instruction->isEmpty()) {
        mInstructions.push_back(instruction);
    }

    size_t memoryLength = instruction->getType()->getMemoryLength();
    std::string returnName = instruction->getReturnName();
    if (!returnName.empty() && returnName[0] != '!' && returnName[0] != '%' && memoryLength > 0) {
        if (!mCfg->hasSymbol(mID, returnName)) {
            mCfg->addSymbol(mID, returnName, instruction->getType());
        }
    }

    return returnName;
}

CFG *BasicBlock::getCFG() {
    return mCfg;
}

size_t BasicBlock::getId() {
    return mID;
}

std::vector<std::shared_ptr<IR>> &BasicBlock::getInstructions() {
    return mInstructions;
}

std::string &BasicBlock::getLabelName() {
    return mLabelName;
}

long BasicBlock::addSymbol(std::string const &symbolName, ast::PrimaryType::Ptr type) {
    return mCfg->addSymbol(mID, symbolName, type);
}

long BasicBlock::addSymbol(std::string const &symbolName, ast::PrimaryType::Ptr type, long index) {
    return mCfg->addSymbol(mID, symbolName, type, index);
}

long BasicBlock::getSymbolIndex(std::string const &symbolName) {
    return mCfg->getSymbolIndex(mID, symbolName);
}

std::string BasicBlock::getNextNumberName() {
    long tmp = mNextNumberName;
    mNextNumberName++;
    return ".L" + std::to_string(tmp);
}

void BasicBlock::addInstructions(std::shared_ptr<BasicBlock> const &child) {
    std::move(child->mInstructions.begin(), child->mInstructions.end(), std::back_inserter(mInstructions));
}

long BasicBlock::mNextNumberName = 0;


} // namespace caramel::ir