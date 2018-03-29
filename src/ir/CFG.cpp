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

#include "CFG.h"
#include "BasicBlock.h"

namespace caramel::ir {

CFG::CFG(
        std::string const &fileName,
        caramel::ast::Context::Ptr treeContext
) : mFileName{fileName},
    mTreeContext{treeContext},
    mSymbols{},
    mSymbolIndex{},
    nextBasicBlockNumber{0},
    mBasicBlocks{} {
    mBasicBlocks.push_back(std::make_shared<BasicBlock>(
            this,
            "program"
    ));
}

void CFG::addBasicBlock(std::shared_ptr<BasicBlock> basicBlock) {
    mBasicBlocks.push_back(basicBlock);
}

void CFG::generateAssembly(std::ostream &output) {

    generateAssemblyPrologue(output);
    output << std::endl;

    for (const caramel::ast::Statement::Ptr stamement : mTreeContext->getStatements()) {

        if (stamement->shouldReturnAnIR()) {
            stamement->getIR(mBasicBlocks[mBasicBlocks.size() - 1])->generateAssembly(output);
        } else if (stamement->shouldReturnABasicBlock()) {
            stamement->getBasicBlock(this)->generateAssembly(output);
        } else {
           // logger.warning() << "Statement return neither IR neither BB";
        }
    }

    output << std::endl;
    generateAssemblyEpilogue(output);

}

void CFG::generateAssemblyPrologue(std::ostream &output) {
    output << ".file  \"" << mFileName << "\"" << std::endl;
    output << ".text";
}

void CFG::generateAssemblyEpilogue(std::ostream &output) {
    output << "leave" << std::endl;
    output << "ret" << std::endl;
}

} // namespace caramel::ir
