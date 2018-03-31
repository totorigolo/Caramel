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

#include "../ast/symboltable/Symbol.h"
#include <memory>

namespace caramel::ir {

class BasicBlock;
class IRVisitor;

enum class Operation {
    empty,
    ldconst,
    copy,
    add,
    sub,
    mul,
    rmem,
    wmem,
    call,
    cmp_eq,
    cmp_lt,
    cmp_le,
    pushq,
    movq,
    ret,
    prolog,
    leave,
    nope
};

class IR {
public:
    using Ptr = std::shared_ptr<IR>;
    using WeakPt = std::weak_ptr<IR>;

    static const std::string REGISTER_BASE_POINTER;
    static const std::string REGISTER_STACK_POINTER;
    static const std::string ACCUMULATOR;

public:
    explicit IR(
            std::shared_ptr<BasicBlock> parentBlock,
            Operation op,
            caramel::ast::PrimaryType::Ptr type,
            std::vector<std::string> parameters = std::vector<std::string>()
    );

    explicit IR(
            std::string const &returnName,
            std::shared_ptr<BasicBlock> parentBlock,
            Operation op,
            caramel::ast::PrimaryType::Ptr type,
            std::vector<std::string> parameters = std::vector<std::string>()
    );

    virtual ~IR() = default;

    std::string getReturnName();

    bool isEmpty() const;

    Operation getOperation() const;

    caramel::ast::PrimaryType::Ptr getType() const;

    std::shared_ptr<BasicBlock> getParentBlock();

    std::vector<std::string> getParameters();

    virtual void accept(std::shared_ptr<IRVisitor> const &visitor, std::ostream &os) = 0;

private:
    std::string mReturnName;
    std::weak_ptr<BasicBlock> mParentBlock;
    Operation mOperation;
    caramel::ast::PrimaryType::Ptr mType;
    std::vector<std::string> mParameters;
};

} // namespace caramel::dataStructure



