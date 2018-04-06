/*
 * MIT License
 *
 * Copyright (c) 2018 Kalate Hexanome, 4IF, INSA Lyon
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

#include <memory>
#include <Common.h>
#include "../BinaryOperator.h"


namespace caramel::ast {

class BitwiseAndOperator : public BinaryOperator {
public:
    using Ptr = std::shared_ptr<BitwiseAndOperator>;
    using WeakPtr = std::weak_ptr<BitwiseAndOperator>;

    static constexpr const char* SYMBOL = "&";

public:
    BitwiseAndOperator() = default;

public:
    ~BitwiseAndOperator() override = default;

public:
    std::shared_ptr<caramel::ir::IR>
    getIR(
            std::shared_ptr<caramel::ir::BasicBlock> &currentBasicBlock,
            std::shared_ptr<caramel::ast::Expression> const &leftExpression,
            std::shared_ptr<caramel::ast::Expression> const &rightExpression
    ) override;

    StatementType getExpressionType() const override;

    std::string getToken() const override;

    bool shouldReturnAnIR() const override;

    bool shouldReturnABasicBlock() const override;

};

} // namespace caramel::ast
