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

#include "../../exceptions/NotImplementedException.h"
#include "IR.h"
#include "../../AstDotNode.h"
#include <Token.h>
#include <memory>
#include <ostream>


namespace caramel::ast {

// TODO: Remove this unneeded enum
// TODO: Split into distinct StatementType cpp files.
enum class StatementType {
    Unknown,
    Declaration,
    VariableDeclaration,
    FunctionDeclaration,
    ArrayDeclaration,
    Definition,
    VariableDefinition,
    FunctionDefinition,
    ArrayDefinition,
    Expression,
    UnaryExpression,
    AtomicExpression,
    Constant,

    // BinaryExpression w/ sub-types
    BinaryExpression,
    BitwiseShiftExpression,
    AdditiveExpression,
    MultiplicativeExpression

};

class Statement : public AstDotNode {
public:
    using Ptr = std::shared_ptr<Statement>;
    using WeakPtr = std::weak_ptr<Statement>;

public:
    ~Statement() override = default;

    explicit Statement(antlr4::Token *startToken, StatementType type = StatementType::Unknown);

    size_t getLine() const;
    size_t getColumn() const;
    size_t getLength() const;
    StatementType getType() const;

    virtual std::shared_ptr<IR> getIR() { throw caramel::exceptions::NotImplementedException(__FILE__); }; // Fixme: must be abstract function at the end

    void acceptAstDotVisit() override;

private:
    size_t mLine;
    size_t mColumn;
    size_t mLength;
    StatementType mType;
};

std::ostream &operator<<(std::ostream &os, const StatementType &type);

} // namespace caramel::ast
