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

#include "X86_64CFGVisitor.h"
#include "X86_64BasicBlockVisitor.h"

namespace caramel::ir::x86_64 {
X86_64CFGVisitor::X86_64CFGVisitor():
    mBasicBlockVisitor{new X86_64BasicBlockVisitor} {}

void X86_64CFGVisitor::generateAssembly(std::shared_ptr<ir::CFG> const &controlFlowGraph, std::ostream &os) {

    generateAssemblyPrologue(controlFlowGraph, os);
    os << std::endl;

    for (auto const &function_root_bb : controlFlowGraph->getBasicBlocks()) {
        generateAssembly(controlFlowGraph, os, function_root_bb->getId(), function_root_bb);

        auto &order = mOrders[function_root_bb->getId()];
        for (auto it = order.rbegin(), end_it = order.rend(); it != end_it; ++it) {
            mBasicBlockVisitor->generateAssembly(*it, os);
        }
    }

    os << std::endl;
    generateAssemblyEpilogue(controlFlowGraph, os);
}

void X86_64CFGVisitor::generateAssembly(
        std::shared_ptr<ir::CFG> const &controlFlowGraph,
        std::ostream &os,
        size_t functionRootId,
        ir::BasicBlock::Ptr bb) {
    if (mVisitedBB.find(bb->getId()) != mVisitedBB.end()) {
        return;
    }
    mVisitedBB.insert(bb->getId());

    if (bb->getNextWhenFalse()) {
        generateAssembly(controlFlowGraph, os, functionRootId , bb->getNextWhenFalse());
    }
    if (bb->getNextWhenTrue()) {
        generateAssembly(controlFlowGraph, os, functionRootId , bb->getNextWhenTrue());
    }

    mOrders[functionRootId].push_back(bb);
}

void X86_64CFGVisitor::generateAssemblyPrologue(
        std::shared_ptr<ir::CFG> const &controlFlowGraph,
        std::ostream &os
) {
    os << ".file  \"" << controlFlowGraph->getFileName() << "\"" << '\n'
       << ".text" << '\n'
       << '\n'
       << ".Ltext0:" << '\n'
       << "  .globl main" << '\n'
       << "  .type main, @function\n";
}

void X86_64CFGVisitor::generateAssemblyEpilogue(
        std::shared_ptr<ir::CFG> const &controlFlowGraph,
        std::ostream &os
) {
    CARAMEL_UNUSED(controlFlowGraph);
    CARAMEL_UNUSED(os);

    // Strings
}

} // namespace caramel::ir::x86_64