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

#include "PdfIRVisitor.h"
#include "../BasicBlock.h"
#include "../instructions/CopyInstruction.h"
#include "../instructions/CopyAddrInstruction.h"
#include "../instructions/ArrayAccessInstruction.h"
#include "../instructions/EmptyInstruction.h"
#include "../instructions/PrologInstruction.h"
#include "../instructions/EpilogInstruction.h"
#include "../instructions/LDConstInstruction.h"
#include "../instructions/FunctionCallInstruction.h"
#include "../instructions/NopInstruction.h"
#include "../instructions/ReturnInstruction.h"
#include "../instructions/AdditionInstruction.h"
#include "../instructions/SubtractionInstruction.h"
#include "../instructions/PushInstruction.h"
#include "../instructions/PopInstruction.h"
#include "../instructions/MultiplicationInstruction.h"
#include "../instructions/FlagToRegInstruction.h"
#include "../instructions/ModInstruction.h"
#include "../instructions/DivInstruction.h"
#include "../instructions/LeftShiftInstruction.h"
#include "../instructions/RightShiftInstruction.h"
#include "../instructions/BitwiseAndInstruction.h"
#include "../instructions/BitwiseOrInstruction.h"
#include "../instructions/BitwiseXorInstruction.h"


namespace caramel::ir::Pdf {

void PdfIRVisitor::visitCopy(caramel::ir::CopyInstruction *instruction, std::ostream &os) {
    os << "copy: " << instruction->getReturnName();
}

void PdfIRVisitor::visitCopyAddr(CopyAddrInstruction *instruction, std::ostream &os) {
    os << "copy addr: " << instruction->getReturnName();
}

void PdfIRVisitor::visitArrayAccess(ArrayAccessInstruction *instruction, std::ostream &os) {
    os << "array access: " << instruction->getReturnName();
}

void PdfIRVisitor::visitEmpty(caramel::ir::EmptyInstruction *instruction, std::ostream &os) {
    os << "empty";
    CARAMEL_UNUSED(instruction);
}

void PdfIRVisitor::visitProlog(caramel::ir::PrologInstruction *instruction, std::ostream &os) {
    os << "prolog: " << instruction->getReturnName();
}

void PdfIRVisitor::visitEpilog(caramel::ir::EpilogInstruction *instruction, std::ostream &os) {
    os << "epilog";
    CARAMEL_UNUSED(instruction);
}

void PdfIRVisitor::visitMod(caramel::ir::ModInstruction *instruction, std::ostream &os) {
    os << "modulo: "
       << instruction->getLeft() << " % " << instruction->getRight();
}

void PdfIRVisitor::visitDivision(caramel::ir::DivInstruction *instruction, std::ostream &os) {
    os << "division: "
       << instruction->getLeft() << " / " << instruction->getRight();
}

void PdfIRVisitor::visitAddition(caramel::ir::AdditionInstruction *instruction, std::ostream &os) {
    os << "addition: "
       << instruction->getLeft() << " + " << instruction->getRight();
}

void PdfIRVisitor::visitLdConst(caramel::ir::LDConstInstruction *instruction, std::ostream &os) {
    os << "ldconst: " << instruction->getDestination() << " = "
       << instruction->getValue();
}

void PdfIRVisitor::visitNope(caramel::ir::NopInstruction *instruction, std::ostream &os) {
    os << "nop";
    CARAMEL_UNUSED(instruction);
}

void PdfIRVisitor::visitFunctionCall(caramel::ir::FunctionCallInstruction *instruction, std::ostream &os) {
    os << "functionCall: " << instruction->getFunctionName();
}

void PdfIRVisitor::visitReturn(caramel::ir::ReturnInstruction *instruction, std::ostream &os) {
    os << "return: " << instruction->getReturnName();
}

void PdfIRVisitor::visitCallParameter(CallParameterInstruction *instruction, std::ostream &os) {
    os << "call parameter: " << instruction->getValue();
}

void PdfIRVisitor::visitSubtraction(SubtractionInstruction *instruction, std::ostream &os) {
    os << "subtraction: "
       << instruction->getLeft() << " - " << instruction->getRight();
}

void PdfIRVisitor::visitPush(PushInstruction *instruction, std::ostream &os) {
    os << "push: " << instruction->getSource();
}

void PdfIRVisitor::visitPop(PopInstruction *instruction, std::ostream &os) {
    os << "push: " << instruction->getDestination();
}

void PdfIRVisitor::visitMultiplication(MultiplicationInstruction *instruction, std::ostream &os) {
    os << "multiplication: "
       << instruction->getLeft() << " * " << instruction->getRight();
}

void PdfIRVisitor::visitFlagToReg(FlagToRegInstruction *instruction, std::ostream &os) {
    os << "flag to reg: "
       << instruction->getLeft() << " - " << instruction->getRight();
}

void PdfIRVisitor::visitLeftShift(LeftShiftInstruction *instruction, std::ostream &os) {
    os << "left shift: "
       << instruction->getLeft() << " << " << instruction->getRight();
}

void PdfIRVisitor::visitRightShift(RightShiftInstruction *instruction, std::ostream &os) {
    os << "right shift: "
       << instruction->getLeft() << " >> " << instruction->getRight();
}

void PdfIRVisitor::visitBitwiseAnd(BitwiseAndInstruction *instruction, std::ostream &os) {
    os << "bitwise and: "
       << instruction->getLeft() << " & " << instruction->getRight();
}

void PdfIRVisitor::visitBitwiseOr(BitwiseOrInstruction *instruction, std::ostream &os) {
    os << "bitwise or: "
       << instruction->getLeft() << " | " << instruction->getRight();
}

void PdfIRVisitor::visitBitwiseXor(BitwiseXorInstruction *instruction, std::ostream &os) {
    os << "bitwise xor: "
       << instruction->getLeft() << " ^ " << instruction->getRight();
}

} // namespace caramel::ir::Pdf
