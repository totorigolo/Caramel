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



namespace caramel::ir::x86_64 {

static constexpr size_t NB_FUNCTION_CALL_REGISTERS = 6;

std::string X86_64IRVisitor::address(std::string const &symbol) {
    return "(" + symbol + ")";
}

std::string X86_64IRVisitor::registerToAssembly(std::string const &register_, size_t bitSize) {
    logger.trace() << "[x86_64] " << "registerToAssembly(" << register_ << ")";
    return getRegister(register_, bitSize);
}

std::string X86_64IRVisitor::toAssembly(ir::BasicBlock::Ptr parentBB, std::string const &anySymbol, size_t bitSize) {
    logger.trace() << "[x86_64] " << "toAssembly(" << "ir" << ", " << anySymbol << ")";
    std::string r;

//     return anySymbol;

    // Is a register
    if (!anySymbol.empty() && anySymbol[0] == '%') {
        r = registerToAssembly(anySymbol, bitSize);
        // Is a temp var
    } else if (!anySymbol.empty() && anySymbol[0] == '!') {
        r = registerToAssembly(IR::REGISTER_10, bitSize);
        // TODO: Manage multiple registries
    } else if (!anySymbol.empty() && anySymbol[0] >= '0' && anySymbol[0] <= '9') {
        r = "$" + anySymbol;
    } else {
        r = std::to_string(parentBB->getSymbolIndex(anySymbol)) +
            address(registerToAssembly(IR::REGISTER_BASE_POINTER, 64)); // always %rbp
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

std::string X86_64IRVisitor::getRegister(std::string register_, size_t bitSize) {
    static bool init = false;
    static std::map<std::string, std::map<size_t, std::string>> REGISTERS;
    if (!init) {
        REGISTERS[IR::ACCUMULATOR][8] = "%al";
        REGISTERS[IR::ACCUMULATOR][16] = "%ax";
        REGISTERS[IR::ACCUMULATOR][32] = "%eax";
        REGISTERS[IR::ACCUMULATOR][64] = "%rax";
        REGISTERS[IR::ACCUMULATOR_1][8] = "%r10b";
        REGISTERS[IR::ACCUMULATOR_1][16] = "%r10w";
        REGISTERS[IR::ACCUMULATOR_1][32] = "%r10d";
        REGISTERS[IR::ACCUMULATOR_1][64] = "%r10";
        REGISTERS[IR::ACCUMULATOR_2][8] = "%r11b";
        REGISTERS[IR::ACCUMULATOR_2][16] = "%r11w";
        REGISTERS[IR::ACCUMULATOR_2][32] = "%r11d";
        REGISTERS[IR::ACCUMULATOR_2][64] = "%r11";
        REGISTERS[IR::REGISTER_10][8] = "%r10b";
        REGISTERS[IR::REGISTER_10][16] = "%r10w";
        REGISTERS[IR::REGISTER_10][32] = "%r10d";
        REGISTERS[IR::REGISTER_10][64] = "%r10";
        REGISTERS[IR::REGISTER_11][8] = "%r11b";
        REGISTERS[IR::REGISTER_11][16] = "%r11w";
        REGISTERS[IR::REGISTER_11][32] = "%r11d";
        REGISTERS[IR::REGISTER_11][64] = "%r11";
        REGISTERS[IR::REGISTER_BASE_POINTER][8] = "%bp"; // < using the 16-bit register
        REGISTERS[IR::REGISTER_BASE_POINTER][16] = "%bp";
        REGISTERS[IR::REGISTER_BASE_POINTER][32] = "%ebp";
        REGISTERS[IR::REGISTER_BASE_POINTER][64] = "%rbp";
        REGISTERS[IR::REGISTER_STACK_POINTER][8] = "%sp"; // < using the 16-bit register
        REGISTERS[IR::REGISTER_STACK_POINTER][16] = "%sp";
        REGISTERS[IR::REGISTER_STACK_POINTER][32] = "%esp";
        REGISTERS[IR::REGISTER_STACK_POINTER][64] = "%rsp";
        init = true;
    }

    if (REGISTERS[register_].find(bitSize) == REGISTERS[register_].end()) {
        logger.fatal() << "The size " << bitSize << " is not valid!";
        exit(1);
    }
    return REGISTERS[register_][bitSize];
}

std::string X86_64IRVisitor::getFunctionCallRegister(size_t index, size_t bitSize) {
    static bool init = false;
    static std::map<size_t, std::map<size_t, std::string>> FC_REGISTERS;
    if (!init) {
        FC_REGISTERS[0][8] = "%di"; // < using the 16-bit register
        FC_REGISTERS[0][16] = "%di";
        FC_REGISTERS[0][32] = "%edi";
        FC_REGISTERS[0][64] = "%rdi";
        FC_REGISTERS[1][8] = "%si"; // < using the 16-bit register
        FC_REGISTERS[1][16] = "%si";
        FC_REGISTERS[1][32] = "%esi";
        FC_REGISTERS[1][64] = "%rsi";
        FC_REGISTERS[2][8] = "%dx"; // < using the 16-bit register
        FC_REGISTERS[2][16] = "%dx";
        FC_REGISTERS[2][32] = "%edx";
        FC_REGISTERS[2][64] = "%rdx";
        FC_REGISTERS[3][8] = "%cx"; // < using the 16-bit register
        FC_REGISTERS[3][16] = "%cx";
        FC_REGISTERS[3][32] = "%ecx";
        FC_REGISTERS[3][64] = "%rcx";
        FC_REGISTERS[4][8] = "%r8d"; // < using the 32-bit register
        FC_REGISTERS[4][16] = "%r8d"; // < using the 32-bit register
        FC_REGISTERS[4][32] = "%r8d";
        FC_REGISTERS[4][64] = "%r8";
        FC_REGISTERS[5][8] = "%r9d"; // < using the 32-bit register
        FC_REGISTERS[5][16] = "%r9d"; // < using the 32-bit register
        FC_REGISTERS[5][32] = "%r9d";
        FC_REGISTERS[5][64] = "%r9";
        init = true;
    }

    if (index >= NB_FUNCTION_CALL_REGISTERS) {
        logger.fatal() << "There are only " << NB_FUNCTION_CALL_REGISTERS << " function call "
                       << "registers, but you're asking for the " << index << "-th one!";
        exit(1);
    }
    if (FC_REGISTERS[index].find(bitSize) == FC_REGISTERS[index].end()) {
        logger.fatal() << "The size " << bitSize << " is not valid!";
        exit(1);
    }
    return FC_REGISTERS[index][bitSize];
}

void X86_64IRVisitor::visitCopy(caramel::ir::CopyInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting copy: " << instruction->getReturnName();

    const auto parameterSize = instruction->getType()->getMemoryLength();
    os << "  mov" + getSizeSuffix(parameterSize) + "    ";

    if (instruction->getRegisterNumber() != -1) {
        os << getFunctionCallRegister(instruction->getRegisterNumber(), 32);
    } else {
        os << toAssembly(instruction->getParentBlock(), instruction->getSource(), parameterSize);
    }
    os << ", " << toAssembly(instruction->getParentBlock(), instruction->getDestination(), parameterSize);
    // os << "\n#mov copy";
}

void X86_64IRVisitor::visitArrayAccessCopy(ArrayAccessCopyInstruction *instruction, std::ostream &os) {

//    cltq
//    movl -16(%rbp,%rax,4), %eax

//    cltq
//    movl arra(yName,index,type), destination

    auto length = instruction->getType()->getMemoryLength();

    auto index = instruction->getIndex();
    if (index.empty() || !(index[4] == 'd' || index[1] == 'e')) {
        logger.warning() << "[x86_64] ArrayAccess with other-than-32-bit index.";
    }

    os << "  cltq\n";

    std::string arrayAsmName = toAssembly(
            instruction->getParentBlock(), instruction->getArrayName(), length);
    arrayAsmName.pop_back();

    std::string indexAsmReg = toAssembly(
            instruction->getParentBlock(), instruction->getIndex(), length); // length might be wrong
    if (indexAsmReg[4] == 'd') {
        indexAsmReg.pop_back();
    }
    if (indexAsmReg[1] == 'e') {
        indexAsmReg[1] = 'r';
    }

    std::string destAsmReg = toAssembly(
            instruction->getParentBlock(), instruction->getDestination(), length);

    if (instruction->isLValue()) {
        os << "  movl " << destAsmReg << ", " << arrayAsmName << "," << indexAsmReg << "," << (length / 8U) << ")";
    } else {
        os << "  movl " << arrayAsmName << "," << indexAsmReg << "," << (length / 8U)
           << "), " << destAsmReg;
    }
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
    const std::string leftLocation = toAssembly(instruction->getParentBlock(), instruction->getLeft(), parameterSize);
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), parameterSize);
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    os << "  movl    " << leftLocation
       << ", " << "%eax" << '\n'; // TODO : Not write %eax directly
    os << "  cltd" << '\n';

    os << "  idivl    " << rightLocation << '\n';

}

void X86_64IRVisitor::visitDivision(caramel::ir::DivInstruction *instruction, std::ostream &os){
    logger.trace() << "[x86_64] " << "visiting division: "
                   << instruction->getLeft() << " / " << instruction->getRight();

    const auto parameterSize = instruction->getType()->getMemoryLength();
    const std::string leftLocation = toAssembly(instruction->getParentBlock(), instruction->getLeft(), parameterSize);
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), parameterSize);
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    os << "  movl    " << leftLocation
       << ", " << storeLocation << '\n';
    os << "  cltd" << '\n';

    os << "  idivl    " << rightLocation;

}

void X86_64IRVisitor::visitAddition(caramel::ir::AdditionInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting addition: "
                   << instruction->getLeft() << " + " << instruction->getRight();

    const auto parameterSize = instruction->getType()->getMemoryLength();
    const std::string leftLocation = toAssembly(instruction->getParentBlock(), instruction->getLeft(), parameterSize);
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), parameterSize);
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    os << "  movl    " << rightLocation
                       << ", " << storeLocation << '\n';

    os << "  add" + getSizeSuffix(parameterSize) + "    "
       << leftLocation << ", " << storeLocation;

}

void X86_64IRVisitor::visitLdConst(caramel::ir::LDConstInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting ldconst: " << instruction->getDestination() << " = "
                   << instruction->getValue();

    os << "  movl    " << toAssembly(instruction->getParentBlock(), instruction->getValue())
       << ", " << toAssembly(instruction->getParentBlock(), instruction->getDestination());

    // os << "\n#mov ldconst";
}

void X86_64IRVisitor::visitNope(caramel::ir::NopInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting nop";

    CARAMEL_UNUSED(instruction);
    os << "  nop";
}

void X86_64IRVisitor::visitFunctionCall(caramel::ir::FunctionCallInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting functionCall: " << instruction->getFunctionName();
    os << "  call    " << instruction->getFunctionName();
    for(int i = 0; i < 6 && i < instruction->getArgumentsLength(); i++) {
        os << "\n  popq     " << getFunctionCallRegister(i, 64);
    }
    if(instruction->getArgumentsLength() > 6) {
        os << "\n  addq    $" << (instruction->getArgumentsLength() - 6) * 8 << ", %rsp";
    }
}

void X86_64IRVisitor::visitReturn(caramel::ir::ReturnInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting return: " << instruction->getReturnName();

    const auto returnSize = instruction->getType()->getMemoryLength();
    os << "  mov" + getSizeSuffix(returnSize) + "    "
       << toAssembly(instruction->getParentBlock(), instruction->getSource(), returnSize)
       << ", " << toAssembly(instruction->getParentBlock(), IR::ACCUMULATOR, 32) // TODO: See TODO in getSizeSuffix()
       << "\n";
    size_t functionContext = instruction->getParentBlock()->getFunctionContext();

     os << "  jmp    " << instruction->getParentBlock()->getCFG()->getFunctionEndBasicBlock(functionContext)->getLabelName();

    // os << "\n#mov return";
}

void X86_64IRVisitor::visitCallParameter(CallParameterInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting call parameter: " << instruction->getValue();

    int index = instruction->getIndex();
    if (index < 6) {
        os << "  pushq    " << getFunctionCallRegister(index, 64) << '\n';
        os << "  mov" + getSizeSuffix(32) + "    " << toAssembly(instruction->getParentBlock(), instruction->getValue(), 32)
           << ", " << getFunctionCallRegister(index, 32) << '\n';

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
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), parameterSize);
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    os << "  movl    " << rightLocation // TODO: We might want to change "l" dynamically ?
       << ", " << storeLocation << '\n';

    os << "  sub" + getSizeSuffix(parameterSize) + "    "
       << leftLocation << ", " << storeLocation;
}

void X86_64IRVisitor::visitPush(PushInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting push: " << instruction->getSource();

    size_t typeSize = instruction->getType()->getMemoryLength();

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

    size_t typeSize = instruction->getType()->getMemoryLength();

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
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), parameterSize);
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    os << "  movl    " << rightLocation // TODO: We might want to change "l" dynamically ?
       << ", " << storeLocation << '\n';

    os << "  imul" + getSizeSuffix(parameterSize) + "    "
       << leftLocation << ", " << storeLocation;
}

void X86_64IRVisitor::visitFlagToReg(FlagToRegInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting flag to reg: "
                   << instruction->getLeft() << " - " << instruction->getRight();

    const auto parameterSize = instruction->getType()->getMemoryLength();
    // TODO : Change 64 to a defined variable
    const std::string leftLocation = toAssembly(instruction->getParentBlock(), instruction->getLeft(), 64);
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), 64);
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), 8);

    os << "  cmp     " << rightLocation << ", " << leftLocation << '\n';

    os << "  set" << instruction->getFtrType() << "    " << storeLocation;

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
    const std::string leftLocation = toAssembly(instruction->getParentBlock(), instruction->getLeft(), parameterSize);
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), parameterSize);
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    os << "  mov"+ getSizeSuffix(parameterSize) << "    " << leftLocation << ", " << storeLocation << '\n';
    os << "  and"+ getSizeSuffix(parameterSize) << "    " << rightLocation << ", " << storeLocation;

}

void X86_64IRVisitor::visitBitwiseOr(BitwiseOrInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting bitwise or: "
                   << instruction->getLeft() << " - " << instruction->getRight();

    const auto parameterSize = instruction->getType()->getMemoryLength();
    const std::string leftLocation = toAssembly(instruction->getParentBlock(), instruction->getLeft(), parameterSize);
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), parameterSize);
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    os << "  mov"+ getSizeSuffix(parameterSize) << "    " << leftLocation << ", " << storeLocation << '\n';
    os << "  or"+ getSizeSuffix(parameterSize) << "    " << rightLocation << ", " << storeLocation;

}

void X86_64IRVisitor::visitBitwiseXor(BitwiseXorInstruction *instruction, std::ostream &os) {
    logger.trace() << "[x86_64] " << "visiting bitwise xor: "
                   << instruction->getLeft() << " - " << instruction->getRight();

    const auto parameterSize = instruction->getType()->getMemoryLength();
    const std::string leftLocation = toAssembly(instruction->getParentBlock(), instruction->getLeft(), parameterSize);
    const std::string rightLocation = toAssembly(instruction->getParentBlock(), instruction->getRight(), parameterSize);
    const std::string storeLocation = toAssembly(instruction->getParentBlock(), instruction->getReturnName(), parameterSize);

    os << "  mov"+ getSizeSuffix(parameterSize) << "    " << leftLocation << ", " << storeLocation << '\n';
    os << "  xor"+ getSizeSuffix(parameterSize) << "    " << rightLocation << ", " << storeLocation ;

}

} // namespace caramel::ir::x86_64
