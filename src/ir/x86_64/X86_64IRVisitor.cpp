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
#include "../instructions/CopyAddrInstruction.h"
#include "../instructions/ArrayAccessCopyInstruction.h"
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

#define COMMENT_INDENT "                 "


namespace caramel::ir::x86_64 {

static constexpr size_t NB_FUNCTION_CALL_REGISTERS = 6;

std::string X86_64IRVisitor::address(std::string const &symbol) {
    return "(" + symbol + ")";
}

std::string X86_64IRVisitor::toAssembly(ir::BasicBlock::Ptr parentBB, std::string const &anySymbol, size_t bitSize) {
    logger.trace() << "[x86_64] " << "toAssembly(" << "ir" << ", " << anySymbol << ")";
    std::string r;

//     return anySymbol;

    // Is a register
    if (!anySymbol.empty() && anySymbol[0] == '%') {
        r = regToAsm(anySymbol, bitSize);
        // Is a temp var
    } else if (!anySymbol.empty() && anySymbol[0] == '!' && false) {
        r = regToAsm(IR::REGISTER_10, bitSize);
        // TODO: Manage multiple registries
    } else if (!anySymbol.empty() && anySymbol[0] >= '0' && anySymbol[0] <= '9') {
        r = "$" + anySymbol;
    } else {
        r = std::to_string(parentBB->getSymbolIndex(anySymbol)) +
            address(regToAsm(IR::BASE_POINTER_REG, 64)); // always %rbp
    }

    logger.trace() << "[x86_64] " << "  => " << r;
    return r;
}

std::string X86_64IRVisitor::getSizeSuffix(size_t bitSize) {
/*
    b (byte) = 8 bits
    w (word) = 16 bits
    l (long) = 32 bits
    q (quad) = 64 bits
 */
    // FIXME: It's movabsq instead of movq
    switch (bitSize) {
        case 8:
            return "b";
        case 16:
            return "w";
        case 32:
            return "l";
        case 64:
            return "q";
        default:
            logger.fatal() << "Wrong bitSize in X86_64IRVisitor::getSizeSuffix(" << bitSize << ").";
            exit(1);
    }
}

std::string X86_64IRVisitor::regToAsm(std::string register_, size_t bitSize) {
    static std::map<std::string, std::map<size_t, std::string>> const REGISTERS = {
            {IR::COUNTER_REG,       {{8,  "%cl"},   {16, "%cx"},   {32, "%ecx"},  {64, "%rcx"}}},
            {IR::DATA_REG,          {{8,  "%dl"},   {16, "%dx"},   {32, "%edx"},  {64, "%rdx"}}},
            {IR::BASE_REG,          {{8,  "%bl"},   {16, "%bx"},   {32, "%ebx"},  {64, "%rbx"}}},
            {IR::SOURCE_REG,        {{16, "%si"},   {32, "%esi"},  {64, "%rsi"}}},
            {IR::DEST_REG,          {{16, "%di"},   {32, "%edi"},  {64, "%rdi"}}},
            {IR::BASE_POINTER_REG,  {{16, "%bp"},   {32, "%ebp"},  {64, "%rbp"}}},
            {IR::STACK_POINTER_REG, {{16, "%sp"},   {32, "%esp"},  {64, "%rsp"}}},
            {IR::ACCUMULATOR,       {{8,  "%al"},   {16, "%ax"},   {32, "%eax"},  {64, "%rax"}}},
            {IR::ACCUMULATOR_1,     {{8,  "%r10b"}, {16, "%r10w"}, {32, "%r10d"}, {64, "%r10"}}},
            {IR::ACCUMULATOR_2,     {{8,  "%r11b"}, {16, "%r11w"}, {32, "%r11d"}, {64, "%r11"}}},
            {IR::REGISTER_8,        {{8,  "%r8b"},  {16, "%r8w"},  {32, "%r8d"},  {64, "%r8"}}},
            {IR::REGISTER_9,        {{8,  "%r9b"},  {16, "%r9w"},  {32, "%r9d"},  {64, "%r9"}}},
            {IR::REGISTER_10,       {{8,  "%r10b"}, {16, "%r10w"}, {32, "%r10d"}, {64, "%r10"}}},
            {IR::REGISTER_11,       {{8,  "%r11b"}, {16, "%r11w"}, {32, "%r11d"}, {64, "%r11"}}}
    };

    if (REGISTERS.find(register_) == REGISTERS.end()) {
        logger.fatal() << "The register " << register_ << " does not exist!";
        exit(1);
    }
    if (REGISTERS.at(register_).find(bitSize) == REGISTERS.at(register_).end()) {
        logger.fatal() << "The size " << bitSize << " is not valid for " << register_ << "!";
        exit(1);
    }
    return REGISTERS.at(register_).at(bitSize);
}

std::string X86_64IRVisitor::getFCReg(size_t index) {
    static std::map<size_t, std::string> const FC_REGISTERS = {
            {0, IR::DEST_REG},
            {1, IR::SOURCE_REG},
            {2, IR::DATA_REG},
            {3, IR::COUNTER_REG},
            {4, IR::REGISTER_8},
            {5, IR::REGISTER_9}
    };

    if (index >= NB_FUNCTION_CALL_REGISTERS) {
        logger.fatal() << "There are only " << NB_FUNCTION_CALL_REGISTERS << " function call "
                       << "registers, but you're asking for the " << index << "-th one!";
        exit(1);
    }
    return FC_REGISTERS.at(index);
}

void X86_64IRVisitor::visitCopy(caramel::ir::CopyInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting copy: " << instruction->getReturnName();

    auto parameterSize = instruction->getType()->getMemoryLength();
    if (instruction->getParentBlock()->hasSymbol(instruction->getDestination())
            && instruction->getParentBlock()->isSymbolParamArray(instruction->getDestination())) {
        parameterSize = 64;
    }

    std::string src = instruction->getSource();
    if (instruction->getRegisterNumber() != -1) { // an argument passed in registers
        src = getFCReg(size_t(instruction->getRegisterNumber()));
    }

    writeMove(instruction->getParentBlock(), os,
              src, parameterSize,
              instruction->getDestination(), parameterSize);

    os << COMMENT_INDENT << "# copyInstr";
}

void X86_64IRVisitor::visitCopyAddr(CopyAddrInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting copy addr: " << instruction->getReturnName();

//    leaq -32(%rbp), %rax
//    const auto parameterSize = instruction->getType()->getMemoryLength();
    os << "  leaq    " << toAssembly(instruction->getParentBlock(), instruction->getSource(), 64)
       << ", " << toAssembly(instruction->getParentBlock(), instruction->getDestination(), 64);
}

void X86_64IRVisitor::visitArrayAccessCopy(ArrayAccessCopyInstruction *instruction, std::ostream &os) {

    os << "\n  # begin of arrayAccess of " << instruction->getArrayName() << '\n';

    auto length = instruction->getType()->getMemoryLength();

    auto index = instruction->getIndex();
    if (index.empty() || !(index[4] == 'd' || index[1] == 'e')) {
        logger.warning() << "[x86_64] ArrayAccess with other-than-32-bit index.";
    }

    if (!instruction->getParentBlock()->isSymbolParamArray(instruction->getArrayName())) {
        os << "  cltq\n";

//    movl  -16(%rbp,  %rax,   4), %eax
//    movl arra(yName,index,type), destination

        std::string arrayAsmName = toAssembly(
                instruction->getParentBlock(), instruction->getArrayName(), length);
        arrayAsmName.pop_back();

        writeMove(instruction->getParentBlock(), os,
                  instruction->getIndex(), length,
                  IR::DATA_REG, length);
        os << '\n';

        std::string indexAsmReg = regToAsm(IR::DATA_REG, 64);

        if (instruction->isLValue()) {
            std::string destAsmReg = toAssembly(
                    instruction->getParentBlock(), instruction->getDestination(), length);

            os << "  movl    " << destAsmReg << ", " << arrayAsmName << "," << indexAsmReg << "," << (length / 8U) << ")";
        } else {
            os << "  movl    " << arrayAsmName << "," << indexAsmReg << "," << (length / 8U)
               << "), " << regToAsm(IR::ACCUMULATOR, 32);
            os << '\n';

            writeMove(instruction->getParentBlock(), os,
                      IR::ACCUMULATOR, length,
                      instruction->getDestination(), length);
        }

    } else { // Array as argument == pointer

/*
  cltq
  leaq 0(,%rax,4), %rdx
  movq -24(%rbp), %rax
  addq %rdx, %rax
  movl (%rax), %r10d
*/

        std::string indexAsmReg = toAssembly(
                instruction->getParentBlock(), instruction->getIndex(), 64); // length might be wrong
        os << "  movq    " << indexAsmReg << ", " << regToAsm(IR::ACCUMULATOR, 64) << '\n';
        os << "  cltq\n";


        indexAsmReg = regToAsm(IR::ACCUMULATOR, 64);
        os << "  leaq    0(," << indexAsmReg << ", " << (length / 8U) << "), " << regToAsm(IR::DATA_REG, 64) << '\n';

        std::string arrayAsmName = toAssembly(
                instruction->getParentBlock(), instruction->getArrayName(), length);
        os << "  movq    " << arrayAsmName << ", " << regToAsm(IR::ACCUMULATOR, 64) << '\n';

        os << "  addq    " << regToAsm(IR::DATA_REG, 64) << ", " << regToAsm(IR::ACCUMULATOR, 64) << '\n';

        os << "  movq    (" << regToAsm(IR::ACCUMULATOR, 64) << "), " << regToAsm(IR::ACCUMULATOR, 64) << '\n';

        std::string destAsmReg = toAssembly(
                instruction->getParentBlock(), instruction->getDestination(), length);
        os << "  movl    " << regToAsm(IR::ACCUMULATOR, length) << ", " << destAsmReg;
    }

    os << "\n  # end of arrayAccess of " << instruction->getArrayName();
}

void X86_64IRVisitor::visitEmpty(caramel::ir::EmptyInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting empty";

    os << "  # empty with returnName=" << instruction->getReturnName();
}

void X86_64IRVisitor::visitProlog(caramel::ir::PrologInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting prolog: " << instruction->getReturnName();
    os << "  pushq   %rbp" << '\n'
       << "  movq    %rsp, %rbp" << '\n';

    os << "  # TODO: Prolog" << '\n';
    // Fixme: Resize the stack to his real needed size
    os << "  subq $1024, %rsp" << '\n';
}

void X86_64IRVisitor::visitEpilog(caramel::ir::EpilogInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting epilog";

    CARAMEL_UNUSED(instruction);
//    os << "  popq    %rbp" << std::endl; // leave restore %rsp for us
    os << "  leave" << '\n';
    os << "  ret";
}

void X86_64IRVisitor::visitMod(caramel::ir::ModInstruction *instruction, std::ostream &os){
    logger.trace() << "[x86_64] " << "visiting modulo: "
                   << instruction->getLeft() << " % " << instruction->getRight();

    const auto parameterSize = instruction->getType()->getMemoryLength();
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), parameterSize);
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    writeMove(instruction->getParentBlock(), os,
              instruction->getLeft(), parameterSize,
              IR::ACCUMULATOR, parameterSize);
    os << '\n';

    os << "  cltd" << '\n';
    os << "  idivl    " << rightLocation << '\n';

}

void X86_64IRVisitor::visitDivision(caramel::ir::DivInstruction *instruction, std::ostream &os){
    logger.trace() << "[x86_64] " << "visiting division: "
                   << instruction->getLeft() << " / " << instruction->getRight();

    const auto parameterSize = instruction->getType()->getMemoryLength();
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), parameterSize);

    writeMove(instruction->getParentBlock(), os,
              instruction->getLeft(), parameterSize,
              instruction->getReturnName(), parameterSize);
    os << '\n';

    os << "  cltd" << '\n';
    os << "  idivl    " << rightLocation;

}

void X86_64IRVisitor::visitAddition(caramel::ir::AdditionInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting addition: "
                   << instruction->getLeft() << " + " << instruction->getRight();

    const auto parameterSize = instruction->getType()->getMemoryLength();
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    writeMove(instruction->getParentBlock(), os,
              instruction->getRight(), parameterSize,
              instruction->getReturnName(), parameterSize);
    os << '\n';

    writeMove(instruction->getParentBlock(), os,
              instruction->getLeft(), parameterSize,
              IR::ACCUMULATOR, parameterSize);
    os << '\n';

    os << "  add" + getSizeSuffix(parameterSize) + "    "
       << regToAsm(IR::ACCUMULATOR, parameterSize) << ", " << storeLocation;

    os << COMMENT_INDENT << "# addInstr";
}

void X86_64IRVisitor::visitLdConst(caramel::ir::LDConstInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting ldconst: " << instruction->getDestination() << " = "
                   << instruction->getValue();

    writeMove(instruction->getParentBlock(), os,
              instruction->getValue(), 32, // TODO: LDConst 32-bit?
              instruction->getDestination(), 32);
}

void X86_64IRVisitor::visitNope(caramel::ir::NopInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting nop";

    CARAMEL_UNUSED(instruction);
    os << "  nop";
}

void X86_64IRVisitor::visitFunctionCall(caramel::ir::FunctionCallInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting functionCall: " << instruction->getFunctionName();
    os << "  call    " << instruction->getFunctionName();
    for (int i = 0; i < 6 && i < instruction->getArgumentsLength(); i++) {
        os << "\n  popq     " << regToAsm(getFCReg(size_t(i)), 64);
    }
    if (instruction->getArgumentsLength() > 6) {
        os << "\n  addq    $" << (instruction->getArgumentsLength() - 6) * 8 << ", %rsp";
    }
}

void X86_64IRVisitor::visitReturn(caramel::ir::ReturnInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting return: " << instruction->getReturnName();

    const auto returnSize = instruction->getType()->getMemoryLength();
    writeMove(instruction->getParentBlock(), os,
              instruction->getSource(), returnSize,
              IR::ACCUMULATOR, returnSize);
    os << '\n';

    size_t functionContext = instruction->getParentBlock()->getFunctionContext();
     os << "  jmp    " << instruction->getParentBlock()->getCFG()->getFunctionEndBasicBlock(functionContext)->getLabelName();

    // os << "\n#mov return";
}

void X86_64IRVisitor::visitCallParameter(CallParameterInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting call parameter: " << instruction->getValue();

    size_t index = instruction->getIndex();
    if (index < 6) {
        os << "  pushq    " << regToAsm(getFCReg(index), 64) << '\n';

        writeMove(instruction->getParentBlock(), os,
                  instruction->getValue(), 64, // TODO: call parameter = 64-bit?
                  getFCReg(index), 64);

        //  os << "\n#mov call";
    } else {
        os << "  pushq   " << toAssembly(instruction->getParentBlock(), instruction->getValue(), 64) << '\n';
    }
}

void X86_64IRVisitor::visitSubtraction(SubtractionInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting subtraction: "
                   << instruction->getLeft() << " - " << instruction->getRight();

    const auto parameterSize = instruction->getType()->getMemoryLength();
    const std::string leftLocation = toAssembly(instruction->getParentBlock(), instruction->getLeft(), parameterSize);
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    writeMove(instruction->getParentBlock(), os,
              instruction->getRight(), parameterSize,
              instruction->getReturnName(), parameterSize);
    os << '\n';

    os << "  sub" + getSizeSuffix(parameterSize) + "    " << leftLocation << ", " << storeLocation;
}

void X86_64IRVisitor::visitPush(PushInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting push: " << instruction->getSource();

//    size_t typeSize = instruction->getType()->getMemoryLength();

    const std::string sourceLocation = toAssembly(
            instruction->getParentBlock(),
            instruction->getSource(),
            64
            //instruction->getType()->getMemoryLength()
    );

    os << "  pushq" << "   " << sourceLocation;

}

void X86_64IRVisitor::visitPop(PopInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting push: " << instruction->getDestination();

//    size_t typeSize = instruction->getType()->getMemoryLength();

    const std::string destLocation = toAssembly(
            instruction->getParentBlock(),
            instruction->getDestination(),
            64
            //instruction->getType()->getMemoryLength()
    );

    os << "  popq " << "   " << destLocation;

}

void X86_64IRVisitor::visitMultiplication(MultiplicationInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting multiplication: "
                   << instruction->getLeft() << " - " << instruction->getRight();

    const auto parameterSize = instruction->getType()->getMemoryLength();
    const std::string leftLocation = toAssembly(instruction->getParentBlock(), instruction->getLeft(), parameterSize);
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    writeMove(instruction->getParentBlock(), os,
              instruction->getRight(), parameterSize,
              instruction->getReturnName(), parameterSize);
    os << '\n';

    os << "  imul" + getSizeSuffix(parameterSize) + "    "
       << leftLocation << ", " << storeLocation;
}

void X86_64IRVisitor::visitFlagToReg(FlagToRegInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting flag to reg: "
                   << instruction->getLeft() << " - " << instruction->getRight();

//    const auto parameterSize = instruction->getType()->getMemoryLength();
    // TODO : Change 32 to a defined variable
    const std::string leftLocation = toAssembly(instruction->getParentBlock(), instruction->getLeft(), 32);
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), 32);
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), 32);

    os << "  cmp     " << rightLocation << ", " << leftLocation << '\n';

    os << "  set" << instruction->getFtrType() << "    " << "%cl\n";
    os << "  movzbl    %cl, " << storeLocation;
}

void X86_64IRVisitor::visitLeftShift(LeftShiftInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting left shift: "
                   << instruction->getLeft() << " - " << instruction->getRight();

    const auto parameterSize = instruction->getType()->getMemoryLength();
    const std::string leftLocation = toAssembly(instruction->getParentBlock(), instruction->getLeft(), parameterSize);
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), 8);
    //const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    os << "  movb    " << rightLocation << ", %cl" << '\n';
    os << "  sal" + getSizeSuffix(parameterSize) << "    " << "%cl"
       << ", " << leftLocation;
}

void X86_64IRVisitor::visitRightShift(RightShiftInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting right shift: "
                   << instruction->getLeft() << " - " << instruction->getRight();

    const auto parameterSize = instruction->getType()->getMemoryLength();
    const std::string leftLocation = toAssembly(instruction->getParentBlock(), instruction->getLeft(), parameterSize);
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), 8);
    //const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    os << "  movb    " << rightLocation << ", %cl" << '\n';
    os << "  sar" + getSizeSuffix(parameterSize) << "    " << "%cl"
       << ", " << leftLocation;
}

void X86_64IRVisitor::visitBitwiseAnd(BitwiseAndInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting bitwise and: "
                   << instruction->getLeft() << " - " << instruction->getRight();

    const auto parameterSize = instruction->getType()->getMemoryLength();
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), parameterSize);
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    writeMove(instruction->getParentBlock(), os,
              instruction->getLeft(), parameterSize,
              instruction->getReturnName(), parameterSize);
    os << '\n';

    os << "  and"+ getSizeSuffix(parameterSize) << "    " << rightLocation << ", " << storeLocation;
}

void X86_64IRVisitor::visitBitwiseOr(BitwiseOrInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting bitwise or: "
                   << instruction->getLeft() << " - " << instruction->getRight();

    const auto parameterSize = instruction->getType()->getMemoryLength();
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), parameterSize);
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    writeMove(instruction->getParentBlock(), os,
              instruction->getLeft(), parameterSize,
              instruction->getReturnName(), parameterSize);
    os << '\n';

    os << "  or"+ getSizeSuffix(parameterSize) << "    " << rightLocation << ", " << storeLocation;
}

void X86_64IRVisitor::visitBitwiseXor(BitwiseXorInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting bitwise xor: "
                   << instruction->getLeft() << " - " << instruction->getRight();

    const auto parameterSize = instruction->getType()->getMemoryLength();
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), parameterSize);
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    writeMove(instruction->getParentBlock(), os,
              instruction->getLeft(), parameterSize,
              instruction->getReturnName(), parameterSize);
    os << '\n';

    os << "  xor"+ getSizeSuffix(parameterSize) << "    " << rightLocation << ", " << storeLocation ;
}

void X86_64IRVisitor::writeMove(BasicBlock::Ptr const &bb, std::ostream &os,
                                std::string src, size_t srcSize,
                                std::string dest, size_t destSize) {
    static std::map<size_t, std::string> const movInstr = {
            {8, "movb"},
            {16, "movw"},
            {32, "movl"},
            {64, "movq"}
    };

    auto const maxSize = std::max(srcSize, destSize);

    // Special case for %si and %di, which aren't 8-bit
    if (srcSize == 8 && (src == IR::SOURCE_REG || src == IR::DEST_REG)) {
        writeMove(bb, os, src, 32, IR::REGISTER_10, 32);
        os << '\n';
        src = IR::REGISTER_10;
    }
    if (destSize == 8 && (dest == IR::SOURCE_REG || dest == IR::DEST_REG)) {
        writeMove(bb, os, dest, 32, IR::REGISTER_11, 32);
        os << '\n';
        dest = IR::REGISTER_11;
    }
    auto srcAsm = toAssembly(bb, src, srcSize);
    auto destAsm = toAssembly(bb, dest, destSize);

    bool const srcMem = srcAsm[0] != '%' && srcAsm[0] != '$';
    bool const destMem = destAsm[0] != '%' && destAsm[0] != '$';
    if (srcMem && destMem) {
        os << "  " << movInstr.at(maxSize) << "    " << srcAsm << ", " << regToAsm(IR::DATA_REG, maxSize);
        os << '\n';
        os << "  " << movInstr.at(maxSize) << "    " << regToAsm(IR::DATA_REG, maxSize) << ", " << destAsm;
    } else {
        os << "  " << movInstr.at(maxSize) << "    " << srcAsm << ", " << destAsm;
    }
}

} // namespace caramel::ir::x86_64
