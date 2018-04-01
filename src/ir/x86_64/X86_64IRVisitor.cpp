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
#include "../instructions/CopyInstruction.h"
#include "../instructions/EmptyInstruction.h"
#include "../instructions/PrologInstruction.h"
#include "../instructions/EpilogInstruction.h"
#include "../instructions/LDConstInstruction.h"
#include "../instructions/FunctionCallInstruction.h"
#include "../instructions/NopInstruction.h"

namespace caramel::ir::x86_64 {

std::string X86_64IRVisitor::address(std::string const &symbol) {
    return "(" + symbol + ")";
}

std::string X86_64IRVisitor::registerToAssembly(std::string const &register_) {
    logger.trace() << "[x86_64] " << "registerToAssembly(" << register_ << ")";
    std::string r;

    if (register_ == ir::IR::REGISTER_BASE_POINTER) {
        r = "%rbp";
    } else if (register_ == ir::IR::REGISTER_STACK_POINTER) {
        r = "%rsp";
    } else if (register_ == ir::IR::ACCUMULATOR) {
        r = "%eax";
    } else {
        throw std::runtime_error("Not valid register for " + register_);
    }

    logger.trace() << "[x86_64] " << "  => " << r;
    return r;
}

std::string X86_64IRVisitor::toAssembly(ir::IR *ir, std::string const &anySymbol) {
    logger.trace() << "[x86_64] " << "toAssembly(" << "ir" << ", " << anySymbol << ")";
    std::string r;

    // Is a register
    if (anySymbol[0] == '%') {
        r = registerToAssembly(anySymbol);
        // Is a temp var
    } else if (anySymbol[0] == '!') {
        r = registerToAssembly(IR::ACCUMULATOR);
        // Is a constant
    } else if (anySymbol[0] >= '0' && anySymbol[0] <= '9') {
        r = "$" + anySymbol;
    } else {
        r = std::to_string(ir->getParentBlock()->getSymbolIndex(anySymbol)) +
               address(registerToAssembly(IR::REGISTER_BASE_POINTER));
    }

    logger.trace() << "[x86_64] " << "  => " << r;
    return r;
}

void X86_64IRVisitor::visitCopy(caramel::ir::CopyInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting copy: " << instruction->getReturnName();

    os << "  movl    " << toAssembly(instruction, instruction->getSource())
       << ", " << toAssembly(instruction, instruction->getDestination());
}

void X86_64IRVisitor::visitEmpty(caramel::ir::EmptyInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting empty";

    CARAMEL_UNUSED(instruction);
    CARAMEL_UNUSED(os);
}

void X86_64IRVisitor::visitProlog(caramel::ir::PrologInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting prolog";

    CARAMEL_UNUSED(instruction);
    os << "  pushq   %rbp" << std::endl;
    os << "  movq    %rsp, %rbp";

}

void X86_64IRVisitor::visitEpilog(caramel::ir::EpilogInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting epilog";

    CARAMEL_UNUSED(instruction);
    os << "  popq    %rbp" << std::endl;
    os << "  ret";
}

void X86_64IRVisitor::visitAddition(caramel::ir::AdditionInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting addition: "; // TODO: complete the trace

    // Todo: create visitAddition
    CARAMEL_UNUSED(instruction);
    CARAMEL_UNUSED(os);
}

void X86_64IRVisitor::visitLdConst(caramel::ir::LDConstInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting ldconst: " << instruction->getReturnName() << " = " << instruction->getValue();

    os << "  movl    " << toAssembly(instruction, instruction->getValue())
       << ", " << toAssembly(instruction, instruction->getDestination());
}

void X86_64IRVisitor::visitNope(caramel::ir::NopInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting nop";

    CARAMEL_UNUSED(instruction);
    os << "  nop";
}

void X86_64IRVisitor::visitFunctionCall(caramel::ir::FunctionCallInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting functionCall: " << instruction->getFunctionName();

    CARAMEL_UNUSED(instruction);
    os << "";
}

} // namespace caramel::ir::x86_64
