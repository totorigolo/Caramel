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

std::string IR::getReturnName() {
    return mReturnName;
}

bool IR::isEmpty() const {
    return mOperation == Operation::empty;
}

caramel::ast::PrimaryType::Ptr IR::getType() const {
    return mType;
}

Operation IR::getOperation() const {
    return mOperation;
}

std::shared_ptr<BasicBlock> IR::getParentBlock() {
    return mParentBlock.lock();
}

std::vector<std::string> IR::getParameters() {
    return mParameters;
}


} // namespace caramel::ir
