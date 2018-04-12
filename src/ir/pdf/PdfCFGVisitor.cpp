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

#include "PdfCFGVisitor.h"
#include "PdfBasicBlockVisitor.h"

namespace caramel::ir::Pdf {
PdfCFGVisitor::PdfCFGVisitor():
    mBasicBlockVisitor{new PdfBasicBlockVisitor} {}

void PdfCFGVisitor::generateAssembly(std::shared_ptr<ir::CFG> const &controlFlowGraph, std::ostream &os) {

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

void PdfCFGVisitor::generateAssembly(
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

void PdfCFGVisitor::generateAssemblyPrologue(
        std::shared_ptr<ir::CFG> const &controlFlowGraph,
        std::ostream &os
) {
    CARAMEL_UNUSED(controlFlowGraph);

    os << "digraph {\n"
          "\tordering=out;\n"
          "\tranksep=.4;\n"
          "\tnode [shape=box, fixedsize=false, fontsize=12, fontname=\"Helvetica\", fontcolor=\"blue\"\n"
          "\t\twidth=.25, height=.25, color=\"black\", fillcolor=\"lightgrey\", style=\"filled, solid\"];\n"
          "\tedge [arrowsize=.5, color=\"black\"];\n\n";
}

void PdfCFGVisitor::generateAssemblyEpilogue(
        std::shared_ptr<ir::CFG> const &controlFlowGraph,
        std::ostream &os
) {
    CARAMEL_UNUSED(controlFlowGraph);

    os << "}\n";
}

} // namespace caramel::ir::Pdf
