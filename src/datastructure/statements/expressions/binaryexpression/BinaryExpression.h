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

#include "../Expression.h"
#include "../../../operators/BinaryOperator.h"


namespace caramel::ast {

class BinaryExpression : public caramel::ast::Expression {
public:
    using Ptr = std::shared_ptr<BinaryExpression>;
    using WeakPtr = std::weak_ptr<BinaryExpression>;

    BinaryExpression(
            std::shared_ptr<caramel::ast::Expression> const &leftExpression,
            std::shared_ptr<caramel::ast::BinaryOperator> const &binaryOperator,
            std::shared_ptr<caramel::ast::Expression> const &rightExpression,
            antlr4::Token *startToken
    );

    ~BinaryExpression() override = default;

public:
    std::shared_ptr<IR> getIR() override;

private:
    std::shared_ptr<caramel::ast::Expression> mLeftExpression;
    std::shared_ptr<caramel::ast::BinaryOperator> mBinaryOperator;
    std::shared_ptr<caramel::ast::Expression> mRightExpression;

};

} // namespace caramel::ast::binaryExpression
