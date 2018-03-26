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
#include <Token.h>
#include <memory>


namespace caramel::dataStructure::statements {

class Statement {
public:
    using Ptr = std::shared_ptr<Statement>;
    using WeakPtr = std::weak_ptr<Statement>;

public:
    explicit Statement(antlr4::Token *startToken);
    virtual ~Statement() = default;

public:
    size_t getLine() const;
    size_t getColumn() const;
    size_t getLength() const;

    virtual std::shared_ptr<IR> getIR() { throw Caramel::Exceptions::NotImplementedException(__FILE__); }; // Fixme: must be abstract function at the end

private:
    size_t mLine;
    size_t mColumn;
    size_t mLength;
};

} // namespace Caramel::DataStructure::statements
