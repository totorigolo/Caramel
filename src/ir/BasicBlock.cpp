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
        size_t functionContext,
        CFG *cfg,
        std::string entryName
) : mID{id},
    mFunctionContext{functionContext},
    mExitWhenTrue{},
    mExitWhenFalse{},
    mCfg{cfg},
    mInstructions{},
    mLabelName{std::move(entryName)} {}

std::shared_ptr<BasicBlock> BasicBlock::getNextWhenTrue() const {
    return mExitWhenTrue;
}

std::shared_ptr<BasicBlock> BasicBlock::getNextWhenFalse() const {
    return mExitWhenFalse;
}

void BasicBlock::setExitWhenTrue(const std::shared_ptr<BasicBlock> &ExitWhenTrue) {
    mExitWhenTrue = ExitWhenTrue;
}

void BasicBlock::setExitWhenFalse(const std::shared_ptr<BasicBlock> &ExitWhenFalse) {
    mExitWhenFalse = ExitWhenFalse;
}

std::string BasicBlock::addInstruction(std::shared_ptr<IR> const &instruction) {
    mInstructions.push_back(instruction);

    size_t memoryLength = instruction->getType()->getMemoryLength();
    std::string returnName = instruction->getReturnName();
    if (!returnName.empty() && returnName[0] != '!' && returnName[0] != '%' && memoryLength > 0) {
        if (!mCfg->hasSymbol(mFunctionContext, returnName)) {
            mCfg->addSymbol(mFunctionContext, returnName, instruction->getType());
        }
    }

    return returnName;
}

CFG *BasicBlock::getCFG() {
    return mCfg;
}

size_t BasicBlock::getId() const {
    return mID;
}

size_t BasicBlock::getFunctionContext() const {
    return mFunctionContext;
}

std::vector<std::shared_ptr<IR>> &BasicBlock::getInstructions() {
    return mInstructions;
}

std::string BasicBlock::getLabelName() {
    return mLabelName; // + "_" + std::to_string(getId()) + " " + std::to_string(getFunctionContext());
}

bool BasicBlock::hasSymbol(std::string const &symbolName) const {
    return mCfg->hasSymbol(mFunctionContext, symbolName);
}

long BasicBlock::isSymbolParamArray(std::string const &symbolName) {
    return mCfg->isSymbolParamArray(mFunctionContext, symbolName);
}

long BasicBlock::addParamArraySymbol(std::string const &symbolName, ast::PrimaryType::Ptr type) {
    return mCfg->addParamArraySymbol(mFunctionContext, symbolName, type);
}

long BasicBlock::addSymbol(std::string const &symbolName, ast::PrimaryType::Ptr type) {
    return mCfg->addSymbol(mFunctionContext, symbolName, type);
}

long BasicBlock::addSymbol(std::string const &symbolName, ast::PrimaryType::Ptr type, long index) {
    return mCfg->addSymbol(mFunctionContext, symbolName, type, index);
}

long BasicBlock::getSymbolIndex(std::string const &symbolName) {
    return mCfg->getSymbolIndex(mFunctionContext, symbolName);
}

std::string BasicBlock::getNextNumberName() {
    return ".L" + std::to_string(mNextNumberName++);
}

BasicBlock::Ptr BasicBlock::getNewWhenTrueBasicBlock(std::string nameSuffix) {
    auto child = mCfg->generateBasicBlock(getNextNumberName() + nameSuffix);
    if (this->getNextWhenTrue()) child->setExitWhenTrue(this->getNextWhenTrue());
    if (this->getNextWhenFalse()) child->setExitWhenFalse(this->getNextWhenFalse());
    this->setExitWhenTrue(child);
    return child;
}

void BasicBlock::addInstructions(std::shared_ptr<BasicBlock> const &child) {
    std::move(child->mInstructions.begin(), child->mInstructions.end(), std::back_inserter(mInstructions));
}

long BasicBlock::mNextNumberName = 0;

void BasicBlock::setLabelName(const std::string &name) {
    mLabelName = name;
}

} // namespace caramel::ir