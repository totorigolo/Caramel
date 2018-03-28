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
        std::shared_ptr<caramel::ir::CFG> cfg,
        std::string const &entryName
) : mExitWhenTrue{},
    mExitWhenFalse{},
    mLabelName{entryName},
    mCfg{cfg},
    mInstructions{} {}

void BasicBlock::addIRInstruction(Operation op, caramel::ast::SymbolType type, std::vector<std::string> params) {
    IR::Ptr instruction = std::make_shared<IR>(std::shared_ptr<BasicBlock>(this), op, type, params);
    mInstructions.push_back(instruction);
}

void BasicBlock::generateAssembly(std::ostream &output) {
    output << mLabelName << ':' << std::endl;
    for(IR::Ptr const &instr : mInstructions) {
        output << "  ";
        instr->generateAssembly(output);
        output << std::endl;
    }
    output << std::endl;
}

std::shared_ptr<BasicBlock> BasicBlock::getNextWhenTrue() const {
    return mExitWhenTrue;
}

std::shared_ptr<BasicBlock> BasicBlock::getNextWhenFalse() const {
    return mExitWhenFalse;
}

} // namespace caramel::ir