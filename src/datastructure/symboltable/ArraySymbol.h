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

#include "Symbol.h"
#include "TypeSymbol.h"
#include "../../datastructure/statements/expressions/Expression.h"

#include <memory>
#include <vector>

namespace caramel::ast {

class Expression;

class ArraySymbol : public Symbol {
public:
    using Ptr = std::shared_ptr<ArraySymbol>;
    using WeakPtr = std::weak_ptr<ArraySymbol>;

public:
    ArraySymbol(const std::string &mName, const PrimaryType::Ptr &mType);
    ArraySymbol(const std::string &mName, const TypeSymbol::Ptr &mType);
    ArraySymbol(const std::string &mName, const PrimaryType::Ptr &mType, size_t size);
    ArraySymbol(const std::string &mName, const TypeSymbol::Ptr &mType, size_t size);
    ArraySymbol(const std::string &mName, const PrimaryType::Ptr &mType, std::vector<Expression::Ptr> &&content);
    ArraySymbol(const std::string &mName, const TypeSymbol::Ptr &mType, std::vector<Expression::Ptr> &&content);

    ~ArraySymbol() override = default;

    bool isSizeDefined() const;
    long getSize() const;

    bool isContentDefined() const;
    std::vector<Expression::Ptr> const &getContent() const;
    void setContent(std::vector<Expression::Ptr> &&content);

    // TODO: Make acceptAstDotVisit() and visitChildrenAstDot() const.
    void acceptAstDotVisit() override;
    void visitChildrenAstDot() override;

private:
    bool mIsSizeDefined;
    size_t mSize;
    bool mIsContentDefined;
    std::vector<Expression::Ptr> mContent;
};

} // namespace caramel::ast


