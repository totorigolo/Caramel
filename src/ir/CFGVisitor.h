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

#include "CFG.h"

namespace caramel::ir {

class CFGVisitor {
public:
    using Ptr = std::shared_ptr<CFGVisitor>;
    using WeakPtr = std::weak_ptr<CFGVisitor>;

    virtual void generateAssembly(std::shared_ptr<ir::CFG> const &controlFlowGraph, std::ostream &os) = 0;

    virtual void generateAssemblyPrologue(
            std::shared_ptr<ir::CFG> const &controlFlowGraph,
            std::ostream &os
    ) = 0;

    virtual void generateAssemblyEpilogue(
            std::shared_ptr<ir::CFG> const &controlFlowGraph,
            std::ostream &os
    ) = 0;

};

}
