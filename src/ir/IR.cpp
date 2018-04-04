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
        std::string const &name,
        Operation operation,
        std::shared_ptr<BasicBlock> parentBlock,
        ast::PrimaryType::Ptr type
) : mName{name},
    mOperation{operation},
    mParentBlock{std::move(parentBlock)},
    mType{std::move(type)} {}

IR::IR(
        Operation operation,
        std::shared_ptr<BasicBlock> parentBlock,
        ast::PrimaryType::Ptr type
) : mName{""},
    mOperation{operation},
    mParentBlock{std::move(parentBlock)},
    mType{std::move(type)} {
    logger.trace() << "Call without name";
}

Operation IR::getOperation() const {
    return mOperation;
}

caramel::ast::PrimaryType::Ptr IR::getType() const {
    return mType;
}

std::shared_ptr<BasicBlock> IR::getParentBlock() {
    return mParentBlock;
}

Operation IR::getOperation() {
    return mOperation;
}

std::string IR::getReturnName() const {
    return mName;
}

} // namespace caramel::ir
