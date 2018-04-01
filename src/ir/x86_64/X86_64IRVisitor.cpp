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

#include "X86_64IRVisitor.h"
#include "../BasicBlock.h"

namespace caramel::ir::x86_64 {

std::string X86_64IRVisitor::address(std::string const &symbol) {
    return "(" + symbol + ")";
}

std::string X86_64IRVisitor::registerToAssembly(std::string const &register_) {

    if (register_ == ir::IR::REGISTER_BASE_POINTER) {
        return "%rbp";
    } else if (register_ == ir::IR::REGISTER_STACK_POINTER) {
        return "%rsp";
    } else if (register_ == ir::IR::ACCUMULATOR) {
        return "%eax";
    }

    throw std::runtime_error("Not valid register for " + register_);
}

std::string X86_64IRVisitor::toAssembly(ir::IR *ir, std::string const &anySymbol) {
    // Is a register
    if (anySymbol[0] == '%') {
        return registerToAssembly(anySymbol);
        // Is a temp var
    } else if (anySymbol[0] == '!') {
        return registerToAssembly(IR::ACCUMULATOR);
        // Is a constant
    } else if (anySymbol[0] >= '0' && anySymbol[0] <= '9') {
        return "$" + anySymbol;
    } else {
        return std::to_string(ir->getParentBlock()->getSymbolIndex(anySymbol)) +
               address(registerToAssembly(IR::REGISTER_BASE_POINTER));
    }
}

void X86_64IRVisitor::visitCopy(caramel::ir::CopyInstruction *instruction, std::ostream &os) {
    os << "  movl    " << toAssembly(instruction, instruction->getSource())
       << ", " << toAssembly(instruction, instruction->getDestination());
}

void X86_64IRVisitor::visitEmpty(caramel::ir::EmptyInstruction *instruction, std::ostream &os) {
    CARAMEL_UNUSED(instruction);
    CARAMEL_UNUSED(os);
}

void X86_64IRVisitor::visitProlog(caramel::ir::PrologInstruction *instruction, std::ostream &os) {
    CARAMEL_UNUSED(instruction);
    os << "  pushq   %rbp" << std::endl;
    os << "  movq    %rsp, %rbp";

}

void X86_64IRVisitor::visitEpilog(caramel::ir::EpilogInstruction *instruction, std::ostream &os) {
    CARAMEL_UNUSED(instruction);
    os << "  popq    %rbp" << std::endl;
    os << "  ret";
}

void X86_64IRVisitor::visitAddition(caramel::ir::AdditionInstruction *instruction, std::ostream &os) {
    // Todo: create visitAddition
    CARAMEL_UNUSED(instruction);
    CARAMEL_UNUSED(os);
}

void X86_64IRVisitor::visitLdConst(caramel::ir::LDConstInstruction *instruction, std::ostream &os) {
    os << "  movl    " << toAssembly(instruction, instruction->getValue())
       << ", " << toAssembly(instruction, instruction->getDestination());
}

void X86_64IRVisitor::visitNope(caramel::ir::NopInstruction *instruction, std::ostream &os) {
    CARAMEL_UNUSED(instruction);
    os << "  nop";
}
} // namespace caramel::ir::x86_64