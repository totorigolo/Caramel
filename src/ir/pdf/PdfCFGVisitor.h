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

#include "../CFG.h"
#include "../CFGVisitor.h"
#include "../BasicBlock.h"

#include <map>
#include <set>
#include <memory>
#include <ostream>

namespace caramel::ir::Pdf {

class PdfBasicBlockVisitor;

class PdfCFGVisitor : public CFGVisitor {
public:
    using Ptr = std::shared_ptr<PdfCFGVisitor>;
    using WeakPtr = std::weak_ptr<PdfCFGVisitor>;

public:
    explicit PdfCFGVisitor();

    virtual ~PdfCFGVisitor() = default;

    void generateAssembly(std::shared_ptr<ir::CFG> const &controlFlowGraph, std::ostream &os);

    void generateAssembly(
            std::shared_ptr<ir::CFG> const &controlFlowGraph,
            std::ostream &os,
            size_t functionRootId,
            ir::BasicBlock::Ptr bb);

    void generateAssemblyPrologue(
            std::shared_ptr<ir::CFG> const &controlFlowGraph,
            std::ostream &os
    );

    void generateAssemblyEpilogue(
            std::shared_ptr<ir::CFG> const &controlFlowGraph,
            std::ostream &os
    );

private:
    std::shared_ptr<PdfBasicBlockVisitor> mBasicBlockVisitor;
    std::map<size_t, std::vector<ir::BasicBlock::Ptr>> mOrders;
    std::set<size_t> mVisitedBB;
};

} // namespace caramel::ir::Pdf



