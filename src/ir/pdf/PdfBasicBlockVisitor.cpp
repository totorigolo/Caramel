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

#include "PdfBasicBlockVisitor.h"
#include "PdfIRVisitor.h"

#include <sstream>


namespace caramel::ir::Pdf {

PdfBasicBlockVisitor::PdfBasicBlockVisitor() : mIRVisitor{new PdfIRVisitor} {}

static std::string escapeQuote(std::string const& str) {
    std::string ret;
    ret.reserve(str.length() + 200);
    for (char c : str) {
        if (c == '"') ret += '\\';
        ret += c;
    }
    return ret;
}

static std::string replace_leading_dot(std::string const &name) {
    std::string ret = name;
    if (ret[0] == '.') {
        ret[0] = '_';
    }
    return ret;
}

void PdfBasicBlockVisitor::generateAssembly(std::shared_ptr<ir::BasicBlock> const &basicBlock, std::ostream &os) {

    std::string mLabelName = basicBlock->getLabelName();
    if (mLabelName.empty()) {
        mLabelName = "unnamed_BB_" + std::to_string(long(this));
    } else if (mLabelName[0] == '.') {
        mLabelName[0] = '_';
    }

    std::stringstream instructionsSS;
    instructionsSS << "Name: " << mLabelName << '\n' << std::endl;
    for (IR::Ptr const &instr : basicBlock->getInstructions()) {
        instr->accept(mIRVisitor, instructionsSS);
        instructionsSS << std::endl;
    }

    os << "\tbb" << mLabelName << "[label=\"" << escapeQuote(instructionsSS.str()) << "\"];\n";

    bool whenTrue = nullptr != basicBlock->getNextWhenTrue();
    bool whenFalse = nullptr != basicBlock->getNextWhenFalse();
    if (whenTrue) {
        os << "\tbb" << mLabelName
           << " -> " << "bb" << replace_leading_dot(basicBlock->getNextWhenTrue()->getLabelName());
        if (whenFalse) {
            os << "[label=\"True\"];";
        }
        os << '\n';
    }
    if (whenFalse) {
        os << "\tbb" << mLabelName
           << " -> " << "bb" << replace_leading_dot(basicBlock->getNextWhenFalse()->getLabelName())
           << "[label=\"False\"];\n";
    }
}

} // namespace caramel::ir::Pdf
