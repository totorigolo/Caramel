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

#include "IR.h"
#include "CFG.h"
#include "../ast/symboltable/Symbol.h"
#include <ostream>
#include <vector>

namespace caramel::ir {

class BasicBlock {
public:
    using Ptr = std::shared_ptr<BasicBlock>;
    using WeakPtr = std::shared_ptr<BasicBlock>;

    BasicBlock(
            int id,
            CFG *cfg,
            std::string const &entryName
    );

    virtual ~BasicBlock() = default;

public:

    std::string addInstruction(std::shared_ptr<IR> const &instruction);

    std::vector<std::shared_ptr<IR>> & getInstructions();
    std::string & getLabelName();

    std::shared_ptr<BasicBlock> getNextWhenTrue() const;
    std::shared_ptr<BasicBlock> getNextWhenFalse() const;

    long getSymbolIndex(std::string const &symbolName);

    CFG * getCFG();

    int getId();

private:
    int mID;
    /**
     * pointer to the next basic block, true branch. If nullptr, return from procedure
     */
    std::shared_ptr<BasicBlock> mExitWhenTrue;
    /**
     * pointer to the next basic block, false branch. If nullptr, the basic block ends with an unconditional jump
     */
    std::shared_ptr<BasicBlock> mExitWhenFalse;
    std::string mLabelName;
    CFG *mCfg;
    std::vector<std::shared_ptr<IR>> mInstructions;
    std::map<std::string, int> mSymbolsIndex;

};

} // namespace caramel::ir


