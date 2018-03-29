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

const std::string IR::REGISTER_BASE_POINTER = "%rbp";
const std::string IR::REGISTER_STACK_POINTER = "%rsp";
const std::string IR::ACCUMULATOR = "%eax";

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

    switch (mOperation) {
        case Operation::copy:
            output << "copy";
            break;
        case Operation::add:
            output << "add";
            break;
        case Operation::call:
            output << "call";
            break;
        case Operation::cmp_eq:
            output << "cmp_eq";
            break;
        case Operation::cmp_le:
            output << "cmp_le";
            break;
        case Operation::cmp_lt:
            output << "cmp_lt";
            break;
        case Operation::ldconst:
            output << "ldconst";
            break;
        case Operation::mul:
            output << "mul";
            break;
        case Operation::rmem:
            output << "rmem";
            break;
        case Operation::sub:
            output << "sub";
            break;
        case Operation::wmem:
            output << "wmem";
            break;
        case Operation::pushq:
            output << "pushq";
            break;
        case Operation::movq:
            output << "movq";
            break;
        case Operation::ret:
            output << "ret";
            break;
        case Operation::leave:
            output << "leave";
            break;
        case Operation::empty:
            logger.warning() << "empty instruction was called";
            return;
    }
    output << " ";
    BasicBlock::Ptr parentBlock = mParentBlock.lock();
    if (mParameters.size() > 0) {
        for (int i = 0; i < mParameters.size() - 1; i++) {
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
