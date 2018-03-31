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

#include <utility>
#include <vector>
#include "IR.h"
#include "BasicBlock.h"

namespace caramel::ir {

IR::IR(
        std::shared_ptr<BasicBlock> parentBlock,
        Operation op,
        caramel::ast::PrimaryType::Ptr type,
        std::vector<std::string> parameters
) : mReturnName{""},
    mParentBlock{parentBlock},
    mOperation{op},
    mType{type},
    mParameters{std::move(parameters)} {}

IR::IR(
        std::string const &returnName,
        std::shared_ptr<BasicBlock> parentBlock,
        Operation op,
        caramel::ast::PrimaryType::Ptr type,
        std::vector<std::string> parameters
) : mReturnName{returnName},
    mParentBlock{parentBlock},
    mOperation{op},
    mType{type},
    mParameters{std::move(parameters)} {}

void IR::generateAssembly(std::ostream &output) {

#define OUTPUT_ENUM_CASE(enum, option)      \
    case enum::option:                      \
        output << #option;                  \
    break;

    switch (mOperation) {
        OUTPUT_ENUM_CASE(Operation, copy)
        OUTPUT_ENUM_CASE(Operation, add)
        OUTPUT_ENUM_CASE(Operation, call)
        OUTPUT_ENUM_CASE(Operation, cmp_eq)
        OUTPUT_ENUM_CASE(Operation, cmp_le)
        OUTPUT_ENUM_CASE(Operation, cmp_lt)
        OUTPUT_ENUM_CASE(Operation, ldconst)
        OUTPUT_ENUM_CASE(Operation, mul)
        OUTPUT_ENUM_CASE(Operation, rmem)
        OUTPUT_ENUM_CASE(Operation, sub)
        OUTPUT_ENUM_CASE(Operation, wmem)
        OUTPUT_ENUM_CASE(Operation, pushq)
        OUTPUT_ENUM_CASE(Operation, movq)
        OUTPUT_ENUM_CASE(Operation, ret)
        OUTPUT_ENUM_CASE(Operation, leave)
        case Operation::empty:
            logger.warning() << "empty instruction was called";
            return;
    }
    output << " ";
    BasicBlock::Ptr parentBlock = mParentBlock.lock();
    if (!mParameters.empty()) {
        for (size_t i = 0; i < mParameters.size() - 1; i++) {
            output << mParameters[i] << "("
                   << parentBlock->getCFG()->getSymbolIndex(parentBlock->getId(), mParameters[i]) << "), ";
        }
        output << mParameters[mParameters.size() - 1] << "("
               << parentBlock->getCFG()->getSymbolIndex(parentBlock->getId(), mParameters[mParameters.size() - 1]) << ")";
    }
}

std::string IR::getReturnName() {
    return mReturnName;
}

bool IR::isEmpty() {
    return mOperation == Operation::empty;
}

caramel::ast::PrimaryType::Ptr IR::getType() {
    return mType;
}


} // namespace caramel::ir
