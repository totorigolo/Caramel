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

#include "ArraySymbol.h"
#include "../../exceptions/ArraySizeMismatchException.h"


namespace caramel::ast {

ArraySymbol::ArraySymbol(const std::string &mName, const PrimaryType::Ptr &mType)
        : Symbol(mName, mType, SymbolType::ArraySymbol),
          mIsSizeDefined{false}, mSize{},
          mIsContentDefined{false} {}

ArraySymbol::ArraySymbol(const std::string &mName, const TypeSymbol::Ptr &aliasType)
        : ArraySymbol(mName, aliasType->getType()) {}

ArraySymbol::ArraySymbol(const std::string &mName, const PrimaryType::Ptr &mType, size_t size)
        : Symbol(mName, mType, SymbolType::ArraySymbol),
          mIsSizeDefined{true}, mSize{size},
          mIsContentDefined{false} {}

ArraySymbol::ArraySymbol(const std::string &mName, const TypeSymbol::Ptr &aliasType, size_t size)
        : ArraySymbol(mName, aliasType->getType(), size) {}

ArraySymbol::ArraySymbol(const std::string &mName, const PrimaryType::Ptr &mType, std::vector<Expression::Ptr> &&content)
        : Symbol(mName, mType, SymbolType::ArraySymbol),
          mIsSizeDefined{true}, mSize{content.size()},
          mIsContentDefined{true}, mContent{content} {
    logger.warning() << "Verify that content is actually moved.";
}

ArraySymbol::ArraySymbol(const std::string &mName, const TypeSymbol::Ptr &mType, std::vector<Expression::Ptr> &&content)
        : ArraySymbol(mName, mType->getType(), std::forward<decltype(mContent)>(content)) {}

bool ArraySymbol::isSizeDefined() const {
    return mIsSizeDefined;
}

long ArraySymbol::getSize() const {
    return mSize;
}

bool ArraySymbol::isContentDefined() const {
    return mIsContentDefined;
}

const std::vector<Expression::Ptr> &ArraySymbol::getContent() const {
    if (!mIsContentDefined) {
        logger.fatal() << "Calling getContent() on an ArraySymbol with undefined content.";
        exit(1);
    }
    return mContent;
}

void ArraySymbol::setContent(std::vector<Expression::Ptr> &&content) {
    if (mIsSizeDefined) {
        using namespace caramel::exceptions;
        throw ArraySizeMismatchException(
                getName(),
                mSize,
                std::to_string(content.size())
                );
    }
    mContent = content;
    logger.warning() << "Verify that content is actually moved.";
    mIsContentDefined = true;
    mSize = mContent.size();
    mIsSizeDefined = true;
}

void ArraySymbol::acceptAstDotVisit() {
    addNode(thisId(), "ArraySymbol: " + getName() + " as " + getType()->getIdentifier(), "note", "orange");

    if (mIsSizeDefined) {
        addEdge(thisId(), thisId() + 1);
        addNode(thisId() + 1, "size=" + std::to_string(mSize));
    } else {
        addEdge(thisId(), thisId() + 1);
        addNode(thisId() + 1, "size unknown");
    }

    visitChildrenAstDot();
}

void ArraySymbol::visitChildrenAstDot() {
    if (!mIsContentDefined) {
        addEdge(thisId(), thisId() + 2, "note");
        addNode(thisId() + 2, "undefined content");
    } else {
        size_t i = 0;
        for (auto const& content : mContent) {
            addEdge(thisId(), content->thisId(), std::to_string(i++));
            content->acceptAstDotVisit();
        }
    }
}

} // namespace caramel::dataStructure::symbolTable
