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

#include "X86_64BasicBlockVisitor.h"
#include "X86_64IRVisitor.h"

namespace caramel::ir::x86_64 {
X86_64BasicBlockVisitor::X86_64BasicBlockVisitor() : mIRVisitor{new X86_64IRVisitor} {}

void X86_64BasicBlockVisitor::generateAssembly(std::shared_ptr<ir::BasicBlock> const &basicBlock, std::ostream &os) {

    std::string mLabelName = basicBlock->getLabelName();

    if (!mLabelName.empty()) {
        os << mLabelName << ':' << std::endl;
    }
    for (IR::Ptr const &instr : basicBlock->getInstructions()) {
        instr->accept(mIRVisitor, os);
        os << std::endl;
    }

    os << std::endl;

}

} // namespace caramel::ir::x86_64
